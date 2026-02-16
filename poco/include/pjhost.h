#ifndef PJHOST_H
#define PJHOST_H

#include <stddef.h>
#include "poco_errcodes.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#define POCOHOST_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define POCOHOST_EXPORT __attribute__((visibility("default")))
#else
#define POCOHOST_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* General-purpose host helpers used by Poco/libpoco and host runtimes */

POCOHOST_EXPORT void* pj_malloc(size_t i);
POCOHOST_EXPORT void* pj_zalloc(size_t size);
POCOHOST_EXPORT void  pj_free(void* v);
POCOHOST_EXPORT void  pj_gentle_free(void* p);
POCOHOST_EXPORT void  pj_freez(void* p);

POCOHOST_EXPORT int   pj_delete(const char *name);

POCOHOST_EXPORT Errcode pj_ioerr(void);

/* String utilities */
POCOHOST_EXPORT void upc(char *s);
POCOHOST_EXPORT char *clone_string(const char *s);

/* File initialization */
POCOHOST_EXPORT void init_stdfiles(void);
POCOHOST_EXPORT void cleanup_lfiles(void);

#ifdef __cplusplus
}
#endif

#endif /* PJHOST_H */


