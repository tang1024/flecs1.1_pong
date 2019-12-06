vflecs Documentation v0.1
=========================

1. Programming Element
======================
- API-defined type
    - ecs_rows_t
- Declarative macro
    - ECS_COLUMN
    - ECS_ENTITY
    - ECS_COMPONENT
    - ECS_SYSTEM
- Functions
    - ecs_init()
- Enumeration constant
    - EcsOnUpdate
- Imperative macro's (function wrappers)
    - ecs_add()
    - ecs_delete() means "empty"

2. Handle
=========
There are 2 kinds of handles: 

- Entity handle **ecs_entity_t**
- Type handle **ecs_type_t**.
- (World handle **ecs_world_t**)

2.1. Entity Handle
------------------
Entity handle is used for refering a single "thing": 

- Entity, Component, System, Type, ...

.. code-block:: c

    typedef uint64_t ecs_entity_t;

2.2. Type Handle
----------------
Type handle is used for refering a type. 
An entity may contain components. 
To describe which components an entity has, the "type" of the entity is introduced. 
Type hanlde points to dynamic data based on the components added and removed.

.. code-block:: c

    struct ecs_vector_t {
        uint32_t count;
        uint32_t size;
    };
    typedef struct ecs_vector_t ecs_vector_t;
    
    typedef const ecs_vector_t* ecs_type_t;

