#ifndef PRIM
#define PRIM

#define WIDTH 1280
#define HEIGHT 720

uint8_t *frame_buffer = (uint8_t *) 0xa0000;

int abs(int x) {
    return x < 0 ? -x : x ;
}

void plot_pixel(uint16_t x, uint16_t y, uint32_t color) {
    uint8_t *new_ptr = frame_buffer + x * 3 + y * WIDTH * 3;
    *new_ptr = *(uint8_t *) &color;
    *(new_ptr + 1) = *((uint8_t *) &color + 1);
    *(new_ptr + 2) = *((uint8_t *) &color + 2);
}

uint32_t fetch_pixel(uint16_t x, uint16_t y) {
    return *(frame_buffer + y * (1280) + x * 3);
}

void single_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    for(;;){
        plot_pixel(x0,y0, color);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint16_t thickness) {
    for (int i = 0; i < thickness; i++) single_line(x0 + i, y0, x1 + i, y1, color);
}

void triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint16_t thickness) {
    draw_line(x0, y0, x1, y1, color, thickness);
    draw_line(x1, y1, x2, y2, color, thickness);
    draw_line(x2, y2, x0, y0, color, thickness);
}

void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {

}

void draw_rectangle_width(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int k = 0; k < height; k++) {
            plot_pixel(x + i, y + k, color);
        }
    }
}

void clear() {
    memset(frame_buffer, 0, HEIGHT * WIDTH * 3);
}

#endif