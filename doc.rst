Element
=======
- API type
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

Handle
======
- entity handle: ecs_entity_t
- type handle: ecs_type_t

Entity Handle "ecs_entity_t"
----------------------------
Create entity handle

- ECS_ENTITY
- ECS_COMPONENT
- ECS_SYSTEM

Type Handle "ecs_type_t"
------------------------
Create type handle

- ECS_COMPONENT
- ECS_TYPE
- ECS_PREFAB
- etc.

Normal Case
```````````
Get a type entity handle "id" and a type type handle "T##id":

    ECS_TYPE(world, MyType, Position)
    
.. code-block:: c

    #define ECS_TYPE(world, id, ...) \
        ecs_entity_t id = ecs_new_type(world, #id, #__VA_ARGS__);\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, id);\
        (void)id;\
        (void)ecs_type(id);\
        
Get the name of the type handle:

.. code-block:: c
 
    #define ecs_type(type) T##type

Exception
`````````    
Get a component entity handle "E##id" and a component type handle "T##id":

ECS_COMPONENT(world, Position)

.. code-block:: c

    #define ECS_COMPONENT(world, id) \
        ECS_ENTITY_VAR(id) = ecs_new_component(world, #id, sizeof(id));\
        ECS_TYPE_VAR(id) = ecs_type_from_entity(world, ecs_entity(id));\
        (void)ecs_entity(id);\
        (void)ecs_type(id);\
        
Get the name of the entity handle:

.. code-block:: c
    
    #define ecs_entity(type) E##type

Type
====
`ecs_new()`

.. code-block:: c

    #define ecs_new(world, type)\
        _ecs_new(world, T##type)

Calls _ecs_new().
        
Specify a type.

Entity
======
Entity is just a *number*, an integer in Flecs.

In Flecs, component and system are stored in entity.

A component is actually an entity with builtin components.

Cannot create and delete an entity.

Entity's type:
T##(EntityHandleName)

`ecs_new(world, 0)`

To obtain an unused empty entity handle.

`ecs_new(world, Position)`

To obtain an unused entity handle and specify its component or type in it.

`ecs_entity_t e = ecs_new_w_count(world, Position, 100);`

To obtain consecutive entity handles. The first of the obtained entity handles is returned.

`ecs_delete(world, e);`

empty entity.

`ecs_new_child(world, parent_entity, 0)`

create a new empty entity which also specifies a parent entity.

`ecs_adopt(world, e, parent_entity);`

a container entity adopts an entity. 
It adds the EcsContainer component(tag) to the parent if it didn't have it already. 

`ecs_orphan(world, e, my_root);`

to orphan entities from containers.


Component
=========
`ecs_add(world, 10, Position)`

add multiple components to an entity

`ecs_remove`

remove multiple components from an entity

`ecs_set(ecs_world_t *world, ecs_entity_t entity, ecs_entity_t component)`

set component value on individual entity

APIs
====

`Position *p_base = ecs_get_ptr(world, my_base, Position);`

Get the C point of the component.

main()
======
- importing modules
- creation of entities

