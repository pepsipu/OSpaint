FLAGS equ 1 << 0 | 1 << 1 | 1 << 2
MAGIC equ 0x1badb002 ; signature
CHECKSUM equ -(MAGIC + FLAGS)
VIDMODE equ 0
WIDTH equ 1280
HEIGHT equ 720
DEPTH equ 24

global _start:function (_start.end - _start)
extern kmain

section .multiboot
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd VIDMODE
    dd WIDTH
    dd HEIGHT
    dd DEPTH
section .bss
    align 16
    resb 8192
    _esp: ; stack will grow down towards the end of the reserved bytes
section .text
    _start:
        mov esp, _esp
        push ebx
        cli
        lgdt [gdt_descriptor]
        call kmain
        cli
        .hang:
        mov byte [0xb8000], ':'
        mov byte [0xb8002], '('
        hlt
        jmp .hang
        .end:
    gdt:
        ; gdt null descriptor
        ; describes a fake segment that is all null bytes
        ; seems to be a requirement for intel CPUs? likely to catch errors when setting the segment for far jumps
        dq 0x0 ; define 8 null bytes, as required
        .data_segment:
        ; gdt data segment descriptor, for 32 bit protected mode, extremely similar to code segment with a change of type descriptor
        ; size = f f f f f, base = 00 00 00 00
        ; flags 1 = 1 (present in memory) 00 (ring permission 0) 1 (code/data)
        ; flags 2 = 1 (multiply size by 4,000) 1 (32 bit) 0 (no 64 bit) 0 (avl bit, does nothing to hardware)
        ; type descriptor = 0 (data) 0 (protect from lower rings from executing segment) 1 (readable) 0 (lock bit / is being accessed)
        dw 0xffff ; the first 4 nibbles of the size of the segment: [f f f f] f
        dw 0x0 ; the lowest 2 bytes of the base address: 00 00 [00 00]
        db 0x0 ; the third lowest byte of the base address: 00 [00] 00 00
        db 10010010b ; first 4 bits is flags 1, last 4 bits is type descriptor
        db 11001111b ; first 4 bits is flags 2, last 4 bits is the last
        db 0x0
       .code_segment:
       ; gdt code segment descriptor, for 32 bit protected mode
       ; size = f f f f f, base = 00 00 00 00
       ; flags 1 = 1 (present in memory) 00 (ring permission 0) 1 (code/data)
       ; flags 2 = 1 (multiply size by 4,000) 1 (32 bit) 0 (no 64 bit) 0 (avl bit, does nothing to hardware)
       ; type descriptor = 1 (code) 0 (protect from lower rings from executing segment) 1 (readable) 0 (lock bit / is being accessed)
       dw 0xffff ; the first 4 nibbles of the size of the segment: [f f f f] f
       dw 0x0 ; the lowest 2 bytes of the base address: 00 00 [00 00]
       db 0x0 ; the third lowest byte of the base address: 00 [00] 00 00
       db 10011010b ; first 4 bits is flags 1, last 4 bits is type descriptor
       db 11001111b ; first 4 bits is flags 2, last 4 bits is the last nibble of the size of the segment: f f f f [f]
       db 0x0 ; set highest byte of base address to 0: [00] 00 00 00
    gdt_descriptor:
        dw gdt_descriptor - gdt - 1
        dd gdt