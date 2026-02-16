/*
 * Standalone DOS library for Poco: fnsplit and fnmerge.
 *
 * Provides real implementations for standalone poco (no ani host).
 * fnsplit delegates to trdfile; fnmerge is implemented here.
 * Works on Unix/macOS (no drive) and Windows (C:, D:, etc.).
 */

#include <stddef.h>
#include <string.h>

#if defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <limits.h>
#endif

#include "pocolib.h"
#include "poco_errcodes.h"

#ifndef PATH_SIZE
#if defined(PATH_MAX)
#define PATH_SIZE PATH_MAX
#else
#define PATH_SIZE 1024
#endif
#endif

/* Declare fnsplit from trdfile - implemented in src/fileio/fpnsplit.c */
extern int fnsplit(char* path, char* device, char* dir, char* file, char* suffix);

/*****************************************************************************
 * ErrCode fnsplit(char *path, char *device, char *dir, char *file, char *suf);
 *
 * Splits path into device, dir, file, suffix. Uses trdfile's fnsplit which
 * handles Unix (no drive), macOS, and Windows (C:, D:, etc.).
 ****************************************************************************/
static int po_fnsplit(char* path, char* device, char* dir, char* file, char* suffix)
{
	if (path == NULL || device == NULL || dir == NULL || file == NULL || suffix == NULL)
	{
		return Err_null_ref;
	}
	return fnsplit(path, device, dir, file, suffix);
}

/*****************************************************************************
 * ErrCode fnmerge(char *path, char *device, char *dir, char *file, char *suf);
 *
 * Merges device, dir, file, suffix into path. NULL components treated as "".
 * Validates total length < PATH_SIZE. Cross-platform.
 ****************************************************************************/
static int po_fnmerge(char* path, char* device, char* dir, char* file, char* suffix)
{
	size_t total_len;
	const char* dev_str = device ? device : "";
	const char* dir_str = dir ? dir : "";
	const char* file_str = file ? file : "";
	const char* suf_str = suffix ? suffix : "";

	if (path == NULL)
	{
		return Err_null_ref;
	}

	total_len = strlen(dev_str) + strlen(dir_str) + strlen(file_str) + strlen(suf_str);
	if (total_len >= PATH_SIZE)
	{
		return Err_dir_too_long;
	}

	strcpy(path, dev_str);
	strcat(path, dir_str);
	strcat(path, file_str);
	strcat(path, suf_str);
	return Success;
}


//*****************************************************************************

static Lib_proto dos_lib[] = {
	{(void*)po_fnsplit, "ErrCode fnsplit(char *path, char *device, char *dir, char *file, char *suf);"},
	{(void*)po_fnmerge, "ErrCode fnmerge(char *path, char *device, char *dir, char *file, char *suf);"},
};

Poco_lib po_dos_standalone_lib = {
	NULL,
	"DOS",
	dos_lib,
	sizeof(dos_lib) / sizeof(dos_lib[0]),
};
