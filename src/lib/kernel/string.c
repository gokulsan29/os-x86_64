#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict dest, void const* restrict src, size_t n)
{
  uint8_t* restrict pdest = (uint8_t* restrict)dest;
  uint8_t const* restrict psrc = (uint8_t const* restrict)src;

  for (size_t i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void* memset(void* s, int c, size_t n)
{
  uint8_t* p = (uint8_t*)s;

  for (size_t i = 0; i < n; i++) {
    p[i] = (uint8_t)c;
  }

  return s;
}

void* memmove(void* dest, void const* src, size_t n)
{
  uint8_t* pdest = (uint8_t*)dest;
  uint8_t const* psrc = (uint8_t const*)src;

  if (src > dest) {
    for (size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  }
  else if (src < dest) {
    for (size_t i = n; i > 0; i--) {
      pdest[i-1] = psrc[i-1];
    }
  }

  return dest;
}

int memcmp(void const* s1, void const* s2, size_t n)
{
  uint8_t const* p1 = (uint8_t const*)s1;
  uint8_t const* p2 = (uint8_t const*)s2;

  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

