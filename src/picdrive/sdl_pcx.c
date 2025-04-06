/*****************************************************************************
* sdl_bmp.c - picdriver for loading BMP files through SDL_Image
 ****************************************************************************/

/*----------------------------------------------------------------------------
 * include the usual header files...
 *--------------------------------------------------------------------------*/

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assert.h>
#include <memory.h>
#include <reqlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl_pdr.h"

// ----------------------------------------------------------------------------
static bool sdlpdr_save_pcx(SDL_Surface *surface, const char* path) {


	return false;
}


/*----------------------------------------------------------------------------
 * Setup rex & pdr interface structures...
 *--------------------------------------------------------------------------*/
char pcx_long_description[] =
	"SDL_Image Loader: PCX Files\n\n"
	"All PCX files are saved as 8-bit.";


SDL_PDR_CREATE(PCX,
			   "PCX Format (SDL)",
			   pcx_long_description,
			   "pcx",
			   IMG_Load,
			   sdlpdr_save_pcx);

