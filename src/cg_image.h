#pragma once

#include <X11/Xlib.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef union __attribute__((scalar_storage_order("big-endian"))) {
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
    uint32_t argb;
} cg_color;

void cg_print_color(cg_color color) {
    printf("a=%u, r=%u, g=%u, b=%u, argb=0x%08X\n", color.a, color.r, color.g, color.b, color.argb);
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

void cg_pixmap_set(cg_pixmap pixmap, size_t y, size_t x, cg_color c) {
    assert(pixmap.data != NULL);
    assert(y < pixmap.height);
    assert(x < pixmap.width);

    pixmap.data[(y * pixmap.width) + x] = c;
}

cg_color cg_pixmap_get(cg_pixmap pixmap, size_t y, size_t x) {
    assert(pixmap.data != NULL);
    assert(y < pixmap.height);
    assert(x < pixmap.width);

    return pixmap.data[(y * pixmap.width) + x];
}

void cg_show_pixmap(cg_pixmap pmp) {
    Display *display = XOpenDisplay(NULL);
    assert(display != NULL);

    Window window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        0,          /* window x */
        0,          /* window y*/
        pmp.width,  /* width */
        pmp.height, /*height */
        0,          /* border width*/
        0x0000000,  /* border color */
        0xFFFFFFFF  /*background color */
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XWindowAttributes window_attr = {0};
    XGetWindowAttributes(display, window, &window_attr);

    XImage *image = XCreateImage(
        display,
        window_attr.visual,
        window_attr.depth,
        ZPixmap,                     /* format */
        0,                           /* offset */
        (char *)pmp.data,            /* data */
        pmp.width,                   /* width */
        pmp.height,                  /* height */
        32,                          /* bitmap_pad */
        pmp.width * sizeof(cg_color) /* bytes_per_line */
    );

    GC gc = XCreateGC(
        display,
        window,
        0 /* valuemask */,
        NULL /*XGCValues values */
    );

    XEvent event;
    while (true) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            XPutImage(
                display,
                window,
                gc,
                image,
                0,         /* src_x */
                0,         /* src_y */
                0,         /* dest_x */
                0,         /* dest_y */
                pmp.width, /* width */
                pmp.height /* height */
            );
        }

        if (event.type == KeyPress)
            break;
    }

    XFreeGC(display, gc);
    free(image);
    XCloseDisplay(display);
}