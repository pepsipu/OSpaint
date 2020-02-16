#include <stdint.h>
#include <isr/clock.c>
#include <lib/mem/memset.c>
#include <isr/double_fault.c>
#include <isr/keyboard.c>
#include <cpu/io.c>

struct {
    uint16_t size;
    uint32_t addr;
} __attribute__((packed)) idt_ptr;

struct idt_entry {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset_2;
} __attribute__((packed)) idt[256];

void register_isr(void *isr, uint8_t index) {
    idt[index] = (struct idt_entry) {
            .offset_1 = (uint16_t) ((uint32_t) isr & 0xffff),
            .offset_2 = (uint16_t) ((uint32_t) isr >> 16) & 0xffff,
            .selector = 16,
            .zero = 0,
            .type = 0x8e
    };
}

void init_idt() {
    idt_ptr.size = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.addr = (uint32_t) idt;
    memset(idt, 0, sizeof(idt));
    register_isr(clock, 32);
    register_isr(double_fault, 8);
    register_isr(key, 33);
    asm volatile ("lidt %0" : : "m"(idt_ptr));
    asm volatile ("sti");
};
