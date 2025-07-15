# Bare-metal OS – V2 (Raspberry Pi Zero 2 W)

These repositories represent the first and second iterations of a bare-metal operating system for the Raspberry Pi Zero 2 W, built without any underlying operating system or emulation.

The goal was to learn how ARM-based systems boot and communicate with hardware at the lowest level, using Assembly and C++ to interact directly with memory-mapped I/O. This includes manually handling GPIO and UART via PL011 registers.

## What was achieved

- Bootloader written in ARMv8 assembly
- Manual control of GPIO pins (e.g., LED blink tests)
- UART serial output (via USB-UART adapter)
- Real hardware deployment (not emulated)
- Structured Makefile-based build system

These versions laid the foundation for a more robust and modular rewrite using Rust that includes a uart terminal.

## See the latest version

For the latest and most complete version (rewritten in Rust + ASM, with UART input and terminal interface), visit:

👉 [Final version (V3)](https://github.com/UNIX-73/baremetal_v3)
