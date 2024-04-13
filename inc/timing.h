#include <sys/time.h>

#define measure_pref(as, block)                                                \
  double as;                                                                   \
  {                                                                            \
    struct timeval start, end;                                                 \
    gettimeofday(&start, NULL);                                                \
    block;                                                                     \
    gettimeofday(&end, NULL);                                                  \
    as = (end.tv_sec - start.tv_sec) * 1e6;                                    \
    as += (end.tv_usec - start.tv_usec);                                       \
  }
