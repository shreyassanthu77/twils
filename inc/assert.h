
#ifndef MY_ASSERT_H
#ifndef NO_ASSERT
#define assert(expr)                                                           \
  do {                                                                         \
    if (!(expr)) {                                                             \
      fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #expr,       \
              __FILE__, __LINE__);                                             \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)
#else
#define assert(expr) ((void)0)
#endif
#endif
