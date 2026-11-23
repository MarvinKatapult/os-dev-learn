#!/bin/bash

BIN=coolos

# For a iso start
# qemu-system-i386 -cdrom $BIN.iso

# Directly start the binary
qemu-system-i386 -kernel $BIN
