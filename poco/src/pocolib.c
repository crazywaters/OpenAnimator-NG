/*****************************************************************************
 * pocolib.c - Common routines for other poco library functions.
 *
 * 05/15/92 (Ian)
 *			Tweaked po_check_formatf() so that a NULL pointer passed for
 *			a %p arg doesn't cause an abort -- it's reasonable to print
 *			the value of a NULL or wild pointer, and printing it won't
 *			cause it to be dereferenced.
 ****************************************************************************/


#include "poco.h"
#include "poco_errcodes.h"
#include "ptrmacro.h"
#include "pocolib.h"
#include <stdarg.h>
#include "poco_port.h"

extern Errcode builtin_err;

Popot empty_popot = { NULL, NULL, NULL };

Errcode po_init_libs(Poco_lib *lib)
/*****************************************************************************
 *
 ****************************************************************************/
{
	Errcode err;

	while (lib != NULL)
		{
		init_list(&lib->resources);
		if (lib->init)
			if (Success > (err = lib->init(lib)))
				return err;
		lib = lib->next;
		}
	return(Success);
}

void po_cleanup_libs(Poco_lib *lib)
/*****************************************************************************
 *
 ****************************************************************************/
{
	while (lib != NULL)
		{
		if (lib->cleanup)
			lib->cleanup(lib);
		lib = lib->next;
		}
}

void poco_freez(Popot *pt)
/*****************************************************************************
 * free buffer allocated into Poco space and set it to NULL
 ****************************************************************************/
{
if (pt->pt != NULL)
	po_free(pt->pt);
pt->pt = pt->min =	pt->max = NULL;
}

Errcode po_check_formatf(int maxlen, int vargcount, int vargsize,
						 char *fmt, va_list pargs)
/****************************************************************************
 * sanity-check the printf-like format & args passed in from a poco program.
 ****************************************************************************/
{
	(void)maxlen;
	(void)vargcount;
	(void)vargsize;
	(void)fmt;
	(void)pargs;
	return Success;
}
