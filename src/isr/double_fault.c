__attribute__((interrupt)) void double_fault(void *isframe) {
    asm volatile ("cli");
    while (1) {
        asm volatile ("hlt");
    }
}