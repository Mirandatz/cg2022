#include "cg_device.h"
#include "cg_draw.h"
#include "cg_linalg.h"
#include "cg_list.h"
#include "cg_universe.h"

void exemplo1(void) {
    CG_ListNode *polygon1 = cg_create_list();
    cg_add_point2d(polygon1, (CG_Point2D){.x = -9, .y = -8});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -7, .y = -3});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -4, .y = -4});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -3, .y = -6});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -6, .y = -9});

    CG_ListNode *polygon2 = cg_create_list();
    cg_add_point2d(polygon2, (CG_Point2D){.x = -6, .y = -2});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -1, .y = -2});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -1, .y = -6});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -6, .y = -6});

    CG_Window window = {.lower_left = {.x = -10.0, .y = -10.0},
                        .upper_right = {.x = 0.0, .y = 0.0}};

    CG_ListNode *normalized_polygon1 = cg_normalize_polygon_naive(polygon1, window);
    CG_ListNode *normalized_polygon2 = cg_normalize_polygon_naive(polygon2, window);

    CG_Device *dev = cg_create_device(
        640,  // columns
        480); // rows

    CG_ViewPort viewport = {
        .lower_left = {0, 0},
        .upper_right = {dev->num_columns - 1, dev->num_rows - 1},
        .device = dev};

    CG_Palette *plt = cg_create_palette(
        (CG_Color[]){
            {.r = 0, .g = 0, .b = 0},
            {.r = 1, .g = 0, .b = 0},
            {.r = 0, .g = 1, .b = 0},
            {.r = 0, .g = 0, .b = 1},
            {.r = 0.623, .g = .168, .b = .407}},
        5);

    cg_draw_polygon(normalized_polygon1, (CG_ColorId)1, viewport);
    cg_draw_polygon(normalized_polygon2, (CG_ColorId)2, viewport);

    // yeey
    cg_device_show(dev, plt);

    // cleanup
    cg_destroy_list(polygon1);
    cg_destroy_list(polygon2);
    cg_destroy_list(normalized_polygon1);
    cg_destroy_list(normalized_polygon2);
    cg_destroy_device(dev);
    cg_destroy_palette(plt);
}

void exemplo2(void) {
    CG_ListNode *polygon1 = cg_create_list();
    cg_add_point2d(polygon1, (CG_Point2D){.x = -9, .y = -8});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -7, .y = -3});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -4, .y = -4});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -3, .y = -6});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -6, .y = -9});

    CG_ListNode *polygon2 = cg_create_list();
    cg_add_point2d(polygon2, (CG_Point2D){.x = -6, .y = -2});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -1, .y = -2});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -1, .y = -6});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -6, .y = -6});

    CG_Window window = {.lower_left = {.x = -10.0, .y = -10.0},
                        .upper_right = {.x = 0.0, .y = 0.0}};

    CG_ListNode *normalized_polygon1 = cg_normalize_polygon_naive(polygon1, window);
    CG_ListNode *normalized_polygon2 = cg_normalize_polygon_naive(polygon2, window);

    CG_Device *dev = cg_create_device(
        640,  // columns
        480); // rows

    CG_ViewPort port1 = {
        .lower_left = {30, 60},
        .upper_right = {347, 220},
        .device = dev};

    CG_ViewPort port2 = {
        .lower_left = {370, 260},
        .upper_right = {503, 407},
        .device = dev};

    CG_Palette *plt = cg_create_palette(
        (CG_Color[]){
            {.r = 0, .g = 0, .b = 0},
            {.r = 1, .g = 0, .b = 0},
            {.r = 0, .g = 1, .b = 0},
            {.r = 0, .g = 0, .b = 1},
            {.r = 0.623, .g = .168, .b = .407}},
        5);

    cg_draw_polygon(normalized_polygon1, (CG_ColorId)1, port1);
    cg_draw_polygon(normalized_polygon2, (CG_ColorId)2, port2);

    cg_draw_viewport(port1, (CG_ColorId)4);
    cg_draw_viewport(port2, (CG_ColorId)4);

    // yeey
    cg_device_show(dev, plt);

    // cleanup
    cg_destroy_list(polygon1);
    cg_destroy_list(polygon2);
    cg_destroy_list(normalized_polygon1);
    cg_destroy_list(normalized_polygon2);
    cg_destroy_device(dev);
    cg_destroy_palette(plt);
}

