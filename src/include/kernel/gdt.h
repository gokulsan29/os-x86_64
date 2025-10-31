#pragma once

#include <assert.h>
#include <stdint.h>

typedef struct gdt_descriptor
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access_byte;
  uint8_t flags_limit_high; // bits 7 to 4 are flags, rest are limit high
  uint8_t base_high;
} __attribute__((packed)) gdt_descriptor_t;

_Static_assert(sizeof(gdt_descriptor_t) == 8, "gdt_descriptor_t should be of 8 bytes");

typedef struct gdt_long_descriptor
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid1;
  uint8_t access_byte;
  uint8_t flags_limit_high; // bits 7 to 4 are flags, rest are limit high
  uint8_t base_mid2;
  uint32_t base_high;
  uint32_t reserved;
} __attribute__((packed)) gdt_long_descriptor_t;

_Static_assert(sizeof(gdt_long_descriptor_t) == 16, "gdt_long_descriptor_t should be of 16 bytes");

typedef struct gdt64
{
  gdt_descriptor_t null_descriptor;
  gdt_descriptor_t kernel_code_segment;
  gdt_descriptor_t kernel_data_segment;
  gdt_descriptor_t user_code_segment;
  gdt_descriptor_t user_data_segment;
  gdt_long_descriptor_t task_state_segment;
} __attribute__((packed)) gdt64_t;

_Static_assert(sizeof(gdt64_t) == 5 * sizeof(gdt_descriptor_t) + sizeof(gdt_long_descriptor_t),
               "gdt64_t should have 5 gdt_descriptor_t and 1 gdt_long_descriptor_t");

typedef struct gdt64_ptr
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt64_ptr_t;

_Static_assert(sizeof(gdt64_ptr_t) == 10, "gdt64_ptr_t should be 10 bytes");

