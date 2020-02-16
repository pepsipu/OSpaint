#include <cpu/idt.c>
#include <cpu/pic.c>
#include <ui/primitives.c>
#include <multiboot/multiboot.h>
#include <ui/desktop.c>
#include <ui/bmp/bmp.c>
#include <multiboot/modules.c>
#include <isr/keyboard.c>
#include <ui/text.c>
#include <paint/main.c>

struct multiboot_info *mb_info;
volatile int pressed = 0;

void remove_splash(char _) { pressed = 1; }

void kmain(struct multiboot_info *_mb_info) {
    mb_info = _mb_info;
    remap_pic();
    init_idt();
    struct module *bmp = _mb_info->mods_addr;
    frame_buffer = (uint8_t *) mb_info->framebuffer_addr;
    key_down_handler = remove_splash;
    draw_bmp((uint8_t *) bmp->start, 0, 0);
    while (!pressed);
    paint_main();
}

