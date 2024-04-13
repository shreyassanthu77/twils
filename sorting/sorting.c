#include "str/string.h"
#include <stddef.h>
#include <stdint.h>

void sort_string(string *strings, int64_t *order, size_t len) {
  for (size_t i = 0; i < len; i++) {
    size_t min = i;
    for (size_t j = i + 1; j < len; j++) {
      if (order[j] < order[min]) {
        min = j;
      }
    }

    string tmp = strings[i];
    strings[i] = strings[min];
    strings[min] = tmp;

    int64_t tmp_order = order[i];
    order[i] = order[min];
    order[min] = tmp_order;
  }
}
