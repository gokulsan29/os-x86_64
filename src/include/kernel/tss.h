#pragma once

#include <assert.h>
#include <stdint.h>

typedef struct tss_long_mode
{
  uint32_t reserved0; // reserved, must be 0
  uint64_t rsp0; // ring 0 stack pointer
  uint64_t rsp1; // ring 1 stack pointer
  uint64_t rsp2; // ring 2 stack pointer
  uint64_t reserved1; // reserved, must be 0
  uint64_t ist1; // interrupt stack table, used for interrupts like double fault
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t reserved2;
  uint16_t reserved3;
  uint16_t io_map_base;
} __attribute__((packed)) tss_long_mode_t;

_Static_assert(sizeof(tss_long_mode_t) == 104, "tss_long_mode_t should be of 104 bytes");

