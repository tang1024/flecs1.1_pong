#include <flecs_components_graphics.h>
#include <string.h>

void FlecsComponentsGraphicsImport(
    ecs_world_t *world,
    int flags)
{
    bool do_2d = !flags || flags & ECS_2D;

    ECS_MODULE(world, FlecsComponentsGraphics);

    ECS_COMPONENT(world, EcsColor);
    ECS_COMPONENT(world, EcsLineColor);
    ECS_COMPONENT(world, EcsLineWidth);
    ECS_SET_COMPONENT(EcsColor);
    ECS_SET_COMPONENT(EcsLineColor);
    ECS_SET_COMPONENT(EcsLineWidth);

    if (do_2d) {
        ECS_COMPONENT(world, EcsCanvas2D);
        ECS_SET_COMPONENT(EcsCanvas2D);

        ECS_COMPONENT(world, EcsCamera2D);
        ECS_SET_COMPONENT(EcsCamera2D);
    }
}
