#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel/common.h"
#include "kernel/debug.h"
#include "kernel/serial_port.h"

#define HEX_TABLE "0123456789abcdef"

static serial_port_t debug_port;

bool
debug_init()
{
  debug_port = serial_port_init(DEBUG_PORT);
  return debug_port.addr != 0;
}

void
debug_putchar(char c)
{
  serial_putchar(debug_port, c);
}

void
_print_uint(uint64_t value)
{
  uint64_t divisor = 1;
  while (value / divisor > 9) {
    divisor = divisor * 10;
  }
  while (divisor > 0) {
    debug_putchar('0' + value / divisor);
    value = value % divisor;
    divisor = divisor / 10;
  }
}

void
_print_int(int64_t value)
{
  uint64_t magnitude = value;
  if (value < 0) {
    debug_putchar('-');
    magnitude = -magnitude;
  }
  _print_uint(magnitude);
}

void
_print_hex(uint64_t value, size_t size)
{
  for (int i = size * 2 - 1; i >= 0;i--) {
    uint8_t nibble = (value >> (i * 4)) & 0xf;
    debug_putchar(HEX_TABLE[nibble]);
  }
}

void
_print_string(char const* str)
{
  while(*str) {
    debug_putchar(*str);
    str++;
  }
}

void
debug_printf(char const* fmt, ...)
{
  VA_LIST args;
  VA_START(args, fmt);
  while (*fmt) {
    if (*fmt != '%') {
      debug_putchar(*fmt);
    }
    else {
      fmt++;
      bool is_long = false;
      bool is_long_long = false;
      if (*fmt == 'l') {
        fmt++;
        is_long = true;
        if (*fmt == 'l') {
          fmt++;
          is_long_long = true;
        }
      }
      switch (*fmt) {
        case '\0': {
          debug_putchar('%');
          VA_END(args);
          return;
        }
        case '%': {
          debug_putchar('%');
          break;
        }
        case 's': {
          char const* str = VA_ARG(args, char const*);
          _print_string(str);
          break;
        }
        case 'c': {
          // A char is promoted to an int
          char c = VA_ARG(args, int);
          debug_putchar(c);
          break;
        }
        case 'd': {
          int64_t value;
          if (is_long_long) {
            value = VA_ARG(args, long long int);
          }
          else if (is_long) {
            value = VA_ARG(args, long int);
          }
          else {
            value = VA_ARG(args, int);
          }
          _print_int(value);
          break;
        }
        case 'x': {
          uint64_t value;
          size_t size;
          if (is_long_long) {
            value = VA_ARG(args, unsigned long long int);
            size = sizeof(unsigned long long int);
          }
          else if (is_long) {
            value = VA_ARG(args, unsigned long int);
            size = sizeof(unsigned long int);
          }
          else {
            value = VA_ARG(args, unsigned int);
            size = sizeof(unsigned int);
          }
          _print_hex(value, size);
          break;
        }
        case 'p': {
          void* ptr = VA_ARG(args, void*);
          _print_hex((uintptr_t)ptr, sizeof(uintptr_t));
          break;
        }
        default: {
          _print_string("printf error: ");
          debug_putchar(*fmt);
          debug_putchar('\n');
        }
      }
    }
    fmt++;
  }
  VA_END(args);
}

