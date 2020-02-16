#ifndef PRIM
#define PRIM

uint32_t *frame_buffer = (uint32_t *) 0xa0000;

void plot_pixel(uint16_t x, uint16_t y, uint32_t color) {
    *(frame_buffer + y * (1280) + x) = color;
}

#endif