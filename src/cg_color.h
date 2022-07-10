#pragma once

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

const float MIN_COLOR_INTENSITY = 0.0;
const float MAX_COLOR_INTENSITY = 1.0;
const uint8_t MIN_CHANNEL_VALUE = 0;
const uint8_t MAX_CHANNEL_VALUE = 255;

// Usado para indexar cores em uma paleta
typedef size_t CG_ColorId;

/* Define uma cor em função de intensidades de vermelho, verde e azul.
 * Valores de `r`, `g` e `b` pertencem ao intervalo [0, 1].
 */
typedef struct CG_Color {
    float r;
    float g;
    float b;
} CG_Color;

typedef struct CG_Palette {
    size_t num_colors;
    CG_Color *colors;
} CG_Palette;

/* Representa uma "cor no dispositivo".
 *
 * O trechinho `__attribute__(blabla)`
 * intrui o compilador sobre como os bytes dessa struct devem ser armazenados na memória.
 * Em geral, isso não é particularmente importante.
 * No caso dessa struct especificamente, garantir que a ordem é a mesma que a utilizada
 * pelo X nos permite passar valores "brutos" dela para o X.
 */
typedef struct __attribute__((scalar_storage_order("big-endian"))) CG_DColor {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} CG_DColor;

CG_Palette *cg_create_palette(CG_Color *colors, size_t num_colors) {
    assert(num_colors > 0);

    CG_Palette *plt = calloc(1, sizeof(CG_Palette));
    plt->num_colors = num_colors;
    plt->colors = calloc(num_colors, sizeof(CG_Color));
    memcpy(
        plt->colors,
        colors,
        num_colors * sizeof(CG_Color));
    return plt;
}

void cg_validate_color(CG_Color c) {
    assert(MIN_COLOR_INTENSITY <= c.r && c.r <= MAX_COLOR_INTENSITY);
    assert(MIN_COLOR_INTENSITY <= c.g && c.g <= MAX_COLOR_INTENSITY);
    assert(MIN_COLOR_INTENSITY <= c.b && c.b <= MAX_COLOR_INTENSITY);
}

void cg_validate_color_id(CG_ColorId color, CG_Palette *plt) {
    assert(plt != NULL);
    assert(plt->colors != NULL);
    assert(color < plt->num_colors);
}

void cg_validate_palette(CG_Palette *plt) {
    assert(plt != NULL);
    assert(plt->colors != NULL);
    assert(plt->num_colors > 0);

    for (size_t i = 0; i < plt->num_colors; i++) {
        cg_validate_color(plt->colors[i]);
    }
}

void cg_destroy_palette(CG_Palette *plt) {
    free(plt->colors);
    free(plt);
}

CG_DColor cg_color_to_dcolor(CG_Color c) {
    cg_validate_color(c);

    return (CG_DColor){
        .a = MAX_CHANNEL_VALUE,
        .r = (uint8_t)(MAX_CHANNEL_VALUE * c.r),
        .g = (uint8_t)(MAX_CHANNEL_VALUE * c.g),
        .b = (uint8_t)(MAX_CHANNEL_VALUE * c.b),
    };
}

CG_DColor cg_palette_map(CG_ColorId color_id, CG_Palette *plt) {
    cg_validate_color_id(color_id, plt);

    return cg_color_to_dcolor(plt->colors[color_id]);
}
