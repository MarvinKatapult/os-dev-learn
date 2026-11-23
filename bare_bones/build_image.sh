#!/bin/bash
BIN=coolos

rm -rf isodir/

mkdir -p isodir/boot/grub
cp coolos isodir/boot/coolos
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o coolos.iso isodir
