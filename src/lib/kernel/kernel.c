#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kernel/debug.h"
#include "limine.h"
#include "limine/limine_request.h"

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm ("hlt");
  }
}

// Entry point of the kernel
void kernel_main(void) {
  // debug must be initialized first
  if (!debug_init()) {
    hcf();
  }

  if (!check_limine_requests()) {
    hcf();
  }

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer = get_framebuffer_response()->framebuffers[0];
  debug_printf("framebuffer width: %lld height: %lld bpp: %lld", framebuffer->width, framebuffer->height, framebuffer->bpp);

  // Note: we assume the framebuffer model is RGB with 32-bit pixels.
  for (size_t i = 0; i < 100; i++) {
    volatile uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
  }

  // We're done, just hang...
  hcf();
}

