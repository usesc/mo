#include <vi_core.h>

/* vi insert */
#if VI_GCC_CLANG 
__attribute__((always_inline, hot))
#endif
static inline ssize_t vi_ins(viu8 *o, viui o_s, viu8 *i, viui i_s, viui p, viu8 c) {
	vi_assert(o != NULL);
	vi_assert(i != NULL);
	vi_assert(p <= i_s);
	vi_assert(i_s+1 <= o_s);

	if (p > i_s || i_s+1 > o_s) return -1; 

	if (o != i) vi_memcpy(o, i, i_s);
	vi_memmov(o+p+1, o+p, i_s-p);
	
	o[p] = c;
	return i_s+1;
}

/* vi delete */
#if VI_GCC_CLANG 
__attribute__((always_inline, hot))
#endif
static inline ssize_t vi_del(viu8 *o, viui o_s, viu8 *i, viui i_s, viui p) {
	vi_assert(o != NULL);
  vi_assert(i != NULL);
  vi_assert(p < i_s);

	if (p >= i_s) return -1; 

	if (o != i) vi_memcpy(o, i, i_s);
	vi_memmov(o+p, o+p+1, i_s-p-1);
	
	return i_s-1;
}

/* vi init file */
vist vi_if(vi_file_t *vf, viu8 *f, int fl, mode_t m) {
	vf->f_n = f;
	vf->n_s = vi_strlen(f);

	vf->f_d = open(f, fl, m);
	if (vf->f_d  == -1) {
		vi_errors("open");
		return -1;
	}

	if (fstat(vf->f_d , &vf->s_t) == -1) {
		vi_errors("fstat");
		close(vf->f_d);
		return -1;
	}

	vf->f_l = vf->s_t.st_size;

	vf->f_m = malloc(vf->f_l);
	if (!vf->f_m) {
		vi_errors("malloc");
		close(vf->f_d);
		return -1;
	}

	ssize_t n = read(vf->f_d, vf->f_m, vf->f_l);
	if (n != vf->f_l) {
		vi_errors("read");
		close(vf->f_d);
		free(vf->f_m);
		return -1;
	}
	
	return 0;
}

/* vi free file */
void vi_ff(vi_file_t *vf) {
	free(vf->f_m);
	vf->f_m = NULL;
	close(vf->f_d);
}
