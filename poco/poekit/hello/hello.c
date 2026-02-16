/*****************************************************************************
 * hello.c - A simple hello world POE module demonstrating the new library
 *           loading system.
 ****************************************************************************/

/*----------------------------------------------------------------------------
 * include the usual header files...
 *--------------------------------------------------------------------------*/

#include "errcodes.h"   /* host error codes (must precede pocorex.h)     */
#include "pocorex.h"    /* required header file, also includes pocolib.h */
#include <stdio.h>      /* for printf */

/*----------------------------------------------------------------------------
 * your data and code goes here...
 *--------------------------------------------------------------------------*/

static void hello_func(void)
{
	printf("Hello from POE!\n");
}

/*----------------------------------------------------------------------------
 * Setup pocorex interface structures...
 *--------------------------------------------------------------------------*/

static Lib_proto poe_calls[] = {
	{ hello_func, "void HelloFunc(void);" },
};

Setup_Pocorex(NOFUNC, NOFUNC, "Hello POE", poe_calls);
