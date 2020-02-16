#include <cpu/io.c>

char thing = 'a';
__attribute__((interrupt)) void key(void *isframe) {
    inb(0x60);
    *((char *) 0xb8000) = thing;
    thing++;
    outb(0x20,0x20);
}