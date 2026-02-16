#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "poco_errcodes.h"
#include "pjhost.h"
#include "poco.h"
#include "pocolib.h"
#include "ptrmacro.h"

extern Errcode builtin_err;

/*****************************************************************************/
 static char* strlwr(char* s) {
 for(char *p=s; *p; p++)
 *p=tolower(*p);
 return s;
}

static char* strupr(char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}



/*****************************************************************************
 * int sprintf(char *buf, char *format, ...)
 ****************************************************************************/
static int po_sprintf(char* buf, char* format, ...)
{
	int rv;
	va_list args;

	if (buf == NULL || format == NULL)
		return (builtin_err = Err_null_ref);

	va_start(args, format);
	rv = vsprintf(buf, format, args);
	va_end(args);

	return rv;
}

/*****************************************************************************
 * int strcmp(char *a, char *b)
 ****************************************************************************/
static int po_strcmp(char* d, char* s)
{
	if (d == NULL || s == NULL)
		return (builtin_err = Err_null_ref);
	return (strcmp(d, s));
}

/*****************************************************************************
 * int stricmp(char *a, char *b)
 ****************************************************************************/
static int po_stricmp(char* d, char* s)
{
	if (d == NULL || s == NULL)
		return (builtin_err = Err_null_ref);
	return (stricmp(d, s));
}

/*****************************************************************************
 * int strncmp(char *a, char *b, int maxlen)
 ****************************************************************************/
static int po_strncmp(char* d, char* s, int maxlen)
{
	if (d == NULL || s == NULL)
		return (builtin_err = Err_null_ref);
	return (strncmp(d, s, maxlen));
}

/*****************************************************************************
 * int strlen(char *a)
 ****************************************************************************/
static int po_strlen(char* s)
{
	if (s == NULL)
		return (builtin_err = Err_null_ref);
	return (strlen(s));
}

/*****************************************************************************
 * char *strcpy(char *dest, char *source)
 ****************************************************************************/
static char* po_strcpy(char* d, char* s)
{

	if (d == NULL || s == NULL)
		builtin_err = Err_null_ref;
	else
		strcpy(d, s);

	return (d);
}

/*****************************************************************************
 * char *strncpy(char *dest, char *source, int maxlen)
 ****************************************************************************/
static char* po_strncpy(char* d, char* s, int maxlen)
{
	if (d == NULL || s == NULL)
		builtin_err = Err_null_ref;
	else {
		strncpy(d, s, maxlen);
	}
	return (d);
}

/*****************************************************************************
 * char *strcat(char *dest, char *source)
 ****************************************************************************/
static char* po_strcat(char* d, char* tail)
{
	if (d == NULL || tail == NULL)
		builtin_err = Err_null_ref;
	else
		strcat(d, tail);

	return (d);
}

/*****************************************************************************
 * char *strdup(char *source)
 ****************************************************************************/
