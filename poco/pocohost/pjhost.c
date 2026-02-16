#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "pjhost.h"

/* Memory helpers used by Poco and libpoco */
POCOHOST_EXPORT void* pj_malloc(size_t i)
{
    return malloc(i);
}

POCOHOST_EXPORT void* pj_zalloc(size_t size)
{
    void *p = malloc(size);
    if (p != NULL) {
        memset(p, 0, size);
    }
    return p;
}

POCOHOST_EXPORT void pj_free(void* v)
{
    free(v);
}

POCOHOST_EXPORT void pj_gentle_free(void* p)
{
    if (p != NULL) {
        free(p);
    }
}

POCOHOST_EXPORT void pj_freez(void* p)
{
    if (p != NULL && *(void**)p != NULL) {
        free(*(void**)p);
        *(void**)p = NULL;
    }
}

/* Minimal file helper some Poco paths rely on */
POCOHOST_EXPORT int pj_delete(const char *name)
{
    return remove(name);
}

/* Optional error query used by some call sites; return Success in CLI host */
POCOHOST_EXPORT Errcode pj_ioerr(void)
{
    return Success;
}

/* String utility helpers used by Poco */
POCOHOST_EXPORT void upc(char *s)
{
    if (!s) return;
    for (; *s; ++s) {
        *s = (char)toupper((unsigned char)*s);
    }
}

POCOHOST_EXPORT char *clone_string(const char *s)
{
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *d = (char*)malloc(len);
    if (!d) return NULL;
    memcpy(d, s, len);
    return d;
}

/* File initialization stubs */
POCOHOST_EXPORT void init_stdfiles(void) { }
POCOHOST_EXPORT void cleanup_lfiles(void) { }


