#ifndef TAILWIND_H
#define TAILWIND_H

#include <stdint.h>
#include <str/string.h>

typedef struct tailwind_t tailwind_t;

typedef struct {
  int64_t a;
  int64_t b;
} tailwind_class_order_t;

tailwind_t *tailwind_init(string source_path);

bool tailwind_load_css(tailwind_t *tailwind, string css_path);

tailwind_class_order_t tailwind_get_class_order(tailwind_t *tailwind,
                                                string class_a, string class_b);

void tailwind_free(tailwind_t *tailwind);

#endif // TAILWIND_H
