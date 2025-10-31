#pragma once

#include <assert.h>
#include <stdint.h>

#define IDT_NUM_ENTRIES 256

typedef struct idt_descriptor_64
{
  uint16_t offset_low;
  uint16_t segment_selector;
  uint8_t reserved_ist_offset; // bits 0 to 2 are IST offset, rest are reserved
  uint8_t attribute_type; // bits 7 to 5 are attributes, 4 is reserved, 3 to 0 is type
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t reserved;
} __attribute__((packed)) idt_descriptor_64_t;

_Static_assert(sizeof(idt_descriptor_64_t) == 16, "idt_descriptor_64_t should be 16 bytes");

typedef struct idt64
{
  idt_descriptor_64_t entries[IDT_NUM_ENTRIES];
} __attribute__((packed)) idt64_t;

_Static_assert(sizeof(idt64_t) == IDT_NUM_ENTRIES * sizeof(idt_descriptor_64_t),
               "idt64_t should be 256 idt_descriptor_64_t");

typedef struct idt64_ptr
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) idt64_ptr_t;

_Static_assert(sizeof(idt64_ptr_t) == 10, "idt64_ptr_t should be 10 bytes");

