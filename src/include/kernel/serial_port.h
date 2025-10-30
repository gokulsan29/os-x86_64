#pragma once

#include <stdint.h>

typedef struct serial_port
{
  uint16_t addr;
} serial_port_t;

serial_port_t serial_port_init(uint16_t port);
void serial_putchar(serial_port_t port, char c);

