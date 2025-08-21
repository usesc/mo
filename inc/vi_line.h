#include <unistd.h>

struct vi_line {
  size_t *lin; /* line array  */
  size_t  lil; /* line number */
};