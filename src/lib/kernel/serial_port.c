#include <stdbool.h>
#include <stdint.h>

#include "kernel/serial_port.h"

static inline uint8_t
inb(uint16_t port)
{
  uint8_t ret;
  asm volatile(
    "in %1, %0\n"
    : "=a"(ret)
    : "Nd"(port)
    :
  );
  return ret;
}

static inline void
outb(uint16_t port, uint8_t val)
{
  asm volatile(
    "out %0, %1\n"
    :
    : "a"(val), "Nd"(port)
    :
  );
}

serial_port_t
serial_port_init(uint16_t port)
{
  // Taken from https://wiki.osdev.org/Serial_Ports#Initialization
  outb(port + 1, 0x00); // Disable all interrupts
  outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(port + 1, 0x00); //                  (hi byte)
  outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(port + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(port + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if(inb(port + 0) != 0xAE) {
    return (serial_port_t){.addr = 0};
  }

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(port + 4, 0x0F);

  return (serial_port_t){.addr = port};
}

bool is_transmit_empty(serial_port_t port)
{
  return ((inb(port.addr + 5) & 0x20) != 0);
}

void
serial_putchar(serial_port_t port, char c)
{
  while (!is_transmit_empty(port)); // Wait till transmit buffer is empty
  outb(port.addr, c);
}

