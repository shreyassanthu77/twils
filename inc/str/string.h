#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
  bool owned;
} string;

#define str_f "%.*s"
#define str_fe(str) (int)str.length, str.data
#define str_av(str) str.data, str.length
#define str_ap(str) str->data, str->length

#define str(s)                                                                 \
  (string) {                                                                   \
    .data = s, .length = sizeof(s) - 1, .capacity = sizeof(s), .owned = false  \
  }

#define str_n(s, n)                                                            \
  (string) { .data = s, .length = n, .capacity = n, .owned = false }
/**
 * Create a new string with a given capacity.
 *
 * @param capacity The capacity of the string.
 * @return The new string.
 */
string string_with_capacity(size_t capacity);
/**
 * Create a new string with a given data.
 *
 * @param data The data of the string.
 * @param length The length of the data.
 * @return The new string.
 */
string string_new(const char *data, size_t length);

/**
 * Create a new string with a given format.
 *
 * @param fmt The format of the string.
 * @return The new string.
 */
string string_fmt(const char *fmt, ...);
/**
 * Clone a string.
 *
 * @param str The string to clone.
 * @return The cloned string.
 */
string string_clone(string *str);
/**
 * Free a string.
 *
 * @param str The string to free.
 */
void string_free(string *str);

void string_write(string *str, size_t from, const char *data, size_t length);

/**
 * Same as string_write but with a format.
 * Reallocates the string if needed.
 *
 * @param str The string.
 * @param from The index to start writing.
 * @param fmt The format.
 */
void string_writef(string *str, size_t from, const char *fmt, ...);

string *string_split_whitespace(string str, size_t *length);

void string_merge_whitespace(string str, string *dest);

#endif // STRING_H
