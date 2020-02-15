OUTDIR=build

mkdirs:
	mkdir -p ${OUTDIR}/bins
	mkdir -p ${OUTDIR}/objs
	mkdir -p ${OUTDIR}/iso/boot/grub
iso:
	cp src/grub.cfg ${OUTDIR}/iso/boot/grub
	cp ${OUTDIR}/bins/ospaint.bin ${OUTDIR}/iso/boot
	grub-mkrescue -o ${OUTDIR}/ospaint.iso ${OUTDIR}/iso
boot:
	nasm -felf32 src/asm/boot.asm -o ${OUTDIR}/objs/boot.o
kernel:
	gcc -c src/kmain.c -o ${OUTDIR}/objs/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32
bin:
	gcc -T src/linker.ld -o ${OUTDIR}/bins/ospaint.bin -ffreestanding -O2 -nostdlib ${OUTDIR}/objs/* -lgcc -m32

all: mkdirs boot kernel bin iso