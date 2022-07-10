#pragma once

#include <assert.h>
#include <string.h>

#include "cg_linalg.h"
#include "cg_list.h"

const size_t POLYGON_MIN_POINT_COUNT = 3;

typedef struct CG_Point2D {
    float x;
    float y;
} CG_Point2D;

typedef struct CG_Window {
    CG_Point2D lower_left;
    CG_Point2D upper_right;
} CG_Window;

void cg_add_point2d(CG_ListNode *list, CG_Point2D pt) {
    assert(list != NULL);
    cg_list_add(list, &pt, sizeof(pt));
}

CG_Point2D cg_get_point2d(CG_ListNode *list) {
    assert(list != NULL);
    assert(list->data != NULL);

    CG_Point2D *data = list->data;
    return *data;
}

/* Cria um novo poligono, alocando memória e copiando cada ponto.
 */
CG_ListNode *cg_copy_polygon(CG_ListNode *polygon) {
    assert(polygon != NULL);

    CG_ListNode *new_polygon = cg_create_list();

    CG_ListNode *old_iterator = polygon;
    while (old_iterator != NULL) {
        cg_add_point2d(
            new_polygon,
            cg_get_point2d(old_iterator));

        old_iterator = old_iterator->next;
    }

    return new_polygon;
}

CG_Point2D *cg_get_point2d_ref(CG_ListNode *list) {
    assert(list != NULL);
    assert(list->data != NULL);

    CG_Point2D *data = list->data;
    return data;
}

void cg_validate_window(CG_Window win) {
    assert(win.lower_left.x < win.upper_right.x);
    assert(win.lower_left.y < win.upper_right.y);
}

bool cg_point2d_in_window(CG_Point2D pt, CG_Window win) {
    bool x_inside = (win.lower_left.x <= pt.x && pt.x <= win.upper_right.x);
    bool y_inside = (win.lower_left.y <= pt.y && pt.y <= win.upper_right.y);
    return x_inside && y_inside;
}

/* Normaliza coordenadas.
   Aceita apenas pontos que estão dentro da window.
*/
CG_Point2D cg_normalize_point2d(CG_Point2D pt, CG_Window win) {
    cg_validate_window(win);
    assert(cg_point2d_in_window(pt, win));

    float delta_x = win.upper_right.x - win.lower_left.x;
    float delta_y = win.upper_right.y - win.lower_left.y;

    return (CG_Point2D){
        .x = (pt.x - win.lower_left.x) / delta_x,
        .y = (pt.y - win.lower_left.y) / delta_y};
}

void cg_validate_normalized_point2d(CG_Point2D pt) {
    assert(0 <= pt.x && pt.x <= 1);
    assert(0 <= pt.y && pt.y <= 1);
}

void cg_validate_polygon_point_count(CG_ListNode *polygon) {
    CG_ListNode *node = polygon;
    for (size_t i = 0; i < POLYGON_MIN_POINT_COUNT; i++) {
        assert(node != NULL);
        assert(node->data != NULL);

        node = node->next;
    }
}

/* Normaliza um poligono de forma ingenua, isto é, assumindo que todos pontos
 * pontos estão dentro da janela de visualização.
 */
CG_ListNode *cg_normalize_polygon_naive(CG_ListNode *polygon, CG_Window window) {
    cg_validate_window(window);
    cg_validate_polygon_point_count(polygon);

    CG_ListNode *normalized_poly = cg_create_list();

    CG_ListNode *current_node = polygon;
    while (current_node != NULL) {
        CG_Point2D original_point = cg_get_point2d(current_node);
        CG_Point2D normalized_point = cg_normalize_point2d(original_point, window);
        cg_add_point2d(normalized_poly, normalized_point);

        current_node = current_node->next;
    }

    // conenct last to first
    CG_Point2D original_point = cg_get_point2d(polygon);
    CG_Point2D normalized_point = cg_normalize_point2d(original_point, window);
    cg_add_point2d(normalized_poly, normalized_point);

    return normalized_poly;
}

void cg_validate_normalized_polygon(CG_ListNode *normalized_polygon) {
    cg_validate_polygon_point_count(normalized_polygon);

    CG_ListNode *node = normalized_polygon;
    while (node != NULL) {
        CG_Point2D point = cg_get_point2d(node);
        cg_validate_normalized_point2d(point);

        node = node->next;
    }
}

CG_Vector3 cg_point2d_to_vector(CG_Point2D pt) {
    return (CG_Vector3){
        .v1 = pt.x,
        .v2 = pt.y,
        .v3 = 1}; // homogeneous coordinate
}

CG_Point2D cg_vector_to_point2d(CG_Vector3 vector) {
    assert(vector.v3 != 0.0);

    return (CG_Point2D){
        .x = vector.v1 / vector.v3,
        .y = vector.v2 / vector.v3};
}

/* Transforma (in-place) os pontos de `polygon` com
 * a transformação linear `trans`.
 */
void cg_transform_polygon(CG_Matrix3 trans, CG_ListNode *polygon) {
    assert(polygon != NULL);

    CG_ListNode *node = polygon;
    while (node != NULL) {
        CG_Point2D *old_point_ref = cg_get_point2d_ref(node);

        CG_Vector3 point_as_vector = cg_point2d_to_vector(*old_point_ref);
        CG_Vector3 transformed_vector = cg_mul_m3v3(trans, point_as_vector);
        CG_Point2D new_point = cg_vector_to_point2d(transformed_vector);

        old_point_ref->x = new_point.x;
        old_point_ref->y = new_point.y;

        node = node->next;
    }
}

CG_Point2D cg_guesstimate_polygon_center(CG_ListNode *polygon) {
    assert(polygon != NULL);
    assert(polygon->data != NULL);

    size_t point_count = 0;
    float sum_x = 0;
    float sum_y = 0;

    for (CG_ListNode *node = polygon;
         node != NULL;
         node = node->next) {

        CG_Point2D pt = cg_get_point2d(node);
        sum_x += pt.x;
        sum_y += pt.y;
        point_count += 1;
    }

    float avg_x = sum_x / point_count;
    float avg_y = sum_y / point_count;

    return (CG_Point2D){
        .x = avg_x,
        .y = avg_y};
}
