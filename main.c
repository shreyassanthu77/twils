#include "sorting/sorting.h"
#include "str/string.h"
#include "tailwind/tailwind.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/time.h>
#define FS_IMPLEMENTATION
#include "fs.h"
#include <stdio.h>

#define measure_pref(as, block)                                                \
  double as;                                                                   \
  {                                                                            \
    struct timeval start, end;                                                 \
    gettimeofday(&start, NULL);                                                \
    block;                                                                     \
    gettimeofday(&end, NULL);                                                  \
    as = (end.tv_sec - start.tv_sec) * 1e6;                                    \
    as += (end.tv_usec - start.tv_usec);                                       \
  }

#define with(init, cleanup, block)                                             \
  init;                                                                        \
  block;                                                                       \
  cleanup;

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

  with(tailwind_t *tw = tailwind_init(js_path), tailwind_free(tw), {
    tailwind_load_css(tw, css_path);
    with(string css = string_with_capacity(1024), string_free(&css), {
      string_merge_whitespace(test_css, &css);
      with(string sorted = string_with_capacity(1024), string_free(&sorted), {
        size_t len;
        int64_t *result;

        tailwind_get_class_order(tw, &len, css);
        measure_pref(total, {
          result = tailwind_get_class_order(tw, &len, css);
          sort_string(css, &sorted, result, len);
        });

        printf("Average time: %fμs\n", total);
        printf("Sorted: " str_f "\n", str_fe(sorted));
      });
    });
  });
  return 0;
}
