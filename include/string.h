#pragma once

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict dest, void const* restrict src, size_t n);
void* memset(void* s, int c, size_t n);
void* memmove(void* dest, void const* src, size_t n);
int memcmp(void const* s1, void const* s2, size_t n);

