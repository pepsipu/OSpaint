#ifndef TEXT
#define TEXT

#include <ui/font_bitmap.h>
#include <ui/primitives.c>
#include <glob.h>
#include <lib/str.c>

void draw_char(char c, uint16_t x, uint16_t y, uint32_t color) {
    size_t font_off = c * 16;
    for (uint32_t ny = 0; ny < 16; ny++) {
        for (uint32_t nx = 0; nx < 8; nx++) {
            if (__font_bitmap__[font_off + ny] & (1 << (8 - nx))) {
                plot_pixel(x + nx, y + ny, color);
            }
        }
    }
}

void draw_string(char *s, uint16_t x, uint16_t y, uint32_t color) {
    for (int i = 0; i < string_len(s); i++) {
        draw_char(s[i], x, y, color);
        x += 8;
    }
}

#endif