void atividade_1(void) {
    // exercicio 1
    CG_Palette *palette = cg_create_palette(
        (CG_Color[]){
            {.r = 0, .g = 0, .b = 0},  // preto
            {.r = 1, .g = 0, .b = 0},  // vermelho
            {.r = 0, .g = 1, .b = 0},  // verde
            {.r = 0, .g = 0, .b = 1},  // azul
            {.r = 1, .g = 1, .b = 1}}, // branco
        5);

    // exercicio 2
    CG_Device *device = cg_create_device(
        800, // columns
        600  // lines
    );

    // exercicio 3
    CG_ListNode *polygon1 = cg_create_list();
    cg_add_point2d(polygon1, (CG_Point2D){.x = -2.0, .y = +6.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = +2.0, .y = +6.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = +8.0, .y = -8.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = +4.0, .y = -8.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = +2.0, .y = -4.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -2.0, .y = -4.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -4.0, .y = -8.0});
    cg_add_point2d(polygon1, (CG_Point2D){.x = -8.0, .y = -8.0});

    CG_ListNode *polygon2 = cg_create_list();
    cg_add_point2d(polygon2, (CG_Point2D){.x = +1.7, .y = -2.0});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -1.7, .y = -2.0});
    cg_add_point2d(polygon2, (CG_Point2D){.x = -0.3, .y = 2.0});
    cg_add_point2d(polygon2, (CG_Point2D){.x = +0.3, .y = 2.0});

    // exercicio 4
    CG_Window window1 = {
        .lower_left = (CG_Point2D){.x = -14.0, .y = -14.0},
        .upper_right = (CG_Point2D){.x = 14.0, .y = 14.0}};

    // exericio 5.1
    CG_ViewPort port1 = {
        .device = device,
        .lower_left = (CG_DPoint){.x = 0, .y = 0},
        .upper_right = (CG_DPoint){.x = 400 - 1, .y = 300 - 1}};

    // exercicio 6
    {
        cg_draw_viewport(port1, (CG_ColorId)1);

        CG_ListNode *tmp = cg_normalize_polygon_naive(polygon1, window1);
        cg_draw_polygon(tmp, (CG_ColorId)1, port1);
        cg_destroy_list(tmp);

        tmp = cg_normalize_polygon_naive(polygon2, window1);
        cg_draw_polygon(tmp, (CG_ColorId)1, port1);
        cg_destroy_list(tmp);
    }

    // exercicio 7
    {
        CG_ListNode *copy1 = cg_copy_polygon(polygon1);
        CG_ListNode *copy2 = cg_copy_polygon(polygon2);

        CG_Matrix3 trans = cg_making_2d_shearing(
            0.5,
            0.0);

        cg_transform_polygon(trans, copy1);
        cg_transform_polygon(trans, copy2);

        CG_ListNode *norm1 = cg_normalize_polygon_naive(copy1, window1);
        CG_ListNode *norm2 = cg_normalize_polygon_naive(copy2, window1);

        CG_ViewPort port2 = {
            .device = device,
            .lower_left = {.x = 400 - 1, .y = 0},
            .upper_right = {.x = 800 - 1, .y = 300 - 1}};

        cg_draw_polygon(norm1, (CG_ColorId)2, port2);
        cg_draw_polygon(norm2, (CG_ColorId)2, port2);
        cg_draw_viewport(port2, (CG_ColorId)2);

        cg_destroy_list(norm1);
        cg_destroy_list(norm2);
        cg_destroy_list(copy2);
        cg_destroy_list(copy1);
    }

    // exercicio 8
    {
        CG_ListNode *copy1 = cg_copy_polygon(polygon1);
        CG_ListNode *copy2 = cg_copy_polygon(polygon2);

        // compute tranformation
        CG_Point2D guesstimated_center = cg_guesstimate_polygon_center(polygon1);
        CG_Matrix3 translate_to_origin = cg_make_2d_translation(
            -guesstimated_center.x,
            -guesstimated_center.y);
        CG_Matrix3 rotate = cg_make_ccw_rotation(-90);
        CG_Matrix3 translate_back = cg_make_2d_translation(
            guesstimated_center.x,
            guesstimated_center.y);
        CG_Matrix3 composed_transformation = cg_mul_m3m3(rotate, translate_to_origin);
        composed_transformation = cg_mul_m3m3(translate_back, composed_transformation);

        cg_transform_polygon(composed_transformation, copy1);
        cg_transform_polygon(composed_transformation, copy2);

        CG_ListNode *norm1 = cg_normalize_polygon_naive(copy1, window1);
        CG_ListNode *norm2 = cg_normalize_polygon_naive(copy2, window1);

        CG_ViewPort port3 = {
            .device = device,
            .lower_left = {.x = 0, .y = 300 - 1},
            .upper_right = {.x = 400 - 1, .y = 600 - 1}};

        cg_draw_polygon(norm1, (CG_ColorId)3, port3);
        cg_draw_polygon(norm2, (CG_ColorId)3, port3);
        cg_draw_viewport(port3, (CG_ColorId)3);

        cg_destroy_list(norm1);
        cg_destroy_list(norm2);
        cg_destroy_list(copy2);
        cg_destroy_list(copy1);
    }

    // exercicio 9
    {
        CG_ListNode *copy1 = cg_copy_polygon(polygon1);
        CG_ListNode *copy2 = cg_copy_polygon(polygon2);

        // compute tranformation
        CG_Matrix3 scaling = cg_make_2d_scaling(0.5, 0.5);
        cg_transform_polygon(scaling, copy1);
        cg_transform_polygon(scaling, copy2);

        CG_Window window4 = {
            .lower_left = {-7, -7},
            .upper_right = {7, 7}};

        CG_ListNode *norm1 = cg_normalize_polygon_naive(copy1, window4);
        CG_ListNode *norm2 = cg_normalize_polygon_naive(copy2, window4);

        CG_ViewPort port3 = {
            .device = device,
            .lower_left = {.x = 400 - 1, .y = 300 - 1},
            .upper_right = {.x = 800 - 1, .y = 600 - 1}};

        cg_draw_polygon(norm1, (CG_ColorId)4, port3);
        cg_draw_polygon(norm2, (CG_ColorId)4, port3);
        cg_draw_viewport(port3, (CG_ColorId)4);

        cg_destroy_list(norm1);
        cg_destroy_list(norm2);
        cg_destroy_list(copy2);
        cg_destroy_list(copy1);
    }

    cg_device_show(device, palette);

    cg_destroy_list(polygon2);
    cg_destroy_list(polygon1);
    cg_destroy_device(device);
    cg_destroy_palette(palette);
}

int main(void) {
    atividade_1();
    return 0;
}
