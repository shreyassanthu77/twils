#ifndef TAILWIND_H
#define TAILWIND_H

#include <stdint.h>
#include <str/string.h>

typedef struct tailwind_t tailwind_t;

tailwind_t *tailwind_init(string base_path);

uint64_t tailwind_get_class_order(tailwind_t *tailwind, string class_name);

void tailwind_free(tailwind_t *tailwind);

#endif // TAILWIND_H
