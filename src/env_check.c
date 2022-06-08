#include <stdbool.h>

#include "cg_image.h"

int main() {
    cg_pixmap pmp;
    cg_pixmap_init(
        &pmp,
        200, // height
        720  // width
    );

    cg_color c = {.a = 255, .r = 159, .g = 43, .b = 104};
    for (size_t y = 0; y < pmp.height; y++) {
        for (size_t x = 0; x < pmp.width; x++) {
            cg_pixmap_set(pmp, y, x, c);
        }
    }
    cg_show_pixmap(pmp);

    cg_pixmap_destroy(&pmp);
}