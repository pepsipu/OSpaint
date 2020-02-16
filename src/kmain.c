#include <cpu/idt.c>
#include <cpu/pic.c>
#include <ui/primitives.c>
#include <multiboot/multiboot.h>

struct multiboot_info *mb_info;

void kmain(struct multiboot_info *_mb_info) {
    mb_info = _mb_info;
    remap_pic();
    init_idt();
    frame_buffer = (uint8_t *) mb_info->framebuffer_addr;
    draw_rectangle_width(600, 400, 20, 20, 0xffffff);
    while (1) {

    }
}

