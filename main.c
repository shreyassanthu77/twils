#include "str/string.h"
#include "tailwind/tailwind.h"
#include <stddef.h>
#define FS_IMPLEMENTATION
#include "fs.h"
#include <stdio.h>

// KHASH_MAP_INIT_STR(str, uint32_t);

int main() {
  const string js_path = str("./lib.js");
  const string css_path = str("./min.css");
  tailwind_t *tw = tailwind_init(js_path);
  tailwind_load_css(tw, css_path);
  tailwind_class_order_t result;
  for (size_t i = 0; i < 10000; i++) {
    result = tailwind_get_class_order(tw, str("p-4"), str("lkasjdf"));
  }
  printf("Result: %ld %ld\n", result.a, result.b);

  tailwind_free(tw);
  return 0;
}
