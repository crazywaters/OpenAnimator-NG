/* Single, clean header for POCO portability shims. */
#ifndef POCO_PORT_H
#define POCO_PORT_H

#include <stddef.h>

/* Memory wrappers */
void *pj_malloc(size_t i);
void *pj_zalloc(size_t size);
void pj_free(void *v);
void pj_gentle_free(void *p);
void pj_freez(void *p);

/* Filesystem and std files */
int pj_delete(const char *name);
void init_stdfiles(void);
void cleanup_lfiles(void);

/* String helpers */
void upc(char *s);
char *clone_string(const char *s);

#endif /* POCO_PORT_H */
