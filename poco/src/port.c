#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "pjhost.h"

#ifndef USE_EXTERNAL_PJ_HOST
void *pj_malloc(size_t i) { return malloc(i); }
void *pj_zalloc(size_t size) { void *p = malloc(size); if (p) memset(p, 0, size); return p; }
void pj_free(void *v) { free(v); }
void pj_gentle_free(void *p) { if (p) free(p); }
void pj_freez(void *p) { if (p && *(void**)p) { free(*(void**)p); *(void**)p = NULL; } }
#endif

int pj_delete(const char *name) { return remove(name); }
void init_stdfiles(void) { }
void cleanup_lfiles(void) { }

void upc(char *s) {
	if (!s) return;
	for (; *s; ++s) {
		*s = (char)toupper((unsigned char)*s);
	}
}

char *clone_string(const char *s) {
	if (!s) return NULL;
	size_t len = strlen(s) + 1;
	char *d = (char*)malloc(len);
	if (!d) return NULL;
	memcpy(d, s, len);
	return d;
}