2.2.1. Type Model
`````````````````
.. code-block:: c

    // a dynamic array of ecs_entity_t with the first element of ecs_vector_t
    // ecs_vector_t -> count & size
    // ecs_entity_t -> builtin component ecs_entity_t
    // ecs_entity_t
    // ecs_entity_t -> custom added component ecs_entity_t
    // ecs_entity_t
    // ecs_entity_t -> CHILDOF | parent ecs_entity_t
    // ecs_entity_t -> INSTANCEOF | base ecs_entity_t
    // ...
    
2.3. Creation of Handles
------------------------
.. code-block:: c

    ecs_entity_t ecs_new_xxx(
        ecs_world_t *world,
        const char *id,
        size_t size);
        
.. code-block:: c
    
    ecs_new_xxx(world, #id, #__VA_ARGS__);

A set of functions to create **entity handle** and **type handle**; 
**entity handle** is returned.

.. code-block:: c

    // calls ecs_new_entity() & ecs_type_from_entity()
    ECS_ENTITY();
        
    // calls ecs_new_component() & ecs_type_from_entity()
    ECS_COMPONENT;
    
    // calls ecs_new_system() & ecs_type_from_entity()
    ECS_SYSTEM();
    
    // calls ecs_new_type() & ecs_type_from_entity()
    ECS_TYPE();
    
    // calls ecs_new_prefab() & ecs_type_from_entity()
    ECS_PREFAB();
    
    // etc.
    
Macros to create **entity handle** and **type handle**; 
both handles are returned.

2.4. Literal of Declared Handle
-------------------------------
Normal Cases
````````````
- Entity handle is named as "id".
- Type handle is named as "T##id".

Example

.. code-block:: c

    #define ECS_TYPE(world, id, ...) \
        ecs_entity_t id = ecs_new_type(world, #id, #__VA_ARGS__);\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, id);\
        (void)id;\
        (void)ecs_type(id);\
        
.. code-block:: c
 
    ECS_TYPE(world, MyType, Position);
    
Create a type entity handle named "id" and a type type handle named "T##id":

.. code-block:: c
 
    #define ecs_type(type) T##type

An imperative macro expending to the name of the ecs type handle of an entity except component.

Exception
---------
- Component entity handle is named as "E##id".
- Component type handle is named as "T##id".

Example

.. code-block:: c

    #define ECS_COMPONENT(world, id) \
        ECS_ENTITY_VAR(id) = ecs_new_component(world, #id, sizeof(id));\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, ecs_entity(id));\
        (void)ecs_entity(id);\
        (void)ecs_type(id);\
        
.. code-block:: c
        
    ECS_COMPONENT(world, Position);
    
Get a component entity handle "E##id" and a component type handle "T##id":        

.. code-block:: c
    
    #define ecs_entity(type) E##type

An imperative macro expending to the name of the ecs entity handle of a component.

Get type handle from entity handle
``````````````````````````````````
.. code-block:: c

    ecs_type_t ecs_type_from_entity(
        ecs_world_t *world, 
        ecs_entity_t entity);
    
Get the corresponding type handle from any entity handle.

.. code-block:: c

    ecs_type_t ecs_get_type(
        ecs_world_t *world,
        ecs_entity_t entity);
        
???

3. World
========
A world is a container in which entities, components and systems can be stored and evaluated. 
An application can create any number of worlds. 
Data between worlds is not shared. 
If the application wants to share data between worlds, this has to be done manually. 

.. code-block:: c

    ecs_world_t *world = ecs_init();

A world in ECS can be created with the ecs_init function.
   
4. Entity
=========
Entities are light-weight objects that represent "things" in the application.

- Entity, Component, System, Type, ...

Therefore a component is actually an entity with builtin component `EcsComponent`.

- Component has entity handle.
- Component has type handle that points to the entity handle of EcsComponent.

Entity cannot be actually created and deleted, but can be emptied.

New and Delete Entity
---------------------
.. code-block:: c
    
    ecs_entity_t _ecs_new(
        ecs_world_t *world,
        ecs_type_t type);
        
    #define ecs_new(world, type)\
        _ecs_new(world, T##type)
        
.. code-block:: c
    
    ecs_new(world, type);
        
Obtain an unused entity handle.

.. code-block:: c   
 
    ecs_new(world, 0);

Obtain an unused empty entity handle.

.. code-block:: c

    ecs_new(world, Position);

Obtain an unused entity handle with specified components or types.

.. code-block:: c

    ecs_entity_t e = ecs_new_w_count(world, Position, 100);

Obtain consecutive entity handles. 
The first of the obtained entity handles is returned.

.. code-block:: c

    ecs_new_component()
    ecs_new_entity()
    new_row_system()
    ecs_new_col_system()
    ecs_new_type()
    ecs_new_prefab()
    _ecs_new_child()
    _ecs_new_instance()
    _ecs_set_ptr()

Functions which call ecs_new().

.. code-block:: c

    ecs_delete(world, e);

Remove all the components and empty an entity.

Store of Entity
---------------
- Internal Entity Index: a sparse set that maps the entity identitfiers to where the entities are stored in an archetype.
- Entity identitfier: the index in the sparse array.

The dense array of the sparse set is used to test if an entity identifier is alive, 
and allows for iterating all entities. 

The data stored in the sparse set is a pointer to the archetype the entity is stored in, 
combined with an row (array index) that points to where in the component arrays the entity is stored.

Systems will occasionally need access to the entity identifier. Because systems access the entities directly from the archetypes and not from the entity index, they need to obtain the entity identifier in another way. Flecs accomplishes this by storing the entity identifiers as an additional column columns in an archetype. Applications can access the entity identifiers using row->entities, or by requesting the column at index 0:

ECS_COLUMN(rows, ecs_entity_t, entities, 0);

Entity Identifier
-----------------
Entities in flecs may have an optional string-based identifier. 

.. code-block:: c

    ecs_set(world, e, EcsId, {"MyEntity"});

An identifier can be added to an entity by setting the EcsId component, like above.

Lookup Entity
`````````````
.. code-block:: c

    ecs_entity_t e = ecs_lookup(world, "MyEntity");

After a string identifier is added, the entity can be looked up like above.

.. code-block:: c

    ECS_ENTITY(world, MyEntity, Point);

**Automatical Register**

Additionally, applications can define entities with the ECS_ENTITY macro, which automatically adds **EcsId** and initializes it with the provided name:

Components, systems, tasks, types and prefabs automatically register the EcsId component when they are created, and can thus be looked up with ecs_lookup.

