#include "sdl_private.h"

static EcsColor WHITE = {255, 255, 255, 255};

static
void Vec2ToGfx(
    EcsVec2 *v,
    Sint16 *x_vec_out,
    Sint16 *y_vec_out,
    uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i ++) {
        x_vec_out[i] = v[i].x;
        y_vec_out[i] = v[i].y;
    }
}

static
void RenderRectangle(
    SdlWindow *wnd,
    EcsMatTransform2D *m,
    float width,
    float height,
    EcsColor *c,
    EcsLineColor *lc)
{
    float w = width / 2.0;
    float h = height / 2.0;

    EcsVec2 points[5];
    points[0] = (EcsVec2){-w, -h};
    points[1] = (EcsVec2){w, -h};
    points[2] = (EcsVec2){w, h};
    points[3] = (EcsVec2){-w, h};
    points[4] = (EcsVec2){-w, -h};

    ecs_mat3x3_transform(m, points, points, 5);
    ecs_mat3x3_transform(&wnd->projection, points, points, 5);

    Sint16 gfx_x[5];
    Sint16 gfx_y[5];
    Vec2ToGfx(points, gfx_x, gfx_y, 5);

    if (!c && !lc) {
        c = &WHITE;
    }

    if (c) {
        filledPolygonRGBA(wnd->display, gfx_x, gfx_y, 5, c->r, c->g, c->b, c->a);
    }

    if (lc) {
        polygonRGBA(wnd->display, gfx_x, gfx_y, 5, lc->r, lc->g, lc->b, lc->a);
    }
}

void SdlRenderRectangle(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsRectangle *shape = ecs_field(rows, EcsRectangle, 1, i);
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsColor *c = ecs_field(rows, EcsColor, 3, i);
        EcsLineColor *lc = ecs_field(rows, EcsLineColor, 4, i);
        RenderRectangle(wnd, m, shape->width, shape->height, c, lc);
    }
}

void SdlRenderSquare(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsSquare *shape = ecs_field(rows, EcsSquare, 1, i);
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsColor *c = ecs_field(rows, EcsColor, 3, i);
        EcsLineColor *lc = ecs_field(rows, EcsLineColor, 4, i);
        RenderRectangle(wnd, m, shape->size, shape->size, c, lc);  
    }
}

void SdlRenderPolygon8(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsPolygon8 *shape = ecs_field(rows, EcsPolygon8, 1, i);
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsColor *c = ecs_field(rows, EcsColor, 3, i);
        EcsLineColor *lc = ecs_field(rows, EcsLineColor, 4, i);
        
        uint8_t point_count = shape->point_count;
        ecs_assert(point_count <= 8, ECS_INVALID_PARAMETER, NULL);
        //EcsPoint2D points[point_count];
		EcsPoint2D* points = malloc(point_count * sizeof(points[0]));

        ecs_mat3x3_transform(m, shape->points, points, point_count);
        ecs_mat3x3_transform(&wnd->projection, points, points, point_count);

        //Sint16 gfx_x[point_count];
        //Sint16 gfx_y[point_count];
		Sint16* gfx_x = malloc(point_count * sizeof(gfx_x[0]));
		Sint16* gfx_y = malloc(point_count * sizeof(gfx_y[0]));
        Vec2ToGfx(points, gfx_x, gfx_y, point_count);

        if (!c && !lc) {
            c = &WHITE;
        }

        if (c) {
            filledPolygonRGBA(wnd->display, gfx_x, gfx_y, point_count, c->r, c->g, c->b, c->a);
        }

        if (lc) {
            polygonRGBA(wnd->display, gfx_x, gfx_y, point_count, lc->r, lc->g, lc->b, lc->a);
        }

		free(points);
		free(gfx_x);
		free(gfx_y);
    }
}

