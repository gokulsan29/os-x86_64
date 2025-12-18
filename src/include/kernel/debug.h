#pragma once

#include <stdbool.h>

#define DEBUG_PORT 0x3f8

bool debug_init();
void debug_putchar(char c);
void debug_printf(char const* fmt, ...);

