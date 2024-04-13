#include "jsr.h"
#include "quickjs/quickjs.h"
#include <fs.h>
#include <stdlib.h>

static inline bool jsr_print_value(jsr_t *jsr, JSValue value, string msg);

typedef struct jsr_t {
  JSRuntime *rt;
  JSContext *ctx;
  JSValue globalThis;
} jsr_t;

jsr_t *jsr_init(void) {
  jsr_t *jsr = calloc(1, sizeof(jsr_t));
  jsr->rt = JS_NewRuntime();
  jsr->ctx = JS_NewContext(jsr->rt);
  jsr->globalThis = JS_GetGlobalObject(jsr->ctx);

  return jsr;
}

bool jsr_eval_module(jsr_t *jsr, string module_name, string module_path) {
  string module_str = read_file(module_path);
  bool res = jsr_eval_module_str(jsr, module_name, module_str);
  string_free(&module_str);
  return res;
}

bool jsr_eval_module_str(jsr_t *jsr, string module_name, string source) {
  JSValue module =
      JS_Eval(jsr->ctx, str_av(source), module_name.data, JS_EVAL_TYPE_MODULE);
  bool res = jsr_print_value(
      jsr, module, string_fmt("Loading module " str_f, str_fe(module_name)));
  JS_FreeValue(jsr->ctx, module);
  return res;
}

typedef struct {
  JSRefCountHeader header; /* must come first, 32-bit */
  uint32_t len : 31;
  uint8_t is_wide_char : 1; /* 0 = 8 bits, 1 = 16 bits characters */
  /* for JS_ATOM_TYPE_SYMBOL: hash = 0, atom_type = 3,
     for JS_ATOM_TYPE_PRIVATE: hash = 1, atom_type = 3
     XXX: could change encoding to have one more bit in hash */
  uint32_t hash : 30;
  uint8_t atom_type : 2; /* != 0 if atom, JS_ATOM_TYPE_x */
  uint32_t hash_next;    /* atom_index for JS_ATOM_TYPE_SYMBOL */
  union {
    uint8_t *str8; /* 8 bit strings will get an extra null terminator */
    uint16_t str16[0];
  } u;
} JSString;

inline JSValue jsr_to_js_string(jsr_t *jsr, string str) {
  JSValue js_str = JS_NewStringLen(jsr->ctx, str_av(str));
  return js_str;
}

JSValue jsr_new_string_array(jsr_t *jsr, string *strings, size_t len) {
  JSValue arr = JS_NewArray(jsr->ctx);
  for (size_t i = 0; i < len; i++) {
    JSValue str = jsr_to_js_string(jsr, strings[i]);
    JS_SetPropertyUint32(jsr->ctx, arr, i, str);
  }
  return arr;
}

void jsr_to_string(jsr_t *jsr, string *result, JSValue value) {
  size_t len;
  const char *str = JS_ToCStringLen(jsr->ctx, &len, value);
  string_write(result, 0, str, len);
  JS_FreeCString(jsr->ctx, str);
  JS_FreeValue(jsr->ctx, value);
}

int64_t jsr_to_int64(jsr_t *jsr, JSValue value) {
  if (JS_IsNull(value)) {
    return -1;
  }
  int64_t res = 0;
  JS_ToInt64Ext(jsr->ctx, &res, value);
  JS_FreeValue(jsr->ctx, value);
  return res;
}

inline int64_t jsr_get_array_length(jsr_t *jsr, JSValue value) {
  JSValue n_val = JS_GetPropertyStr(jsr->ctx, value, "length");
  int64_t n = jsr_to_int64(jsr, n_val);
  JS_FreeValue(jsr->ctx, n_val);
  return n;
}

int64_t *jsr_to_int64_array(jsr_t *jsr, JSValue value, int64_t *buf,
                            size_t len) {
  for (size_t i = 0; i < len; i++) {
    JSValue val = JS_GetPropertyUint32(jsr->ctx, value, i);
    if (JS_IsNull(val)) {
      buf[i] = -1;
    }
    buf[i] = jsr_to_int64(jsr, val);
  }
  JS_FreeValue(jsr->ctx, value);
  return buf;
}

JSValue jsr_get_global(jsr_t *jsr, string name) {
  JSValue global = JS_GetPropertyStr(jsr->ctx, jsr->globalThis, name.data);
  return global;
}

JSValue jsr_get(jsr_t *jsr, JSValue obj, string name) {
  JSValue prop = JS_GetPropertyStr(jsr->ctx, obj, name.data);
  return prop;
}

JSValue jsr_call(jsr_t *jsr, JSValue func, size_t argc, JSValueConst *argv) {
  JSValue res = JS_Call(jsr->ctx, func, jsr->globalThis, argc, argv);
  return res;
}

static size_t __run = 0;
static string __msg = str("run_");
JSValue jsr_run(jsr_t *jsr, string code) {
  string_writef(&__msg, 4, "%zu.js", ++__run);
  JSValue res =
      JS_Eval(jsr->ctx, str_av(code), __msg.data, JS_EVAL_TYPE_MODULE);
  return res;
}

void jsr_debug(jsr_t *jsr, JSValue value) {
#ifndef NDEBUG
  if (JS_IsException(value)) {
    JSValue exception_val = JS_GetException(jsr->ctx);
    const char *exception_str = JS_ToCString(jsr->ctx, exception_val);
    fprintf(stderr, "[ERR] %s\n", exception_str);
    JS_FreeCString(jsr->ctx, exception_str);
    JS_FreeValue(jsr->ctx, exception_val);
    return;
  }
  const char *str = JS_ToCString(jsr->ctx, value);
  fprintf(stderr, "[DBG] %s\n", str);
  JS_FreeCString(jsr->ctx, str);
#endif
}

void jsr_free(jsr_t *jsr) {
  JS_FreeValue(jsr->ctx, jsr->globalThis);
  JS_FreeContext(jsr->ctx);
  JS_FreeRuntime(jsr->rt);
  string_free(&__msg);
  free(jsr);
}

void jsr_free_value(jsr_t *jsr, JSValue value) {
  JS_FreeValue(jsr->ctx, value);
}

static inline bool jsr_print_value(jsr_t *jsr, JSValue value, string msg) {
#ifndef NDEBUG
  if (JS_IsException(value)) {
    JSValue exception_val = JS_GetException(jsr->ctx);
    const char *exception_str = JS_ToCString(jsr->ctx, exception_val);
    fprintf(stderr, "[ERR] " str_f ": %s\n", str_fe(msg), exception_str);
    JS_FreeCString(jsr->ctx, exception_str);
    JS_FreeValue(jsr->ctx, exception_val);
    return false;
  } else {
    const char *str = JS_ToCString(jsr->ctx, value);
    fprintf(stderr, "[OK] " str_f ": %s\n", str_fe(msg), str);
    JS_FreeCString(jsr->ctx, str);
    return true;
  }

#endif
  return true;
}
