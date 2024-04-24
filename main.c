#include "sorting/sorting.h"
#include "str/string.h"
#include "tailwind/tailwind.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/time.h>
#define FS_IMPLEMENTATION
#include "fs.h"
#include "timing.h"
#include <stdio.h>

// KHASH_MAP_INIT_STR(str, uint32_t);

const string test_css =
    str("   bg-blue-500 \n \rtext-white py-2 px-4 what "
        "hover:focus:bg-blue-700 "
        "md:text-lg lg:hover:text-red-500 \t \t \r\n focus:outline-none "
        "focus:ring     \t some-random-class "
        "focus:ring-blue-500 focus:ring-opacity-50 shadow-lg       "
        "rounded-lg border       border-transparent bg-gradient-to-r "
        "from-blue-500 to-blue-500");

int main() {
  const string js_path = str("./lib.js");
  const string css_path = str("./min.css");

  tailwind_t *tw = tailwind_init(js_path);
  tailwind_load_css(tw, css_path);

  size_t n;
  string *classes = string_split_whitespace(test_css, &n);
  int64_t *order;

  // for (size_t i = 0; i < n i += 2) {
  //   measure_pref(total, {
  //     order = tailwind_get_class_order(tw, classes + i, 2);
  //     sort_string(classes, order, n);
  //   });
  //
  //   printf("total: %fns\n", total);
  // }
  //
  // for (size_t i = 0; i < n; i += 4) {
  //   measure_pref(total, {
  //     order = tailwind_get_class_order(tw, classes + i, 4);
  //     sort_string(classes, order, n);
  //   });
  //
  //   printf("total: %fns\n", total);
  // }

  for (size_t i = 0; i < n; i += 5) {
    measure_pref(total, {
      order = tailwind_get_class_order(tw, classes + i, 5);
      sort_string(classes, order, n);
    });

    printf("total: %fns\n", total);
  }

  printf("Sorted: ");
  for (size_t i = 0; i < n; i++) {
    printf(str_f " ", str_fe(classes[i]));
  }
  printf("\n");

  string_free(classes);
  tailwind_free(tw);
  return 0;
}
