#include <vi_core.h>

/* vi insert n bytes after offset */
/* mm1 = memory 1, m1s = memory 1 size, mm2 = memory 2, m2s = memory 2 size, off = offset */
static inline __attribute__((always_inline, hot))
ssize_t vi_insert(char *mm1, size_t m1s, char *mm2, size_t m2s, size_t off) {
	vi_runc(!mm1 || !mm2, -1);
	vi_runc(off >= m1s, -1);
	vi_runc(m2s > m1s-off, -1);

	vi_memmov(mm1+off+m2s, mm1+off, m1s-off);
	vi_memcpy(mm1+off, mm2, m2s);
	
	return m1s+m2s;
}

/* vi delete n bytes after offset */
/* mem = memory, msz = memory size, off = offset, byt = N bytes */
static inline __attribute__((always_inline, hot))
ssize_t vi_delete(char *mem, size_t msz, size_t off, size_t byt) {
	vi_runc(!mem, -1);
	vi_runc(off >= msz, -1);
	vi_runc(byt > msz-off, -1);

	vi_memmov(mem+off, mem+off+byt, msz-off-byt);
	return msz-byt;
}

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

	vif->fem = vif->sts.st_size;
	vif->frm = vif->sts.st_size;

	vif->fim = malloc(vif->frm);
	if (!vif->fim) {
		vi_errors("malloc");
		close(vif->fid);
		return -1;
	}

	ssize_t n = read(vif->fid, vif->fim, vif->frm);
	if (n != vif->frm) {
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

int vi_revif(struct vi_file *vif, ssize_t del) {
	vi_runc(!vif->fim, -1);
	vi_runc(vif->fem+del<vif->frm, 0);

	size_t nsz = vif->frm ? vif->frm : 64;
	while (nsz < vif->fem+del) {
		nsz += nsz;
	}

	char *tmp = realloc(vif->fim, nsz);
	if (!tmp) return -1;

	vif->frm = nsz;
	vif->fim = tmp;

	return 0;
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

/* vi check if printable character */
/* cha = char */
static inline __attribute__((always_inline, hot))
unsigned char vi_cip(char cha) {
	if (cha >= 32 || cha <= 126) {
		return 1; 
	}
	switch (cha) {
		case 0x0A: return 1;
		case 0x09: return 1;
		default: return 0;
	}
	return 0;
}
