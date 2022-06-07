#include <stdio.h>
#include <stdlib.h>

#include "cg_image.h"
#include <X11/Xlib.h>

int main() {
    cg_pixmap pixmap;
    cg_pixmap_init(&pixmap, 1920, 1080);

    cg_color red = {
        .a = 255,
        .r = 255,
        .g = 0,
        .b = 0,
    };

    cg_pixmap_set(pixmap, 0, 0, red);
    cg_print_color(cg_pixmap_get(pixmap, 0, 0));

    cg_pixmap_destroy(&pixmap);
    Display *display = XOpenDisplay(NULL);

    int window_x = 0;
    int window_y = 0;
    unsigned int window_width = 1280;
    unsigned int window_height = 720;
    unsigned int border_width = 10;
    unsigned long border_color = 0xFF0000FF;
    unsigned long background_color = 0x00FF00FF;

    Window window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        window_x,
        window_y,
        window_width,
        window_height,
        border_width,
        border_color,
        background_color);

    XMapWindow(display, window);

    long int eventMask = StructureNotifyMask;
    XSelectInput(display, window, eventMask);

    XEvent event;
    do {
        XNextEvent(display, &event); // calls XFlush
    } while (event.type != MapNotify);

    GC gc = XCreateGC(display, window,
                      0,     // mask of values
                      NULL); // array of values
    XSetForeground(display, gc, background_color);

    XDrawLine(display, window, gc, 10, 10, 190, 190); // from-to
    XDrawLine(display, window, gc, 10, 190, 190, 10); // from-to

    eventMask = ButtonPressMask | ButtonReleaseMask;
    XSelectInput(display, window, eventMask); // override prev

    do {
        XNextEvent(display, &event); // calls XFlush()
    } while (event.type != ButtonRelease);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}