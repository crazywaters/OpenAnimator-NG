/* Consolidated single copy */
#ifndef POCOLIB_H
#define POCOLIB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef LINKLIST_H
#include "linklist.h"
#endif

#ifndef STDTYPES_H
#include "stdtypes.h"
#endif
#ifndef POCO_ERRCODES_H
#include "poco_errcodes.h"
#endif

#include <stdarg.h>

typedef struct popot /* The poco bounds-checked pointer type */
{
	void* pt;
	void* min;
	void* max;
} Popot;

extern Popot empty_popot;

typedef struct string_ref
{
	Dlnode node;
	int ref_count;
	Popot string;
} String_ref;

typedef String_ref* PoString;

#define PoStringBuf(s) ((*(s))->string.pt)

typedef union pt_num /* Overlap popular datatypes in the same space */
{
	int i;
	int inty;
	short s;
	UBYTE* bpt;
	char c;
	long l;
	ULONG ul;
	float f;
	double d;
	void* p;
	int doff;    /* data offset */
	int (*func)(); /* code pointer */
	Popot ppt;
	PoString postring;
} Pt_num;

typedef struct lib_proto /* Poco library prototype lines */
{
	void* func;
	char* proto;
} Lib_proto;

typedef struct poco_lib /* Poco library main control structure */
{
	struct poco_lib* next;
	char* name;
	Lib_proto* lib;
	int count;
	Errcode (*init)(struct poco_lib* lib);
	void (*cleanup)(struct poco_lib* lib);
	void* local_data;
	Dlheader resources;
	void* rexhead;
	char reserved[12];
} Poco_lib;

#ifndef RNODE_FIELDS
#define RNODE_FIELDS Dlnode node; void* resource
#endif

typedef struct rnode /* Used for resource tracking in builtin libs */
{
	RNODE_FIELDS
} Rnode;

Errcode po_check_formatf(int maxlen, int vargcount, int vargsize, char* fmt, va_list pargs);
Errcode po_init_libs(Poco_lib* lib);
void po_cleanup_libs(Poco_lib* lib);
void po_free(void* pt);
void* po_malloc(int size);
void* po_calloc(int size_el, int el_count);
Popot poco_lmalloc(long size);
void poco_freez(Popot* pt);

#define Array_els(array) (sizeof(array)/sizeof((array)[0]))
#define Popot_bufsize(p) ((size_t)((char*)((p)->max) - (char*)((p)->pt) + 1))
#define Popot_make_null(p) ((p)->pt = (p)->min = (p)->max = NULL)

#endif /* POCOLIB_H */