static char* po_strdup(char* s)
{
	Popot d;

	if (s == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	int len = strlen(s) + 1;
	d = poco_lmalloc(len);
	if (d.pt == NULL) {
		builtin_err = Err_no_memory;
		return NULL;
	}
	strcpy(d.pt, s);
	return (char*)d.pt;
}

/*****************************************************************************
 * char *strchr(char *source, int c)
 ****************************************************************************/
static char* po_strchr(char* s1, int c)
{
	if (s1 == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	return strchr(s1, c);
}

/*****************************************************************************
 * char *strrchr(char *source, int c)
 ****************************************************************************/
static char* po_strrchr(char* s1, int c)
{
	if (s1 == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	return strrchr(s1, c);
}

/*****************************************************************************
 * char *strstr(char *string, char *substring)
 ****************************************************************************/
static char* po_strstr(char* s1, char* s2)
{

	if (s1 == NULL || s2 == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	return strstr(s1, s2);
}

/*****************************************************************************
 * char *stristr(char *string, char *substring)
 ****************************************************************************/
static char* po_stristr(char* s1, char* s2)
{
	char *p1 = NULL, *p2 = NULL;
	char* res;
	char* result = NULL;

	if (s1 == NULL || s2 == NULL) {
		builtin_err = Err_null_ref;
		goto OUT;
	}
	if ((p1 = clone_string(s1)) == NULL) {
		builtin_err = Err_no_memory;
		goto OUT;
	}
	if ((p2 = clone_string(s2)) == NULL) {
		builtin_err = Err_no_memory;
		goto OUT;
	}
	upc(p1);
	upc(p2);
	if ((res = strstr(p1, p2)) == NULL)
		goto OUT;
	result = s1 + (res - p1);
OUT:
	pj_gentle_free(p1);
	pj_gentle_free(p2);
	return result;
}

/*****************************************************************************
 * int atoi(char *str);
 ****************************************************************************/
static int po_atoi(char* str)
{
	if (str == NULL)
		return builtin_err = Err_null_ref;

	return atoi(str);
}

/*****************************************************************************
 * double atof(char *str);
 ****************************************************************************/
static double po_atof(char* str)
{
	if (str == NULL)
		return builtin_err = Err_null_ref;

	return atof(str);
}

/*****************************************************************************
 * int strspn(char *string, char *charset)
 ****************************************************************************/
static int po_strspn(char* s1, char* s2)
{

	if (s1 == NULL || s2 == NULL) {
		return builtin_err = Err_null_ref;
	}
	return strspn(s1, s2);
}

/*****************************************************************************
 * int strcspn(char *string, char *charset)
 ****************************************************************************/
static int po_strcspn(char* s1, char* s2)
{

	if (s1 == NULL || s2 == NULL) {
		return builtin_err = Err_null_ref;
	}
	return strcspn(s1, s2);
}

/*****************************************************************************
 * char *strpbrk(char *string, char *breakset)
 ****************************************************************************/
static char* po_strpbrk(char* s1, char* s2)
{

	if (s1 == NULL || s2 == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	return strpbrk(s1, s2);
}

/*****************************************************************************
 * char *strtok(char *string, char *delimset)
 ****************************************************************************/
static char* po_strtok(char* s1, char* s2)
{

	if (s2 == NULL) /* note that NULL s1 is allowed! */
	{
		builtin_err = Err_null_ref;
		return NULL;
	}
	return strtok(s1, s2);
}

/*****************************************************************************
 * char *getenv(char *varname)
 ****************************************************************************/
static char* po_getenv(char* s1)
{

	if (s1 == NULL) {
		builtin_err = Err_null_ref;
		return NULL;
	}
	return getenv(s1);
}

/*****************************************************************************
 * char *strlwr(char *string)
 ****************************************************************************/
static char* po_strlwr(char* d)
{

	if (d == NULL)
		builtin_err = Err_null_ref;
	else
		strlwr(d);
	return (d);
}

/*****************************************************************************
 * char *strupr(char *string)
 ****************************************************************************/
static char* po_strupr(char* d)
{

	if (d == NULL)
		builtin_err = Err_null_ref;
	else
		strupr(d);
	return (d);
}

/*****************************************************************************
 * char *strerror(int errnum)
 ****************************************************************************/
static char* po_strerror(int err)
{
	static char errmsg[ERRTEXT_SIZE];

	get_errtext(err, errmsg);
	return errmsg;
}

/*----------------------------------------------------------------------------
 * library protos...
 *
 * Maintenance notes:
 *	The stringent rules that apply to maintaining most builtin lib protos
 *	don't apply here.  These functions are not visible to poe modules;
 *	you can add or delete protos anywhere in the list below.
 *--------------------------------------------------------------------------*/

static Lib_proto lib[] = {
	/* string stuff */
	{ po_sprintf, "int     sprintf(char *buf, char *format, ...);" },
	{ po_strcmp, "int     strcmp(char *a, char *b);" },
	{ po_stricmp, "int     stricmp(char *a, char *b);" },
	{ po_strncmp, "int     strncmp(char *a, char *b, int maxlen);" },
	{ po_strlen, "int     strlen(char *a);" },
	{ po_strcpy, "char    *strcpy(char *dest, char *source);" },
	{ po_strncpy, "char    *strncpy(char *dest, char *source, int maxlen);" },
	{ po_strcat, "char    *strcat(char *dest, char *source);" },
	{ po_strdup, "char    *strdup(char *source);" },
	{ po_strchr, "char    *strchr(char *source, int c);" },
	{ po_strrchr, "char    *strrchr(char *source, int c);" },
	{ po_strstr, "char    *strstr(char *string, char *substring);" },
	{ po_stristr, "char    *stristr(char *string, char *substring);" },
	{ po_atoi, "int     atoi(char *string);" },
	{ po_atof, "double  atof(char *string);" },
	{ po_strpbrk, "char    *strpbrk(char *string, char *breakset);" },
	{ po_strspn, "int     strspn(char *string, char *breakset);" },
	{ po_strcspn, "int     strcspn(char *string, char *breakset);" },
	{ po_strtok, "char    *strtok(char *string, char *delimset);" },
	{ po_getenv, "char    *getenv(char *varname);" },
	{ po_strlwr, "char    *strlwr(char *string);" },
	{ po_strupr, "char    *strupr(char *string);" },
	{ po_strerror, "char    *strerror(int errnum);" },
};

Poco_lib po_str_lib = {
	NULL,
	"(C standard) String",
	lib,
	Array_els(lib),
};
