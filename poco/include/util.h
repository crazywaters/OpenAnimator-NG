#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

static inline char *clone_string(char *s) {
	if (!s) return NULL;
	size_t len = strlen(s) + 1;
	char *out = (char*)malloc(len);
	if (!out) return NULL;
	memcpy(out, s, len);
	return out;
}

static inline void upc(char *s) {
	if (!s) return;
	for (; *s; ++s) {
		unsigned char c = (unsigned char)*s;
		if (c >= 'a' && c <= 'z') *s = (char)(c - 'a' + 'A');
	}
}

#endif /* UTIL_H */
#ifndef POCO_UTIL_H
#define POCO_UTIL_H

#ifndef ERRTEXT_SIZE
#define ERRTEXT_SIZE 128
#endif

#endif /* POCO_UTIL_H */


