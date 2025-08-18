#include "vi_core.h"
#include "vi_line.h"
#include <string.h> 
#include <stdlib.h>

int main() {
  vi_file_t f;
  vi_if(&f, "main.c", O_RDONLY, 0755);

  size_t count = vi_cnt(f.f_m, strlen(f.f_m), 0x0A);

  printf("count: %d\n", count);

  vi_ff(&f);
  
  return 0;
}