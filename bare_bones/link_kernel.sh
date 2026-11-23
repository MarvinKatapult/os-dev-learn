#!/bin/bash
BIN=coolos

i686-elf-gcc -T linker.ld -o $BIN -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
if grub-file --is-x86-multiboot $BIN; then
    echo "Valid Multiboot header"
else
    echo "Error: Invalid Multiboot header!"
fi
