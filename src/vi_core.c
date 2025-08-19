#include <vi_core.h>

/* vi insert */
/* out = output, ous = output size, inp = input, ins = input size, pos = position, cha = char */
static inline __attribute__((always_inline, hot))
ssize_t vi_insert(char *out, unsigned int ous, char *inp, unsigned int ins, unsigned int pos, char cha) {
	vi_assert(out != NULL);
	vi_assert(inp != NULL);
	vi_assert(pos <= ins);
	vi_assert(ins+1 <= ous);

	if (pos > ins || ins+1 > ous) return -1;

	if (out != inp) vi_memcpy(out, inp, ins);
	vi_memmov(out+pos+1, out+pos, ins-pos);
	
	out[pos] = cha;
	return ins+1;
}

/* vi delete */
/* out = output, inp = input, ins = input size, pos = position */
static inline __attribute__((always_inline, hot))
ssize_t vi_delete(char *out, char *inp, unsigned int ins, unsigned int pos) {
	vi_assert(out != NULL);
	vi_assert(inp != NULL);
	vi_assert(pos < ins);

	if (pos >= ins) return -1; 

	if (out != inp) vi_memcpy(out, inp, ins);
	vi_memmov(out+pos, out+pos+1, ins-pos-1);
	
	return ins-1;
}

/* vi delete two, TODO: */
/* deletes memory from one off_t to another and shifts memory down  */

/* vi init vi file */
/* vif = vi file, fin = filename, fla = flags, mod = mode */
int vi_inivif(struct vi_file *vif, char *fin, int fla, mode_t mod) {
	vif->fin = fin;
	vif->fns = vi_strlen(fin);

	vif->fid = open(fin, fla, mod);
	if (vif->fid  == -1) {
		vi_errors("open");
		return -1;
	}

	if (fstat(vif->fid , &vif->sts) == -1) {
		vi_errors("fstat");
		close(vif->fid);
		return -1;
	}

	vif->fml = vif->sts.st_size;

	vif->fim = malloc(vif->fml);
	if (!vif->fim) {
		vi_errors("malloc");
		close(vif->fid);
		return -1;
	}

	ssize_t n = read(vif->fid, vif->fim, vif->fml);
	if (n != vif->fml) {
		vi_errors("read");
		close(vif->fid);
		free(vif->fim);
		return -1;
	}
	
	return 0;
}

/* vi free file */
/* vif = vi file */
void vi_freevif(struct vi_file *vif) {
	if (!vif) return;

	if (vif->fim) {
		free(vif->fim);
    	vif->fim = NULL;
  	}

  	if (vif->fid >= 0) {
    	close(vif->fid);
    	vif->fid = -1;
  	}
	
  	memset(vif, 0, sizeof(*vif));
}

/* vi increment until byte */
/* mem = memory, mln = length, cha = char */
static inline __attribute__((always_inline, hot))
off_t vi_iub(const char *mem, size_t mln, char cha) {
	for (off_t i = 0; i < mln; i++) {
		if (mem[i] == cha) return i;
	}
  	return -1; 
}

/* vi decrement until byte */
/* mem = memory, off = offset, cha = char */
static inline __attribute__((always_inline, hot))
off_t vi_dub(const char *mem, off_t off, char cha) {
	for (off_t i = off; i >= 0; i--) {
		if (mem[i] == cha) return i;
	}
	return -1; 
}

/* vi delete current line */
/* mem = memory, off = offset, */
static inline __attribute__((always_inline, hot))
off_t vi_dcl(const char *mem, off_t off) {
	/* 
 	calculate decrement until newline and
 	increment, store those two values as off_t
  	buffer+offt1 = start of line
   	buffer+offt2 = end of line
	delete from start of line to end
 	shift memory back where it belongs
 	*/
}
