#include <stdbool.h>

#include "cg_color.h"
#include "cg_device.h"

int main() {
    CG_Device *device = cg_create_device(
        640, // columns
        480  // rows
    );

    CG_Palette *palette = cg_create_palette(
        (CG_Color[]){
            {.r = 0.623, .g = .168, .b = .407}},
        1);

    cg_device_show(device, palette);

    cg_destroy_palette(palette);
    cg_destroy_device(device);
}
