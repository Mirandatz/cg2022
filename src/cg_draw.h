#pragma once

#include <math.h>

#include "cg_color.h"
#include "cg_device.h"
#include "cg_universe.h"

void cg_draw_viewport(CG_ViewPort port, CG_ColorId color) {
    cg_validate_viewport(port);

    // top and bot
    for (size_t x = port.lower_left.x;
         x < port.upper_right.x;
         x++) {
        cg_device_set(port.device, x, port.upper_right.y, color);
        cg_device_set(port.device, x, port.lower_left.y, color);
    }

    // left and right
    for (size_t y = port.lower_left.y;
         y < port.upper_right.y;
         y++) {
        cg_device_set(port.device, port.lower_left.x, y, color);
        cg_device_set(port.device, port.upper_right.x, y, color);
    }
}

CG_DPoint cg_npoint2d_to_dpoint(CG_Point2D npt, CG_ViewPort port) {
    cg_validate_normalized_point2d(npt);
    cg_validate_viewport(port);

    size_t x_scale = port.upper_right.x - port.lower_left.x;
    size_t y_scale = port.upper_right.y - port.lower_left.y;

    return (CG_DPoint){
        .x = (size_t)(port.lower_left.x + (npt.x * x_scale)),
        .y = (size_t)(port.lower_left.y + (npt.y * y_scale))};
}

void _cg_sort_dpoints_by_x(CG_DPoint *a, CG_DPoint *b) {
    if (a->x > b->x) {
        CG_DPoint tmp;
        memcpy(&tmp, a, sizeof(CG_DPoint));
        memcpy(a, b, sizeof(CG_DPoint));
        memcpy(b, &tmp, sizeof(CG_DPoint));
    }
}

void cg_draw_vertical_segment(CG_DPoint pt0, CG_DPoint pt1, CG_ColorId color_id, CG_ViewPort port) {
    cg_validate_viewport(port);
    assert(pt0.x == pt1.x);

    size_t x = pt0.x;

    size_t bot;
    size_t top;
    if (pt0.y < pt1.y) {
        bot = pt0.y;
        top = pt1.y;
    } else {
        bot = pt1.y;
        top = pt0.y;
    }

    for (size_t y = bot; y <= top; y++)
        cg_device_set(port.device, x, y, color_id);
}

// Bresenham http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void cg_draw_line_segment(CG_DPoint pt0, CG_DPoint pt1, CG_ColorId color_id, CG_ViewPort port) {
    cg_validate_viewport(port);
    assert(cg_dpoint_in_viewport(pt0, port));
    assert(cg_dpoint_in_viewport(pt1, port));

    if (pt0.x == pt1.x) {
        cg_draw_vertical_segment(pt0, pt1, color_id, port);
        return;
    }

    int x0 = pt0.x;
    int y0 = pt0.y;

    int x1 = pt1.x;
    int y1 = pt1.y;

    int delta_x = abs(x1 - x0);

    int x_inc = x0 < x1 ? 1 : -1;
    int delta_y = abs(y1 - y0);
    int y_inc = y0 < y1 ? 1 : -1;

    int error = (delta_x > delta_y ? delta_x : -delta_y) / 2;

    while (true) {
        cg_device_set(port.device, x0, y0, color_id);

        if (x0 == x1 && y0 == y1)
            break;

        int previous_error = error;

        if (previous_error > -delta_x) {
            error -= delta_y;
            x0 += x_inc;
        }

        if (previous_error < delta_y) {
            error += delta_x;
            y0 += y_inc;
        }
    }
}

/* Desenha um polígono.
 * Pontos precisam estar normalizados.
 * Não conecta último ponto ao primeiro.
 */
void cg_draw_polygon(CG_ListNode *normalized_polygon,
                     CG_ColorId color_id,
                     CG_ViewPort port) {
    cg_validate_normalized_polygon(normalized_polygon);
    cg_validate_viewport(port);

    CG_ListNode *prev = normalized_polygon;
    CG_ListNode *current = prev->next;

    while (current != NULL) {
        CG_Point2D pt0 = cg_get_point2d(prev);
        CG_Point2D pt1 = cg_get_point2d(current);

        cg_draw_line_segment(
            cg_npoint2d_to_dpoint(pt0, port),
            cg_npoint2d_to_dpoint(pt1, port),
            color_id,
            port);

        prev = current;
        current = current->next;
    }
}
