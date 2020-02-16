#include <cpu/io.c>

__attribute__((interrupt)) void clock(void *isframe) {
    outb(0x20,0x20);
}