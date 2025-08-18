#include <vi_line.h>

#if VI_GCC_CLANG 
__attribute__((always_inline, hot))
#endif
static inline size_t vi_cnt_b(viu8 *h, size_t h_l, viu8 n) {
  size_t c = 0;
  for (size_t i = 0; i < h_l; i++) {
    if ((uint8_t)h[i] == n) c++;
  }
  return c;
}

#if VI_GCC_CLANG
__attribute__((always_inline, hot))
static inline size_t vi_cnt_sse2(viu8 *h, size_t h_l, viu8 n) {
  size_t cnt = 0;

  __m128i na = _mm_set1_epi8(n);

  viui i = 0;
  while (i+32 <= h_l) {
    __m128i a1 = _mm_loadu_si128((__m128i*)(h+i));
    __m128i a2 = _mm_loadu_si128((__m128i*)(h+i+16));
    __m128i c1 = _mm_cmpeq_epi8(a1, na);
    __m128i c2 = _mm_cmpeq_epi8(a2, na);

    viui e = _mm_movemask_epi8(c1);
    viui f = _mm_movemask_epi8(c2);

    cnt += vi_popcnt(e) + vi_popcnt(f);

    i += 32;
  }

  cnt += vi_cnt_b(h+i, h_l-i, n);

  return cnt;
}
#endif

#if VI_GCC_CLANG
__attribute__((always_inline, hot))
static inline size_t vi_cnt_avx2(viu8 *h, size_t h_l, viu8 n) {
  size_t cnt = 0;

  __m256i na = _mm256_set1_epi8(n);

  viui i = 0;
  while (i+64 <= h_l) {
    __m256i a1 = _mm256_loadu_si256((__m256i*)(h+i));
    __m256i a2 = _mm256_loadu_si256((__m256i*)(h+i+32));
    __m256i c1 = _mm256_cmpeq_epi8(a1, na);
    __m256i c2 = _mm256_cmpeq_epi8(a2, na);

    viui e = _mm256_movemask_epi8(c1);
    viui f = _mm256_movemask_epi8(c2);

    cnt += vi_popcnt(e) + vi_popcnt(f);

    i += 64;
  }

  cnt += vi_cnt_b(h+i, h_l-i, n);

  return cnt;
}
#endif

void *vi_mt_th(void *a) {
  vi_td_t *cd = (vi_td_t*)a;

  cd->c_h(cd->c_m, cd->c_s, cd->t_b);

  return NULL;
}

size_t vi_cnt(viu8 *h, size_t h_l, viu8 n) {
  long np = sysconf(_SC_NPROCESSORS_ONLN);

  if (np == -1 || np == 1 || h_l < VI_DIMRET) {
    return vi_cnt_b(h, h_l, n);
  }

  viui share = h_l / np;
  viui lover = h_l % np;

  pthread_t thrarr[np];
  vi_td_t thrarrdat[np];

  vi_thc funvar;

#if defined(__AVX2__)
  funvar = vi_cnt_avx2;
#elif defined(__SSE2__)
  funvar = vi_cnt_sse2;
#else
  funvar = vi_cnt_b;
#endif

#ifndef VI_GCC_CLANG
  funvar = vi_cnt_b;
#endif 

  for (viui i = 0; i < np; i++) {
    size_t share2 = share;
    if (i == np - 1) share2 += lover;
    thrarrdat[i] = (vi_td_t){
      .c_m = h + (i * share),
      .c_s = share2,
      .t_r = 0,
      .c_h = funvar,
      .t_b = n
    };

    if (pthread_create(&thrarr[i], NULL, vi_mt_th, &thrarrdat[i]) != 0) {
      return vi_nhs_b(h, h_l, n);
    }
  }

  size_t cnt = 0;
  for (viui i = 0; i < np; i++) {
    pthread_join(thrarr[i], NULL);
    cnt += thrarrdat[i].t_r;
  }

  cnt += vi_cnt_b(h+cnt, lover, n);

  return cnt;
}