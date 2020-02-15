flags equ 1 << 0 | 1 << 1
magic equ 0x1badb002 ; signature
checksum equ -(magic + flags)

global _start:function (_start.end - _start)
extern kmain

section .multiboot
    align 4
    dd magic
    dd flags
    dd checksum
section .bss
    align 16
    resb 1024
    _esp: ; stack will grow down towards the end of the reserved bytes (4kb)
section .text
    _start:
        mov esp, _esp
        call kmain
        cli
        .hang:
        hlt
        jmp .hang
        .end: