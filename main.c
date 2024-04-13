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

#define bench(as, n, i, block, free)                                           \
  double as;                                                                   \
  {                                                                            \
    double __total = 0;                                                        \
    for (size_t i = 0; i < n; i++) {                                           \
      measure_pref(__as##elapsed, block);                                      \
      __total += __as##elapsed;                                                \
      free;                                                                    \
    }                                                                          \
    __total /= n;                                                              \
    as = __total;                                                              \
  }

// KHASH_MAP_INIT_STR(str, uint32_t);

int main() {
  const string js_path = str("./lib.js");
  const string css_path = str("./min.css");
  tailwind_t *tw = tailwind_init(js_path);
  tailwind_load_css(tw, css_path);

  size_t len;
  int64_t *result;

  const size_t n = 100'000;
  bench(
      total, n, i,
      {
        result = tailwind_get_class_order(
            tw, &len,
            string_new(
                "   bg-blue-500 \n \rtext-white py-2 px-4 what "
                "hover:focus:bg-blue-700 "
                "md:text-lg lg:hover:text-red-500\t \t \r\n focus:outline-none "
                "focus:ring     \t some-random-class"
                "focus:ring-blue-500 focus:ring-opacity-50 shadow-lg       "
                "rounded-lg border       border-transparent bg-gradient-to-r "
                "from-blue-500 to-blue-700",
                303));
      },
      (void)(0));
  printf("Average time: %fμs\n", total);
  for (size_t i = 0; i < len; i++) {
    printf("%ld", result[i]);
    if (i < len - 1) {
      printf(", ");
    }
  }
  printf("\n");

  free(result);
  tailwind_free(tw);
  return 0;
}
