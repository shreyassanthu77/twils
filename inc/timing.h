#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <unistd.h>

#define measure_pref(as, block)                                                \
  double as;                                                                   \
  {                                                                            \
    struct timespec start, end;                                                \
    clock_gettime(CLOCK_REALTIME, &start);                                     \
    block;                                                                     \
    clock_gettime(CLOCK_REALTIME, &end);                                       \
    as = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);    \
  }

static inline void noop() { clock_gettime(CLOCK_REALTIME, NULL); }
