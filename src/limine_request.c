#include <stdbool.h>
#include <stddef.h>

#include "limine.h"

// Set the base revision to 4, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

struct limine_framebuffer_response const*
get_framebuffer_response()
{
  return framebuffer_request.response;
}

bool
check_limine_requests()
{
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    return false;
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL
   || framebuffer_request.response->framebuffer_count < 1) {
    return false;
  }
  return true;
}

