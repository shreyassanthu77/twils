#include "str/string.h"
#include "tailwind/tailwind.h"
#include <stddef.h>
#include <time.h>
#define FS_IMPLEMENTATION
#include "fs.h"
#include <stdio.h>

// KHASH_MAP_INIT_STR(str, uint32_t);

int main() {
  const string js_path = str("./lib.js");
  const string css_path = str("./min.css");
  tailwind_t *tw = tailwind_init(js_path);
  tailwind_load_css(tw, css_path);

  string result = string_with_capacity(2000);
  time_t start = time(NULL);
  for (int i = 0; i < 100'000; i++) {
    tailwind_get_class_order(
        tw, &result,
        str("bg-blue-500 text-white py-2 px-4 what hover:focus:bg-blue-700"));
  }
  time_t end = time(NULL);
  double elapsed = difftime(end, start);
  printf("Class order: %.*s\n", str_fe(result));

  printf("Elapsed time: %.2fs\n", elapsed);

  string_free(&result);
  tailwind_free(tw);
  return 0;
}
