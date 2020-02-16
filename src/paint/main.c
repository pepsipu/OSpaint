#define SELECT_OFFSET 15

#include <lib/mem/memcpy.c>

uint32_t double_buffer[WIDTH][HEIGHT];
uint8_t *back_buffer;

uint8_t color_index = 0;
uint32_t palette[] = {
        0xffffff,
        0x0,
        0xff0000,
        0x00ff00,
        0x0000ff,
        0xffff00
};

void movement(char c);

struct cursor {
    uint16_t x;
    uint16_t y;
    uint16_t size;
} cursor = {
        .x = 0,
        .y = 0,
        .size = 4
};

volatile uint8_t running = 1;
volatile uint8_t update = 1;
volatile uint8_t cmd = 0;

volatile char cmd_buffer[256];

void paint_handler(char c);

void draw_color_select() {
    for (int i = 1; i < 4; i++) {
        for (int k = 0; k < 2; k++) {
            uint16_t x = 240 / 8 * ((k % 2) + 1) * 2;
            draw_line(x, i * 60 + SELECT_OFFSET, x + 40, i * 60 + SELECT_OFFSET, 0x444444, 1);
        }

    }
    uint16_t x = 240 / 8 * ((color_index % 2) + 1) * 2;
    if (color_index == 0 || color_index == 1) {
        draw_line(x, 1 * 60 + SELECT_OFFSET, x + 40, 60 + SELECT_OFFSET, 0xffffff, 1);
    } else if (color_index == 2 || color_index == 3) {
        draw_line(x, 2 * 60 + SELECT_OFFSET, x + 40, 2 * 60 + SELECT_OFFSET, 0xffffff, 1);
    } else if (color_index == 4 || color_index == 5) {
        draw_line(x, 3 * 60 + SELECT_OFFSET, x + 40, 3 * 60 + SELECT_OFFSET, 0xffffff, 1);
    }
}

void draw_cursor() {
    draw_rectangle_width_buff(cursor.x + 239, cursor.y, cursor.size, cursor.size, palette[color_index], back_buffer);
}

void scale_handler(char c) {
    switch (c) {
        case 'w':
            if (cursor.x + cursor.size + 1 < WIDTH && cursor.y + cursor.size + 1 < HEIGHT - 23) {
                cursor.size++;
            }
            break;
        case 's':
            if (cursor.size != 0) {
                cursor.size--;
            }
            break;
        case '\n':
            key_down_handler = paint_handler;
            break;
    }
    update = 1;
}

struct cursor line_point = {
        .x = 0,
        .y = 0,
        .size = 0,
}; // size will be used to identify if the point has been laid down or not
struct cursor line_point2 = {
        .x = 0,
        .y = 0,
        .size = 0
};;
struct cursor prev = {
        .x = 0,
        .y = 0,
        .size = 0
};;
struct cursor prev2 = {
        .x = 0,
        .y = 0,
        .size = 0
};

void triangle_handler(char c) {
    movement(c);
    if (!line_point.size) {
        if (c == '\n') {
            line_point.size = 1;
            line_point.x = cursor.x;
            line_point.y = cursor.y;
        }
    } else if (!line_point2.size) {
        if (c == '\n') {
            line_point2.size = 1;
            line_point2.x = cursor.x;
            line_point2.y = cursor.y;
        }
    } else {
        if (c == '\n') {
            line_point.size = 0;
            line_point2.size = 0;
            prev.size = 0;
            key_down_handler = paint_handler;
        }
        if (prev.size) {
            draw_line(prev.x, prev.y, line_point.x + 239, line_point.y, 0, 1); // clear
            draw_line(prev.x, prev.y, line_point2.x + 239, line_point2.y, 0, 1); // clear
        }
        prev = (struct cursor) {
                .x = cursor.x + 239,
                .y = cursor.y,
                .size = 1
        };
        draw_line(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, palette[color_index], 1);
        draw_line(cursor.x + 239, cursor.y, line_point2.x + 239, line_point2.y, palette[color_index], 1);
        draw_line(line_point.x + 239, line_point.y, line_point2.x + 239, line_point2.y, palette[color_index], 1);
    }
    update = 1;
}

void rect_handler(char c) {
    movement(c);
    if (!line_point.size) {
        if (c == '\n') {
            line_point.size = 1;
            line_point.x = cursor.x;
            line_point.y = cursor.y;
        }
    } else {
        if (c == '\n') {
            line_point.size = 0;
            prev.size = 0;
            key_down_handler = paint_handler;
        }
        if (prev.size) {
            draw_rectangle(prev.x, prev.y, line_point.x + 239, line_point.y, 0); // clear
        }
        prev = (struct cursor) {
                .x = cursor.x + 239,
                .y = cursor.y,
                .size = 1
        };
        draw_rectangle(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, palette[color_index]);
    }
    update = 1;
}

