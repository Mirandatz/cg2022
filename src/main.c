#include <stdbool.h>

#include "cg_image.h"

int main() {
    cg_pixmap pmp;
    cg_pixmap_init(
        &pmp,
        200, // height
        720  // width
    );

    cg_color red = {.a = 255, .r = 255, .g = 0, .b = 0};
    for (size_t y = 0; y < pmp.height; y++) {
        for (size_t x = 0; x < pmp.width; x++) {
            cg_pixmap_set(pmp, y, x, red);
        }
    }
    cg_show_pixmap(pmp);

    cg_color blue = {.a = 30, .r = 0, .g = 0, .b = 255};
    for (size_t y = 0; y < pmp.height; y++) {
        for (size_t x = 0; x < pmp.width; x++) {
            cg_pixmap_set(pmp, y, x, blue);
        }
    }
    cg_show_pixmap(pmp);

    cg_pixmap_destroy(&pmp);
}