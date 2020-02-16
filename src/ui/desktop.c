#include <ui/primitives.c>

uint32_t default_colors[3][2] = {
        {0xffffff, 0x0},
        {0xff0000, 0x00ff00},
        {0x0000ff, 0xffff00}
};

void draw_icons() {
    for (int i = 0; i < 3; i++) {
        for (int k = 1; k < 3; k++) {
            draw_rectangle_width(240 / 8 * k * 2, i * 60 + 30, 40, 40, 0xffffff);
            draw_rectangle_width(240 / 8 * k * 2 + 2, i * 60 + 32, 38, 38, default_colors[i][k - 1]);
        }
    }
}

void draw_desktop() {
    draw_line(240, 0, 240, HEIGHT, 0xffffff, 1);
    draw_line(0, 230, 240, 230, 0xffffff, 1);
    draw_line(240, HEIGHT - 30, WIDTH - 1, HEIGHT - 30, 0xffffff, 1);
    draw_icons();
}

