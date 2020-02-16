#define SELECT_OFFSET 15


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

struct cursor prev_cursor = {
        .x = 0,
        .y = 0,
        .size = 0
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
    draw_rectangle_width(prev_cursor.x, prev_cursor.y, prev_cursor.size, prev_cursor.size, 0);
    draw_rectangle_width(cursor.x + 239, cursor.y, cursor.size, cursor.size, palette[color_index]);
    prev_cursor = (struct cursor) {
            .x = cursor.x + 239,
            .y = cursor.y,
            .size = cursor.size
    };
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

struct cursor line_point; // size will be used to identify if the point has been laid down or not
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
            key_down_handler = paint_handler;
        }
        draw_rectangle(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, 0); // clear
        draw_line(cursor.x + 239, cursor.y, line_point.x + 239, line_point.y, palette[color_index], 1);
    }
    update = 1;
}

void exec() {
    if (string_cmp(cmd_buffer, "scale")) {
        key_down_handler = scale_handler;
    } else if (string_cmp(cmd_buffer, "line")) {
        key_down_handler = line_handler;
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
    memset(cmd_buffer, 0, 256);
    clear();
    key_down_handler = paint_handler;
    while (running) {
        while (!update) {}
        update = 0;
        draw_desktop();
        draw_cursor();
        draw_color_select();
        draw_rectangle_width(241, HEIGHT - 23, WIDTH - 241, 23, 0);
        if (cmd) {
            draw_char(';', 240, HEIGHT - 23, 0xffffff);
            draw_string(cmd_buffer, 248, HEIGHT - 23, 0xffffff);
        }


    }
}