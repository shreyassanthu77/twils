#include "tailwind.h"
#include "timing.h"
#include <fs.h>
#include <jsr/jsr.h>
#include <stdlib.h>
#include <str/string.h>

typedef struct tailwind_t {
  jsr_t *jsr;
  JSValue design_system;
  bool loaded;
  JSValue get_class_order;
} tailwind_t;

tailwind_t *tailwind_init(string source_path) {
  tailwind_t *tailwind = calloc(1, sizeof(tailwind_t));
  tailwind->loaded = false;

  tailwind->jsr = jsr_init();
  jsr_eval_module(tailwind->jsr, str("tailwind.js"), source_path);

  const string load_tw_to_global = str(
      //
      "import * as tw from 'tailwind.js';"
      "globalThis.load_design_system = tw.__unstable__loadDesignSystem;"
      "globalThis.getClassOrder = function(ds, cls) {"
      "	const res = ds.getClassOrder(cls);"
      "	return res.map(x => x[1]);"
      "};"
      //
  );
  jsr_eval_module_str(tailwind->jsr, str("load_tw_to_global"),
                      load_tw_to_global);

  return tailwind;
}

bool tailwind_load_css(tailwind_t *tailwind, string css_path) {
  string css_str = read_file(css_path);
  JSValue css_value = jsr_to_js_string(tailwind->jsr, css_str);

  JSValue load_design_system_fn =
      jsr_get_global(tailwind->jsr, str("load_design_system"));
  tailwind->design_system =
      jsr_call(tailwind->jsr, load_design_system_fn, 1, &css_value);
  jsr_free_value(tailwind->jsr, load_design_system_fn);

  bool success = JS_IsException(tailwind->design_system) == false;
  if (success) {
    tailwind->get_class_order =
        jsr_get_global(tailwind->jsr, str("getClassOrder"));
    tailwind->loaded = true;
  }
  return success;
}

static size_t cap = 0;
static int64_t *buf = NULL;
int64_t *tailwind_get_class_order(tailwind_t *tailwind, string *classes,
                                  size_t len) {
  if (!tailwind->loaded) {
    return NULL;
  }
  JSValue cls_arr = jsr_new_string_array(tailwind->jsr, classes, len);
  JSValue args[2] = {tailwind->design_system, cls_arr};
  JSValue order = jsr_call(tailwind->jsr, tailwind->get_class_order, 2, args);

  if (cap == 0) {
    cap = len;
    buf = calloc(cap, sizeof(int64_t));
  } else if (cap < len) {
    cap = len;
    buf = realloc(buf, cap * sizeof(int64_t));
  }

  int64_t *order_buf = jsr_to_int64_array(tailwind->jsr, order, buf, len);
  jsr_free_value(tailwind->jsr, cls_arr);
  return order_buf;
}

void tailwind_free(tailwind_t *tailwind) {
  jsr_free_value(tailwind->jsr, tailwind->design_system);
  jsr_free_value(tailwind->jsr, tailwind->get_class_order);
  jsr_free(tailwind->jsr);
  free(buf);
  free(tailwind);
}
