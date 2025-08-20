#ifndef VI_CORE_H
#define VI_CORE_H 1

#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

#include <vi_line.h>

#define vi_memcpy(dest, src, n) __builtin_memcpy(dest, src, n)
#define vi_memmov(dest, src, n) __builtin_memmove(dest, src, n)
#define vi_memset(s, c, n)      __builtin_memset(s, c, n)
#define vi_strlen(s)            __builtin_strlen(s)
#define vi_popcnt(a)            __builtin_popcount(a)

#if DEBUG
#define vi_errors(s) perror(s)
#define vi_assert(x) assert(x)
#define vi_nassert(x) assert(!x)
#else
#define vi_errors(s) 
#define vi_assert(x)
#define vi_nassert(x)
#endif

#define vi_fail(expr, str) do{if(expr) {vi_errors(str); vi_nassert(expr)}}while(0)
#define vi_runc(expr, rval) do{if (expr) {return rval;}}while(0) 

/* vi television struct */
struct vi_tv {
	size_t cso; /* cursor offset */
	size_t viy; /* viewport y    */
	size_t teh; /* term height   */
	size_t tew; /* term width    */
};

/* vi file struct */
struct vi_file {
	char *      fim; /* file memory        */
	size_t      fml; /* file memory length */
	char *      fin; /* file name          */
	size_t      fns; /* file name size     */
	int         fid; /* file descriptor    */
	struct stat sts; /* struct stat        */
};

/* vi insert */
/* out = output, ous = output size, inp = input, ins = input size, pos = position, cha = char */
static inline __attribute__((always_inline, hot))
ssize_t vi_insert(char *out, unsigned int ous, char *inp, unsigned int ins, unsigned int pos, char cha);

/* vi delete */
/* out = output, inp = input, ins = input size, pos = position */
static inline __attribute__((always_inline, hot))
ssize_t vi_delete(char *out, char *inp, unsigned int ins, unsigned int pos);

/* vi init vi file */
/* vif = vi file, fin = filename, fla = flags, mod = mode */
int vi_inivif(struct vi_file *vif, char *fin, int fla, mode_t mod);

/* vi free file */
/* vif = vi file */
void vi_freevif(struct vi_file *vif);

/* vi increment until byte */
/* mem = memory, mln = memory length, cha = char */
static inline __attribute__((always_inline, hot))
off_t vi_iub(const char *mem, size_t mln, char cha);

/* vi decrement until byte */
/* mem = memory, off = memory offset, cha = char */
static inline __attribute__((always_inline, hot))
off_t vi_dub(const char *mem, off_t off, char cha);

#endif
