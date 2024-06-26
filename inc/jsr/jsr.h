#ifndef JSR_H
#define JSR_H

#include <quickjs/quickjs.h>
#include <str/string.h>

typedef struct jsr_t jsr_t;

jsr_t *jsr_init(void);

bool jsr_eval_module(jsr_t *jsr, string module_name, string module_path);

bool jsr_eval_module_str(jsr_t *jsr, string module_name, string source);

JSValue jsr_to_js_string(jsr_t *jsr, string str);

void jsr_to_string(jsr_t *jsr, string *result, JSValue value);

JSValue jsr_new_string_array(jsr_t *jsr, string *strings, size_t len);

int64_t jsr_to_int64(jsr_t *jsr, JSValue value);

int64_t jsr_get_array_length(jsr_t *jsr, JSValue value);

int64_t *jsr_to_int64_array(jsr_t *jsr, JSValue value, int64_t *buf,
                            size_t len);

JSValue jsr_get_global(jsr_t *jsr, string name);

JSValue jsr_get(jsr_t *jsr, JSValue obj, string name);

JSValue jsr_call(jsr_t *jsr, JSValue func, size_t argc, JSValueConst *argv);

void jsr_free_value(jsr_t *jsr, JSValue value);

void jsr_debug(jsr_t *jsr, JSValue value);

void jsr_free(jsr_t *jsr);

#endif // JSR_H
