#include "str/string.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void sort_string(string str, string *dest, int64_t *order, size_t len) {
  string *split = string_split_whitespace(str, NULL);
  for (size_t i = 0; i < len; i++) {
    size_t min = i;
    for (size_t j = i + 1; j < len; j++) {
      if (order[j] < order[min]) {
        min = j;
      }
    }

    string tmp = split[i];
    split[i] = split[min];
    split[min] = tmp;

    int64_t tmp_order = order[i];
    order[i] = order[min];
    order[min] = tmp_order;
  }

  dest->length = 0;
  for (size_t i = 0; i < len; i++) {
    string_write(dest, dest->length, str_av(split[i]));
    if (i < len - 1) {
      string_write(dest, dest->length, " ", 1);
    }
  }
}
