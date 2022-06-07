#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef union __attribute__((scalar_storage_order("big-endian"))) {
    struct {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    uint32_t argb;
} cg_color;

void cg_print_color(cg_color color) {
    printf("a=%u, r=%u, g=%u, b=%u, argb=0x%08x\n", color.a, color.r, color.g, color.b, color.argb);
}

typedef struct {
    size_t height;
    size_t width;
    cg_color *data;
} cg_pixmap;

void cg_pixmap_init(cg_pixmap *pixmap, size_t height, size_t width) {
    assert(pixmap != NULL);
    assert(height > 0);
    assert(width > 0);

    pixmap->height = height;
    pixmap->width = width;
    pixmap->data = calloc(height * width, sizeof(cg_color));
    assert(pixmap->data != NULL);
}

void cg_pixmap_destroy(cg_pixmap *pixmap) {
    assert(pixmap != NULL);
    assert(pixmap->data != NULL);

    free(pixmap->data);
    pixmap->height = 0;
    pixmap->width = 0;
    pixmap->data = NULL;
}

void cg_pixmap_set(cg_pixmap pixmap, size_t x, size_t y, cg_color c) {
    assert(pixmap.data != NULL);
    assert(y < pixmap.height);
    assert(x < pixmap.width);

    pixmap.data[(y * pixmap.width) + x] = c;
}

cg_color cg_pixmap_get(cg_pixmap pixmap, size_t x, size_t y) {
    assert(pixmap.data != NULL);
    assert(y < pixmap.height);
    assert(x < pixmap.width);

    return pixmap.data[(y * pixmap.width) + x];
}