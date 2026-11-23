#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross compiler you dummy!"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

/* Hardware text mode color constants. */
typedef enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} VgaColor;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
uint16_t * terminal_buffer = (uint16_t*)VGA_MEMORY;

static inline uint8_t _vga_entry_color(VgaColor fg, VgaColor bg)
{
    return fg | bg << 4;
} /* _vga_print_color */

static inline uint16_t _vga_entry(uint8_t uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
} /* _vga_entry */

static inline uint32_t _count_digits(uint32_t n)
{
    int32_t ret = 0;
    while (n > 0) {
        ret++;
        n /= 10;
    }
    return ret;
}

void terminal_init(void)
{
    terminal_row = 0;
    terminal_col = 0;
    terminal_color = _vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = _vga_entry(' ', terminal_color);
        }
    }
} /* terminal_init */

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
} /* terminal_setcolor */

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = _vga_entry(c, color);
} /* terminal_putentryat */

void terminal_putchar(char c) 
{
    if (c == '\n') {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_col, terminal_row);

    if (++terminal_col == VGA_WIDTH) {
        terminal_col = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
} /* terminal_putchar */

void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
} /* terminal_write */

void terminal_write_reverse(const char* data, size_t size) 
{
    if (size == 0) return;
    for (size_t i = size - 1;; i--) {
        terminal_putchar(data[i]);
        if (i == 0) break;
    }
} /* terminal_write */

size_t strlen(const char * data)
{
    size_t c = 0;
    while (*data) { c++; data++; }
    return c;
} /* strlen */

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
} /* terminal_writestring */

void terminal_writestring_reverse(const char* data)
{
    terminal_write_reverse(data, strlen(data));
} /* terminal_writestring */

void terminal_writeuint32(uint32_t n)
{
#define MAX_DIGITS_FOR_32BIT_NUMBER 10
    char buf[MAX_DIGITS_FOR_32BIT_NUMBER + 1] = { '0' };
    for (uint32_t i = 0; i < sizeof(buf) - 1 && n > 0; i++, n /= 10) {
        buf[i] = (n % 10) + '0';
    }
    terminal_writestring_reverse(buf);
} /* terminal_writeuint32 */

uint32_t fib(int32_t n)
{
    if (n < 2) return 1;
    return fib(n - 2) + fib(n - 1);
} /* fib */

void kernel_main(void) 
{
    /* Initialize terminal interface */
    terminal_init();

    for (int i = 0; i < 30; i++) {
        terminal_writeuint32(fib(i));
        terminal_writestring("\n");
    }
} /* kernel_main */
