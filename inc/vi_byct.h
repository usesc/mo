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

#include <vi_core.h>

#define VI_DIMRET 4096

/* vi thread handler callback */
typedef size_t (*vi_thc)(char *, size_t, char);

__attribute__((aligned(64)))
struct vi_td {
  char * chm; /* chunk memory   */
  size_t chs; /* chunk size     */
  size_t trs; /* thread result  */
  vi_thc cbh; /* countb handle  */
  char   tgb; /* target byte    */
};

/* vi count byte, basic */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_bas(char *mem, size_t mln, char byt);

/* vi count byte, sse2 */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_sse2(char *mem, size_t mln, char byt);

/* vi count byte, avx2 */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_avx2(char *mem, size_t mln, char byt);

/* vi pthread handler */
/* a = pthread arg */
void *vi_pth(void *a);

/* vv use this function vv */

/* vi count byte generalized */
/* mem = memory, mln = memory length, byt = byte */
size_t vi_ctb(char *mem, size_t mln, char byt);

#endif