5. Component
============
New a Component
---------------
.. code-block:: c

    #define ECS_COMPONENT(world, id) \
        ECS_ENTITY_VAR(id) = ecs_new_component(world, #id, sizeof(id));\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, ecs_entity(id));\
        (void)ecs_entity(id);\
        (void)ecs_type(id);\
        
Macro to create `component entity` and `component type`.
       
.. code-block:: c

    ecs_entity_t ecs_new_component(
        ecs_world_t *world,
        const char *id,
        size_t size);

Function to create `component entity` and `component type`.

.. code-block:: c

    ecs_delete(world, e);

Possible to delete a component?

Add and Remove Component
------------------------
.. code-block:: c
    
    void _ecs_add(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
        
    #define ecs_add(world, entity, type)\
        _ecs_add(world, entity, T##type)
    
.. code-block:: c
    
    ecs_add(world, entity_handle, component_ctype_name);

Add a components to an entity by component type.

.. code-block:: c
    
    void _ecs_remove(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
    
    #define ecs_remove(world, entity, type)\
        _ecs_remove(world, entity, T##type)

.. code-block:: c
    
    ecs_remove(world, entity_handle, component_ctype_name);
    
Remove a component from an entity by component type.

Has a Component
---------------
.. code-block:: c

    ecs_has

Set and Get Component Value
---------------------------
.. code-block:: c
    
    ecs_entity_t _ecs_set_ptr(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_entity_t component,
        size_t size,
        void *ptr);
        
    #define ecs_set(world, entity, component, ...)\
        _ecs_set_ptr(world, entity, ecs_entity(component), sizeof(component), &(component)__VA_ARGS__)

Assign a specific value to a component of an entity.

.. code-block:: c
        
    void* _ecs_get_ptr(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
        
    #define ecs_get(world, entity, type)\
        (*(type*)_ecs_get_ptr(world, entity, T##type))
        
Get the value of the component from an entity.  

Event
-----
- EcsOnAdd
- EcsOnRemove

6. Type
=======
A type identifies a collection of 1..n entities(ecs things). 

- Group components so that they can be added to an entity with a single ecs_add call
- Group systems so that they can be enabled or disabled with a single ecs_enable call

.. code-block:: c
    
    #define ECS_TYPE(world, id, ...) \
        ecs_entity_t id = ecs_new_type(world, #id, #__VA_ARGS__);\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, id);\
        (void)id;\
        (void)ecs_type(id);\
        
.. code-block:: c

    ECS_TYPE(world, MovableType, Position, Velocity);
    
Create a `type entity` and a `type type`.

Types handles be used interchangeably with components for most API operations. 

.. code-block:: c
    
    ecs_entity_t _ecs_new(
        ecs_world_t *world,
        ecs_type_t type);
    
    #define ecs_new(world, type)\
        _ecs_new(world, T##type)
    
.. code-block:: c

    ecs_new(world, MovableType);
    ecs_new(world, Position);    
    
Create a new entity.
        
Add and Remove Type
-------------------
.. code-block:: c
  
    void _ecs_add(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
        
    #define ecs_add(world, entity, type)\
        _ecs_add(world, entity, T##type)
        
.. code-block:: c

    ecs_add(world, entity, Position)
    
Add multiple components to an entity by type. 

.. code-block:: c

    void _ecs_remove(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
        
    #define ecs_remove(world, entity, type)\
        _ecs_remove(world, entity, T##type)

Remove multiple components from an entity by type.

7. Tag
======
.. code-block:: c

    ECS_TAG(world, MyTag);
    
A tag can be defined with the ECS_TAG macro.

.. code-block:: c   
    
    ecs_add(world, e, MyTag);

Tags can be added/removed like any other component.

8. Advanced Entity
==================
Hierarchy & Container
---------------------
A container is an entity that can contain other entities. 
There are several methods to add a child entity to a container entity. 

.. code-block:: c

    ecs_entity_t _ecs_new_child(
        ecs_world_t *world,
        ecs_entity_t parent,
        ecs_type_t type);
        
.. code-block:: c

    ecs_new_child(world, parent_entity, 0);

Create a new empty entity which also specifies a parent entity.
- It adds the "CHILDOF | parent_entity" as a ecs_entity_t to the child entity type handle?

.. code-block:: c
    ecs_entity_t parent = ecs_new(world, 0);
    ecs_entity_t child = ecs_new_child(world, parent, 0);
    
The easiest way is with the ecs_new_child function.

.. code-block:: c

    ecs_adopt(world, e, parent_entity);

A container entity adopts an entity. 
- It adds the EcsContainer component(tag) to the parent type handle if it didn't have it already. 

.. code-block:: c

    ecs_entity_t parent = ecs_new(world, 0);
    ecs_entity_t child = ecs_new(world, 0);
    ecs_adopt(world, child, parent);

Alternatively, you can add an entity to a container entity after its creation using ecs_adopt.

.. code-block:: c

    ecs_orphan(world, e, my_root);

Orphan entities from containers.

.. code-block:: c

    if (ecs_contains(world, parent, child) {
        printf("entity %u is a child of %u\n", child, parent);
    }

With the ecs_contains function you can check whether an entity contains another entity:

Inheritance & Prefab
--------------------
Prefab
``````
Prefabs are a special kind of entity that enable applications to **reuse components values across entities**.

.. code-block:: c

    ECS_PREFAB(world, CirclePrefab, EcsCircle, EcsPosition2D);

To create a prefab, you can use the ECS_PREFAB macro, or ecs_new_prefab function.
This defines a prefab with the EcsCircle and EcsPosition2D components. 

.. code-block:: c

    ecs_entity_t e1 = ecs_new(world, CirclePrefab);
    ecs_entity_t e2 = ecs_new(world, CirclePrefab);

This will make the EcsCircle and EcsPosition2D components available on entities e1 and e2.
Component values of EcsCircle and EcsPosition2D are now shared between entities, and stored only once in memory.

Override
````````
Entities can override component values from a prefab, 
by either adding or setting a component on themselves, 
using ecs_add or ecs_set. 

When a component is added using ecs_add, it will be initialized with the component value of the prefab.

9. System
=========
Enable System
-------------
.. code-block:: c

    ecs_enable(world, LogPoints, false);

10. Task
========
A task is a system that has no interest expression. Tasks are run once every frame.

.. code-block:: c
    ECS_SYSTEM(world, MyTask, EcsOnUpdate, 0);
    
Tasks are defined the same way as normal systems, but instead of an interest expression, you specify 0.

Feature
=======
A feature is a type that contains systems.

.. code-block:: c

    ECS_TYPE(world, MyFeature, SystemA, SystemB);

    ecs_enable(World, MyFeature, true);
    
Enable/disable multiple systems with a single API call.

System Filter Expression
========================
CONTAINER.Component
-------------------

.. code-block:: c

    ECS_SYSTEM(world, MySystem, EcsOnUpdate, CONTAINER.Foo, Bar);

Entity should be a container and should have `Foo` Component.

Module
======
Modules are used to group entities / components / systems.

.. code-block:: c

    ECS_IMPORT(world, EcsComponentsTransform, 0);
    
can be imported with the ECS_IMPORT macro.

- This will invoke the EcsComponentsTransform function, which will define the entities / components / systems. 
- Furthermore, the macro will declare the variables to the entity / component / system handles to the local scope, so that they can be accessed by the code.

APIs
====
.. code-block:: c

    void* _ecs_get_ptr(
        ecs_world_t *world,
        ecs_entity_t entity,
        ecs_type_t type);
        
    #define ecs_get_ptr(world, entity, type)\
        _ecs_get_ptr(world, entity, T##type)

.. code-block:: c
        
    Position *p_base = ecs_get_ptr(world, my_base, Position);

Get the C point of the component which is guaranteed to remain valid until one of the operations happens such as adding, removing, creating or deleting entities.

.. code-block:: c

    ecs_lookup()
    
Find entities, components and systems that are named (that have the EcsId component). 

.. code-block:: c

    ecs_progress(0)
    
Let Flecs determine delta_time automatically, by specifying 0 to ecs_progress.
    
.. code-block:: c

    ecs_set_target_fps
    
Set a target FPS. 
ecs_progress function will automatically insert sleeps to make sure your application runs at the specified FPS.

OS API
------
.. code-block:: c
    
    // Set default calls, like malloc, free
    ecs_os_set_api_defaults();

    // Set the custom API callbacks
    ecs_os_set_api(&os_api);

main()
======
- importing modules
- creation of entities

