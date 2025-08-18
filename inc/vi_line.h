#ifndef VI_LINE_H
#define VI_LINE_H 1

#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>

#include <vi_type.h>
#include <vi_core.h>

#define VI_DIMRET 4096

/* vi thread handler callback */
typedef size_t (*vi_thc)(viu8 *, size_t, viu8);

typedef struct vi_td {
  viu8 * c_m; /* chunk memory   */
  size_t c_s; /* chunk size     */
  size_t t_r; /* thread result  */
  vi_thc c_h; /* countb handle  */
  char   t_b; /* target byte    */
} vi_td_t; 

/* vi count byte - basic */
#if VI_GCC_CLANG
__attribute__((always_inline, hot))
#endif
static inline size_t vi_cnt_b(viu8 *h, size_t h_l, viu8 n);

/* vi count byte - sse2 */
#if VI_GCC_CLANG
__attribute__((always_inline, hot))
static inline size_t vi_cnt_sse2(viu8 *h, size_t h_l, viu8 n);
#endif

/* vi count byte - avx2 */
#if VI_GCC_CLANG
__attribute__((always_inline, hot))
static inline size_t vi_cnt_avx2(viu8 *h, size_t h_l, viu8 n);
#endif

/* vi multithread thread handler */
void *vi_mt_th(void *a);

/* vi count byte - USE THIS FUNCTION */
size_t vi_cnt(viu8 *h, size_t h_l, viu8 n);

#endif