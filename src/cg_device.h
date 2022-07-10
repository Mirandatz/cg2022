#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cg_color.h"

typedef struct CG_Device {
    size_t num_columns;
    size_t num_rows;
    CG_ColorId *buffer;
} CG_Device;

typedef struct CG_DPoint {
    size_t x;
    size_t y;
} CG_DPoint;

typedef struct CG_ViewPort {
    CG_DPoint lower_left;
    CG_DPoint upper_right;
    CG_Device *device;
} CG_ViewPort;

CG_Device *cg_create_device(size_t num_columns, size_t num_rows) {
    assert(num_columns > 0);
    assert(num_rows > 0);

    CG_Device *dev = calloc(1, sizeof(CG_Device));
    dev->num_columns = num_columns;
    dev->num_rows = num_rows;
    dev->buffer = calloc(num_columns * num_rows, sizeof(CG_ColorId));
    return dev;
}

void cg_validate_device(CG_Device *dev) {
    assert(dev != NULL);
    assert(dev->num_rows > 0);
    assert(dev->num_columns > 0);
    assert(dev->buffer != NULL);
}

void cg_device_set(CG_Device *dev, size_t x, size_t y, CG_ColorId c) {
    cg_validate_device(dev);
    assert(x < dev->num_columns);
    assert(y < dev->num_rows);

    dev->buffer[(y * dev->num_columns) + x] = c;
}

CG_ColorId cg_device_get(CG_Device *dev, size_t x, size_t y) {
    cg_validate_device(dev);
    assert(x < dev->num_columns);
    assert(y < dev->num_rows);

    return dev->buffer[(y * dev->num_columns) + x];
}

void cg_destroy_device(CG_Device *dev) {
    cg_validate_device(dev);

    free(dev->buffer);
    dev->buffer = NULL;

    dev->num_rows = 0;
    dev->num_columns = 0;

    free(dev);
}

/* Essa função converte o buffer da nossa "representação de dispositivo"
 * para o formato esperado pelo X11.
 * A conversão leva em conta que a origem do nosso dispostivo é
 * no canto inferior esquerdo da tela e a do X11 é no canto superior esquerdo,
 * garantindo que a imagem é "desenhada como esperado".
 */
char *_cg_dev_buffer_to_xbuffer(CG_Device *dev, CG_Palette *plt) {
    cg_validate_device(dev);
    cg_validate_palette(plt);

    // copy device data to a buffer, changing the origin
    // from top-left top bottom-left
    size_t pixel_count = dev->num_columns * dev->num_rows;
    CG_DColor *buffer = calloc(pixel_count, sizeof(CG_DColor));

    for (size_t y = 0; y < dev->num_rows; y++) {
        for (size_t x = 0; x < dev->num_columns; x++) {
            CG_ColorId color_id = cg_device_get(dev, x, y);
            cg_validate_color_id(color_id, plt);
            CG_DColor devicecolor = cg_palette_map(color_id, plt);

            buffer[((dev->num_rows - y - 1) * dev->num_columns) + x] = devicecolor;
        }
    }

    return (char *)buffer;
}

/* Essa função faz o ritual necessário, conversando com o X11,
 * para criar uma janela e desenhar nosso device (parâmetro `dev`) nela,
 * usando nossa paleta (parâmetro `plt`).
 * Pressionar qualquer botão fecha a janela e libera os recursos alocados
 * por esta função.
 */
void cg_device_show(CG_Device *dev, CG_Palette *plt) {
    cg_validate_device(dev);
    cg_validate_palette(plt);

    Display *display = XOpenDisplay(NULL);
    assert(display != NULL);

    Window window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        0,                /* window x */
        0,                /* window y*/
        dev->num_columns, /* num_columns */
        dev->num_rows,    /*num_rows */
        0,                /* border num_columns*/
        0x0000000,        /* border color */
        0xFFFFFFFF        /*background color */
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XWindowAttributes window_attr = {0};
    XGetWindowAttributes(display, window, &window_attr);

    char *x11buffer = _cg_dev_buffer_to_xbuffer(dev, plt);
    XImage *image = XCreateImage(
        display,
        window_attr.visual,
        window_attr.depth,
        ZPixmap, /* format */
        0,       /* offset */
        x11buffer,
        dev->num_columns,                    /* num_columns */
        dev->num_rows,                       /* num_rows */
        32,                                  /* bitmap_pad */
        dev->num_columns * sizeof(CG_DColor) /* bytes_per_line */
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
                0,                /* src_x */
                0,                /* src_y */
                0,                /* dest_x */
                0,                /* dest_y */
                dev->num_columns, /* num_columns */
                dev->num_rows     /* num_rows */
            );
        }

        if (event.type == KeyPress)
            break;
    }

    XFreeGC(display, gc);
    XDestroyImage(image);
    XCloseDisplay(display);
}

bool cg_dpoint_is_in_device(CG_DPoint pt, CG_Device *dev) {
    return pt.x < dev->num_columns && pt.y < dev->num_rows;
}

void cg_validate_viewport(CG_ViewPort port) {
    cg_validate_device(port.device);
    assert(cg_dpoint_is_in_device(port.lower_left, port.device));
    assert(cg_dpoint_is_in_device(port.upper_right, port.device));
    assert(port.lower_left.x < port.upper_right.x);
    assert(port.lower_left.y < port.upper_right.y);
}

bool cg_dpoint_in_viewport(CG_DPoint pt, CG_ViewPort port) {
    cg_validate_viewport(port);

    return port.lower_left.x <= pt.x && pt.x <= port.upper_right.x && port.lower_left.y <= pt.y && pt.y <= port.upper_right.y;
}
