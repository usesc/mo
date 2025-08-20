#include <vi_line.h>

/* vi count byte, basic */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_bas(char *mem, size_t mln, char byt) {
  size_t c = 0;
  for (size_t i = 0; i < mln; i++) {
    if ((uint8_t)mem[i] == byt) c++;
  }
  return c;
}

/* vi count byte, sse2 */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_sse2(char *mem, size_t mln, char byt) {
  size_t cnt = 0;

  __m128i na = _mm_set1_epi8(byt);

  unsigned int i = 0;
  while (i+32 <= mln) {
    __m128i a1 = _mm_loadu_si128((__m128i*)(mem+i));
    __m128i a2 = _mm_loadu_si128((__m128i*)(mem+i+16));
    __m128i c1 = _mm_cmpeq_epi8(a1, na);
    __m128i c2 = _mm_cmpeq_epi8(a2, na);

    unsigned int e = _mm_movemask_epi8(c1);
    unsigned int f = _mm_movemask_epi8(c2);

    cnt += vi_popcnt(e) + vi_popcnt(f);

    i += 32;
  }

  cnt += vi_ctb_bas(mem+i, mln-i, byt);

  return cnt;
}

/* vi count byte, avx2 */
/* mem = memory, mln = memory length, byt = byte */
static inline __attribute__((always_inline, hot))
size_t vi_ctb_avx2(char *mem, size_t mln, char byt) {
  size_t cnt = 0;

  __m256i na = _mm256_set1_epi8(byt);

  unsigned int i = 0;
  while (i+64 <= mln) {
    __m256i a1 = _mm256_loadu_si256((__m256i*)(mem+i));
    __m256i a2 = _mm256_loadu_si256((__m256i*)(mem+i+32));
    __m256i c1 = _mm256_cmpeq_epi8(a1, na);
    __m256i c2 = _mm256_cmpeq_epi8(a2, na);

    unsigned int e = _mm256_movemask_epi8(c1);
    unsigned int f = _mm256_movemask_epi8(c2);

    cnt += vi_popcnt(e) + vi_popcnt(f);

    i += 64;
  }

  cnt += vi_ctb_bas(mem+i, mln-i, byt);

  return cnt;
}

/* vi pthread handler */
/* a = pthread arg */
void *vi_pth(void *a) {
  struct vi_td *cd = (struct vi_td *)a;

  cd->cbh(cd->chm, cd->chs, cd->tgb);

  return NULL;
}

/* vi count byte generalized */
/* mem = memory, mln = memory length, byt = byte */
size_t vi_ctb(char *mem, size_t mln, char byt) {
  /* nrp = N/number-of real processors */
  long nrp = sysconf(_SC_NPROCESSORS_ONLN);

  if (nrp == -1 || nrp == 1 || mln < VI_DIMRET) {
    goto simple_impl;
  }

  unsigned int share = mln / nrp;
  unsigned int lover = mln % nrp;

  pthread_t *threads = NULL;
  struct vi_td *threads_data = NULL;

  threads = malloc(nrp * sizeof(pthread_t));
  threads_data = malloc(nrp * sizeof(struct vi_td));
  if (!threads || !threads_data) goto simple_impl;

  vi_thc ctb_impl;

  #if defined(__AVX2__)
    ctb_impl = vi_ctb_avx2;
  #elif defined(__SSE2__)
    ctb_impl = vi_ctb_sse2;
  #else
    ctb_impl = vi_ctb_bas;
  #endif

  size_t i;
  for (i = 0; i < nrp; i++) {
    size_t share2 = share;
    if (i == nrp-1) share2 += lover;

    threads_data[i] = (struct vi_td) {
      .chm = mem + (i*share),
      .chs = share2,
      .trs = 0,
      .cbh = ctb_impl,
      .tgb = byt
    };

    if (pthread_create(&threads[i], NULL, vi_pth, &threads_data[i]) != 0) {
      goto simple_impl;
    }
  }

  size_t cnt = 0;

  size_t j;
  for (j = 0; j < nrp; j++) {
    pthread_join(threads[j], NULL);
    cnt += threads_data[j].trs;
  }

  if (threads)      free(threads);
  if (threads_data) free(threads_data);

  return cnt;

simple_impl:

  if (threads)      free(threads);
  if (threads_data) free(threads_data);

  return vi_ctb_bas(mem, mln, byt);
}