void line_handler(char c) {
    movement(c);
    if (!line_point.size) {
        if (c == '\n') {
            line_point.size = 1;
            line_point.x = cursor.x;
            line_point.y = cursor.y;
        }
    } else {
        if (c == '\n') {
            line_point.size = 0;
            prev.size = 0;
            key_down_handler = paint_handler;
        }
        if (prev.size) {
            draw_line(prev.x, prev.y, line_point.x + 239, line_point.y, 0, 1); // clear
        }
        prev = (struct cursor) {
                .x = cursor.x + 239,
                .y = cursor.y,
                .size = 1
        };
        draw_line(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, palette[color_index], 1);
    }
    update = 1;
}

void td_handler(char c) {
    movement(c);
    if (!line_point.size) {
        if (c == '\n') {
            line_point.size = 1;
            line_point.x = cursor.x;
            line_point.y = cursor.y;
        }
    } else {
        if (c == '\n') {
            line_point.size = 0;
            prev.size = 0;
            key_down_handler = paint_handler;
        }
        draw_line(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, palette[color_index], 1);
    }
    update = 1;
}

uint16_t scale = 1;
void circle_handler(char c) {
    draw_circle(cursor.x + 239, cursor.y, scale + 1, 0);
    switch (c) {
        case 'w':
            scale++;
            break;
        case 's':
            scale--;
            break;
        case '\n':
            key_down_handler = paint_handler;
    }
    draw_circle(cursor.x + 239, cursor.y, scale, palette[color_index]);
    update = 1;
}

void exec() {
    if (string_cmp(cmd_buffer, "scale")) {
        key_down_handler = scale_handler;
    } else if (string_cmp(cmd_buffer, "line")) {
        key_down_handler = line_handler;
    } else if (string_cmp(cmd_buffer, "3d")) {
        key_down_handler = td_handler;
    } else if (string_cmp(cmd_buffer, "triangle")) {
        key_down_handler = triangle_handler;
    } else if (string_cmp(cmd_buffer, "fill")) {
        flood_fill(cursor.x + 239, cursor.y, fetch_pixel(cursor.x + 239, cursor.y), palette[color_index]);
    } else if (string_cmp(cmd_buffer, "rectangle")) {
        key_down_handler = rect_handler;
    } else if (string_cmp(cmd_buffer, "circle")) {
        key_down_handler = circle_handler;
    }
}

void movement(char c) {
    switch (c) {
        case 'w':
            if (cursor.y - cursor.size > 0) {
                cursor.y -= cursor.size;
            }
            break;
        case 's':
            if (cursor.y + 2 * cursor.size < HEIGHT - 31) {
                cursor.y += cursor.size;
            }
            break;
        case 'a':
            if (cursor.x - cursor.size > 0) {
                cursor.x -= cursor.size;
            }
            break;
        case 'd':
            if (cursor.x + 2 * cursor.size < WIDTH - 239) {
                cursor.x += cursor.size;
            }
            break;
        case 'z':
            draw_rectangle_width(cursor.x + 239, cursor.y, cursor.size, cursor.size, palette[color_index]);
    }
}

void paint_handler(char c) {
    if (cmd) {
        if (cmd > 30) {
            c = '\n';
        }
        if (c != 0) {
            if (c == '\n') {
                exec();
                memset(cmd_buffer, 0, 256);
                cmd = 0;
            } else if (c == '\b') {
                cmd--;
                cmd_buffer[cmd - 1] = 0;
            } else {
                cmd_buffer[cmd - 1] = c;
                cmd++;
            }
        }
    } else {
        movement(c);
        switch (c) {
            case ';':
                cmd = 1;
                break;
            case '1':
                color_index = 0;
                break;
            case '2':
                color_index = 1;
                break;
            case '3':
                color_index = 2;
                break;
            case '4':
                color_index = 3;
                break;
            case '5':
                color_index = 4;
                break;
            case '6':
                color_index = 5;
                break;
            case 'z':
                break;
        }
    }
    update = 1;
}

int paint_main() {
    back_buffer = frame_buffer;
    frame_buffer = (uint8_t *) double_buffer;
    memset(cmd_buffer, 0, 256);
    clear();
    key_down_handler = paint_handler;
    while (running) {
        while (!update) {}
        update = 0;
        draw_desktop();
        draw_color_select();
        draw_rectangle_width(241, HEIGHT - 23, WIDTH - 241, 23, 0);
        if (cmd) {
            draw_char(';', 240, HEIGHT - 23, 0xffffff);
            draw_string(cmd_buffer, 248, HEIGHT - 23, 0xffffff);
        }
        memcpy(back_buffer, double_buffer, WIDTH * HEIGHT * 3);
        draw_cursor();
    }
}