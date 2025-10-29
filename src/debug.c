#include <stdbool.h>

#include "common.h"
#include "debug.h"
#include "serial_port.h"

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
debug_printstring(char const* str)
{
  while(*str) {
    debug_putchar(*str);
    str++;
  }
}

