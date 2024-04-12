#include "string.h"
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create a new string with a given capacity.
 *
 * @param capacity The capacity of the string.
 * @return The new string.
 */
string string_with_capacity(size_t capacity) {
  char *data = calloc(capacity + 1, sizeof(char));
  return (string){
      .data = data,
      .length = 0,
      .capacity = capacity,
      .owned = true,
  };
}

/**
 * Create a new string with a given data.
 *
 * @param data The data of the string.
 * @param length The length of the data.
 * @return The new string.
 */
string string_new(const char *data, size_t length) {
  string str = string_with_capacity(length);
  memcpy(str.data, data, length);
  str.length = length;
  return str;
}

/**
 * Create a new string with a given format.
 *
 * @param fmt The format of the string.
 * @return The new string.
 */
string string_fmt(const char *fmt, ...) {
  va_list args;
  va_start(args);

  va_list args_copy;
  va_copy(args_copy, args);
  size_t length = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);

  string str = string_with_capacity(length);
  vsnprintf(str.data, length + 1, fmt, args);
  str.length = length;

  return str;
}

/**
 * Clone a string.
 *
 * @param str The string to clone.
 * @return The cloned string.
 */
inline string string_clone(string *str) {
  string new_str = {
      .data = str->data,
      .length = str->length,
      .capacity = str->capacity,
      .owned = false, // We don't own the data so will be copied when modified
  };
  return new_str;
}

/**
 * Free a string.
 *
 * @param str The string to free.
 */
void string_free(string *str) {
  if (str == NULL || str->data == NULL || !str->owned) {
    return;
  }

  free(str->data);
}

#define max(a, b) ((a) > (b) ? (a) : (b))
/**
 * Get the length of a string.
 * Reallocates the string if needed.
 *
 * @param str The string.
 * @return The length of the string.
 */
void string_write(string *str, size_t from, const char *data, size_t length) {
  if (!str->owned) {
    str->data = calloc(str->length + 1, sizeof(char));
    memcpy(str->data, str->data, str->length);
    str->capacity = str->length;
    str->owned = true;
  }

  size_t capacity = str->capacity - from;
  if (length > capacity) {
    size_t new_capacity =
        max(str->capacity * 2, str->capacity + (length - capacity));
    str->data = realloc(str->data, new_capacity + 1);
    str->capacity = new_capacity;
  }
  memcpy(str->data + from, data, length);
  str->length = max(str->length, from + length);
}

/**
 * Same as string_write but with a format.
 * Reallocates the string if needed.
 *
 * @param str The string.
 * @param from The index to start writing.
 * @param fmt The format.
 */
void string_writef(string *str, size_t from, const char *fmt, ...) {
  if (!str->owned) {
    str->data = calloc(str->length + 1, sizeof(char));
    memcpy(str->data, str->data, str->length);
    str->capacity = str->length;
    str->owned = true;
  }

  va_list args;
  va_start(args, fmt);

  va_list args_copy;
  va_copy(args_copy, args);
  size_t length = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);

  size_t capacity = str->capacity - from;
  if (length > capacity) {
    size_t new_capacity =
        max(str->capacity * 2, str->capacity + (length - capacity));
    str->data = realloc(str->data, new_capacity + 1);
    str->capacity = new_capacity;
  }

  vsnprintf(str->data + from, length, fmt, args);

  str->length = max(str->length, from + length);
  va_end(args);
}
#undef max

string *string_split_whitespace(string str, size_t *length) {
  string *strings = calloc(4, sizeof(string));
  size_t capacity = 4;
  *length = 0;
  for (size_t i = 0; i < str.length; i++) {
    if (isspace(str.data[i])) {
      continue;
    }

    size_t start = i;
    while (i < str.length && !isspace(str.data[i])) {
      i++;
    }

    if (*length == capacity) {
      capacity *= 2;
      strings = realloc(strings, capacity * sizeof(string));
    }

    strings[*length] = str_n(str.data + start, i - start);
    (*length)++;
  }
  return strings;
}
