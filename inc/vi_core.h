#ifndef VI_CORE_H
#define VI_CORE_H 1

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

#include <vi_type.h>
#include <vi_line.h>

#if defined(__clang__) || defined(__GNUC__)
#define VI_GCC_CLANG 1
#define vi_memcpy(dest, src, n) __builtin_memcpy(dest, src, n)
#define vi_memmov(dest, src, n) __builtin_memmove(dest, src, n)
#define vi_memset(s, c, n)      __builtin_memset(s, c, n)
#define vi_strlen(s)            __builtin_strlen(s)
#define vi_popcnt(a)            __builtin_popcount(a)
#else
#include <string.h>
#define vi_memcpy(dest, src, n) memcpy(dest, src, n)
#define vi_memmov(dest, src, n) memmove(dest, src, n)
#define vi_memset(s, c, n)      memset(s, c, n)
#define vi_strlen(s)            strlen(s)
#endif

#define VILOGERR 1
#if VILOGERR
#define vi_errors(s) perror(s)
#define vi_assert(x) assert(x)
#else
#define vi_errors(s) 
#define vi_assert(x)
#endif

/* vi television struct */
typedef struct vi_tv {
	size_t c_o; /* cursor offset */
	size_t v_y; /* viewport y    */
	size_t t_h; /* term height   */
	size_t t_w; /* term width    */
} vi_tv_t;

/* vi file struct */
typedef struct vi_file {
	viu8 * f_m; /* file memory     */
	size_t f_l; /* file mem length */
	viu8 * f_n; /* file name       */
	size_t n_s; /* file name size  */
	int    f_d; /* file descriptor */
	stat_t s_t; /* struct stat     */
} vi_file_t;

/* vi insert */
#if VI_GCC_CLANG 
__attribute__((always_inline, hot))
#endif
static inline ssize_t vi_ins(viu8 *o, viui o_s, viu8 *i, viui i_s, viui p, viu8 c);

/* vi delete */
#if VI_GCC_CLANG 
__attribute__((always_inline, hot))
#endif
static inline ssize_t vi_del(viu8 *o, viui o_s, viu8 *i, viui i_s, viui p);

vist vi_if(vi_file_t *vf, viu8 *f, int fl, mode_t m);

void vi_ff(vi_file_t *vf);

#endif