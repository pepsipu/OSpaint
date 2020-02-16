#include <cpu/idt.c>
#include <cpu/pic.c>
#include <vesa/vesa.c>

void kmain() {
    remap_pic();
    init_idt();
    while (1) {

    }
}

