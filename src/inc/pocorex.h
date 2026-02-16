/*****************************************************************************
 * POCOREX.H - Header file defining structures used by POE loadable modules.
 ****************************************************************************/

#ifndef POCOREX_H
#define POCOREX_H

#ifndef STDTYPES_H
	#include "stdtypes.h"
#endif

#ifndef POCOLIB_H
	#include "pocolib.h"
#endif

/*****************************************************************************
 * Platform-specific symbol export macro
 * POE modules should use this macro to export the poco_rexlib_get() function
 ****************************************************************************/

#if defined(_WIN32)
#define POCO_EXPORT __declspec(dllexport)
#else
#define POCO_EXPORT __attribute__((visibility("default")))
#endif

/*****************************************************************************
 * Poco loadable library header structure
 * This minimal structure replaces the Rexlib header for Poco modules.
 * It contains only the fields needed for Poco library loading and initialization.
 ****************************************************************************/

typedef struct pocorex_hdr {
	USHORT version;		/* library version, must match POCOREX_VERSION */
	EFUNC init;		/* optional initialization function, called after library is loaded */
	VFUNC cleanup;		/* optional cleanup function, called before library is unloaded */
	char *id_string;	/* optional identifier string for validation */
} Pocorex_hdr;

/*****************************************************************************
 * Poco loadable library data block
 * This structure contains the header and the Poco library definition.
 * POE modules should export a function 'poco_rexlib_get()' that returns
 * a pointer to a statically allocated Pocorex structure.
 ****************************************************************************/

#define POCOREX_VERSION 	200

typedef struct pocorex {
	Pocorex_hdr hdr;		/* poco library header */
	Poco_lib lib;			/* Poco lib control struct, see pocolib.h */
} Pocorex;

/*****************************************************************************
 * Entry point function type for POE modules
 * Each POE module must export a function with this signature:
 *   Pocorex* poco_rexlib_get(void);
 * This function should return a pointer to a statically allocated Pocorex
 * structure containing the library definition.
 ****************************************************************************/

typedef Pocorex* (*Poco_rexlib_get_func)(void);

/*****************************************************************************
 * The following macro provides the easiest way to set up your Pocorex data
 * block from within a POE source module.  After the prototypes have been
 * defined in the Lib_proto array, code a Setup_Pocorex macro.
 *
 * Example:
 *
 *	 Lib_proto lib_calls[] = {
 *		{libfunc1, "void func1(void);"},
 *		{libfunc2, "int *func2(int a);"},
 *	  };
 *
 *	 Setup_Pocorex(get_screen, free_screen, "My Poco Lib", lib_calls);
 *
 *	 Pocorex* poco_rexlib_get(void) {
 *		 return &rexlib_header;
 *	 }
 *
 * Parameters:
 *   init: Optional initialization function (EFUNC), or NULL
 *   cleanup: Optional cleanup function (VFUNC), or NULL  
 *   libname: Library name string
 *   libprotos: Array of Lib_proto structures defining exported functions
 *
 * Note: The library must export poco_rexlib_get() as a public symbol.
 *****************************************************************************/

/* kiki note:
 * I extended this macro to add the poco_rexlib_get function at the end.
 * This is the new method for getting access to the libprotos from the
 * compiled binary; the old way was very DOS-specific, and this should be
 * more portable.  Adding the function to the macro should simplify porting
 * forward old modules, without needing to add that extra bit.
 */

#define Setup_Pocorex(init, cleanup, libname, libprotos) \
 static char _l_name[] = libname;\
 Pocorex rexlib_header = { \
   {POCOREX_VERSION, init, cleanup, _l_name}, \
   {NULL, _l_name, libprotos, (sizeof(libprotos)/sizeof(libprotos[0]))} \
 };\
 \
POCO_EXPORT Pocorex* poco_rexlib_get(void) \
{ \
	return &rexlib_header; \
}

#endif /* POCOREX_H */

