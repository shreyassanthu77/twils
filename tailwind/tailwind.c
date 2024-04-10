#include "tailwind.h"
#include <quickjs/quickjs.h>
#include <stdio.h>
#include <stdlib.h>
#include <str/string.h>

typedef struct tailwind_t {
  string base_path;
  JSRuntime *rt;
  JSContext *ctx;
} tailwind_t;

static inline void setup_qjs(tailwind_t *tailwind) {
  tailwind->rt = JS_NewRuntime();
  tailwind->ctx = JS_NewContext(tailwind->rt);
}

tailwind_t *tailwind_init(string base_path) {
  tailwind_t *tailwind = calloc(1, sizeof(tailwind_t));

  tailwind->base_path = string_clone(&base_path);

  setup_qjs(tailwind);

  return tailwind;
}

uint64_t tailwind_get_class_order(tailwind_t *tailwind, string class_name) {
  JSValue v = JS_Eval(tailwind->ctx, "globalThis.add = (a, b) => a + b;", 29,
                      "main.js", JS_EVAL_TYPE_GLOBAL);
  JS_FreeValue(tailwind->ctx, v);

  JSValue global = JS_GetGlobalObject(tailwind->ctx);
  JSValue add = JS_GetPropertyStr(tailwind->ctx, global, "add");

  JSValue args[] = {JS_NewInt32(tailwind->ctx, 1),
                    JS_NewInt32(tailwind->ctx, 1)};
  JSValue res = JS_Call(tailwind->ctx, add, global, 2, (JSValueConst *)args);

  if (JS_IsException(res)) {
    JSValue exception_val = JS_GetException(tailwind->ctx);
    JSValue exception_str = JS_ToString(tailwind->ctx, exception_val);
    const char *exception_cstr = JS_ToCString(tailwind->ctx, exception_str);
    printf("Exception: %s\n", exception_cstr);
    JS_FreeCString(tailwind->ctx, exception_cstr);
    JS_FreeValue(tailwind->ctx, exception_str);
    JS_FreeValue(tailwind->ctx, exception_val);
  }

  const char *str = JS_ToCString(tailwind->ctx, res);
  printf("Result: %s\n", str);
  JS_FreeCString(tailwind->ctx, str);

  return 0;
}

void tailwind_free(tailwind_t *tailwind) {
  JS_FreeContext(tailwind->ctx);
  JS_FreeRuntime(tailwind->rt);

  string_free(&tailwind->base_path);
  free(tailwind);
}
