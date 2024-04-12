#ifndef FS_H
#define FS_H

#include <str/string.h>

string read_file(string path);
#endif // FS_H

#ifdef FS_IMPLEMENTATION
#include <stdio.h>

string read_file(string path) {
  FILE *file = fopen(path.data, "r");
  if (!file) {
    return str("");
  }

  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);

  string result = string_with_capacity(length);
  fread(result.data, length, 1, file);
  fclose(file);
  result.length = length;

  return result;
}
#endif // FS_IMPLEMENTATION
