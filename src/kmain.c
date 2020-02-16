#include <cpu/idt.c>
#include <cpu/pic.c>
#include <ui/primitives.c>
#include <multiboot/multiboot.c>

void kmain(struct multiboot_info *mb_info) {
    remap_pic();
    init_idt();
    while (1) {

    }
}

