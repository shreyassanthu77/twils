#include "tailwind/tailwind.h"
#include <stdio.h>

int main() {
  tailwind_t *tw = tailwind_init(str("path/to/tailwindcss"));
  uint64_t result = tailwind_get_class_order(tw, str("class-name"));
  printf("Result: %llu\n", result);

  tailwind_free(tw);
  return 0;
}