void SdlRenderCircle(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsCircle *shape = ecs_field(rows, EcsCircle, 1, i);
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsColor *c = ecs_field(rows, EcsColor, 3, i);
        EcsLineColor *lc = ecs_field(rows, EcsLineColor, 4, i);

        float radius = shape->radius;

        EcsVec2 position = (EcsVec2){0, 0};
        ecs_mat3x3_transform(m, &position, &position, 1);

        /* Transform position & size to screen coordinates */
        EcsVec2 coord[] = {
            {position.x, position.y},
            {radius, radius}
        };

        ecs_mat3x3_transform(&wnd->projection, coord, coord, 2);

        if (!c && !lc) {
            c = &WHITE;
        }

        if (c) {
            filledEllipseRGBA(wnd->display,
                coord[0].x, coord[0].y, /* position */
                radius * wnd->scale.x, radius * wnd->scale.y, /* size */
                c->r, c->g, c->b, c->a  /* color */
            );
        }

        if (lc) {
            SDL_SetRenderDrawBlendMode(wnd->display, (lc->a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(wnd->display, lc->r, lc->g, lc->b, lc->a);

            int steps = radius * wnd->scale.y;
            if (steps > 75) {
                steps = 75;
            }

            SDL_Point *points = alloca(sizeof(SDL_Point) * (steps + 1));
            float step_size = (2.0 * M_PI) / steps;
            int w;
            for (w = 0; w < 2; w ++) {
                int i;
                float x_radius = radius * wnd->scale.x;
                float y_radius = radius * wnd->scale.y;

                for (i = 0; i <= steps; i ++) {
                    points[i].x = cos(step_size * i) * x_radius + coord[0].x;
                    points[i].y = sin(step_size * i) * y_radius + coord[0].y;                
                }
                
                SDL_RenderDrawLines(wnd->display, points, steps + 1);          
                radius -= 0.5 / wnd->scale.y;
            }
        }
    }
}

void SdlRenderDot(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsColor *c = ecs_field(rows, EcsColor, 3, i);

        if (!c) {
            c = &WHITE;
        }

        EcsVec2 position = (EcsVec2){0, 0};
        ecs_mat3x3_transform(m, &position, &position, 1);
        ecs_mat3x3_transform(&wnd->projection, &position, &position, 1);

        SDL_SetRenderDrawColor(wnd->display, c->r, c->g, c->b, c->a);

        SDL_RenderDrawLine(wnd->display, position.x - 1, position.y - 1, position.x + 1, position.y - 1);
        SDL_RenderDrawLine(wnd->display, position.x - 1, position.y + 1, position.x + 1, position.y + 1);
        SDL_RenderDrawLine(wnd->display, position.x - 1, position.y, position.x + 1, position.y);   
    }
}

void SdlRenderLine(ecs_rows_t *rows) {
    SdlWindow *wnd = rows->param;

    int i;
    for (i = 0; i < rows->count; i ++) {
        EcsLine *shape = ecs_field(rows, EcsLine, 1, i);
        EcsMatTransform2D *m = ecs_field(rows, EcsMatTransform2D, 2, i);
        EcsLineColor *c = ecs_field(rows, EcsLineColor, 3, i);

        if (!c) {
            c = &WHITE;
        }

        EcsVec2 position[2] = {
            {shape->start.x, shape->start.y},
            {shape->stop.x, shape->stop.y}
        };

        ecs_mat3x3_transform(m, position, position, 2);
        ecs_mat3x3_transform(&wnd->projection, position, position, 2);

        SDL_SetRenderDrawColor(wnd->display, c->r, c->g, c->b, c->a);
        SDL_RenderDrawLine(wnd->display, position[0].x, position[0].y, position[1].x, position[1].y);          
    }
}

void SdlRender2D(ecs_rows_t *rows) {
    ecs_world_t *world = rows->world;
    SdlWindow *wnd = ecs_column(rows, SdlWindow, 1);
    ecs_entity_t SdlRenderSquare = ecs_column_entity(rows, 2);
    ecs_entity_t SdlRenderRectangle = ecs_column_entity(rows, 3);
    ecs_entity_t SdlRenderCircle = ecs_column_entity(rows, 4);
    ecs_entity_t SdlRenderPolygon8 = ecs_column_entity(rows, 5);
    ecs_entity_t SdlRenderDot = ecs_column_entity(rows, 6);
    ecs_entity_t SdlRenderLine = ecs_column_entity(rows, 7);

    int i;
    for (i = 0; i < rows->count; i ++) {
        SDL_SetRenderDrawColor(wnd[i].display, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(wnd[i].display);

        ecs_run(world, SdlRenderDot, 0, wnd);
        ecs_run(world, SdlRenderLine, 0, wnd);
        ecs_run(world, SdlRenderSquare, 0, wnd);
        ecs_run(world, SdlRenderCircle, 0, wnd);
        ecs_run(world, SdlRenderRectangle, 0, wnd);
        ecs_run(world, SdlRenderPolygon8, 0, wnd);

        SDL_RenderPresent(wnd[i].display);
    }
}
