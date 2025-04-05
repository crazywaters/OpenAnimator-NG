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

#include "animinfo.h" /* need gfx to access the screen */
#include "cmap.h"     /* need gfx to access the screen */
#include "errcodes.h" /* most PDR modules will need error codes info  */
#include "gfx.h"      /* need gfx to access the screen */
#include "picdrive.h" /* required header file */
#include "pj_sdl.h"


/*----------------------------------------------------------------------------
 * PDR_SdlFile structure, our extension to host's Image_file.
 * Since all the file IO is handled through SDL_image, I'm trying
 * to keep the bulk of what's here generic.  Instead of doing picdriver
 * code per format, I've written functions that take pointers to the IMG_*
 * functions.
 *--------------------------------------------------------------------------*/
typedef struct pdr_sdlfile {
	Image_file hdr; /* PJ Image_file, must be first in struct! */
	SDL_Surface *surface;
	int width;
	int height;
	int depth;
	char *path[PATH_MAX];
	bool (*sdl_save_image)(SDL_Surface* surface, char* path);
	SDL_Surface*  (*sdl_load_image)(char* path);
} PDR_SdlFile;


/*****************************************************************************
 * as long as it's one frame of 8 bits-per-pixel, we can do it as requested.
 * if the host has asked for anything else, force the data in the Anim_info
 * to match what we can do, and return FALSE to indicate we tweaked it.
 ****************************************************************************/
static bool spec_best_fit(Anim_info *ainfo)
{
	if (ainfo->depth == 8 && ainfo->num_frames == 1) {
		return true;
	}

	ainfo->depth = 8;
	ainfo->num_frames = 1;
	return false;
}

/*****************************************************************************
 * Clean up resources used by picture driver.
 ****************************************************************************/
static void close_file(Image_file **psf)
{
	PDR_SdlFile *sf;

	if (psf == NULL || *psf == NULL) { /* these could be NULL if we get   */
		return;                        /* called by our own error cleanup.*/
	} else {
		sf = (PDR_SdlFile *)*psf; /* retrieve and recast to our type */
	}

	if (sf->surface != NULL) { /* if file is open, 			   */
		SDL_DestroySurface(sf->surface);
		sf->surface = NULL;
	}

	free(sf);    /* free our control structure	   */
	*psf = NULL; /* indicate it's now free          */
}

/*****************************************************************************
 * allocate main data structure, open file.
 ****************************************************************************/
static Errcode alloc_and_open(PDR_SdlFile **psf, char *path, char *openmode)
{
	(void)path;
	(void)openmode;

	// Alloc storage, punt on error.
	PDR_SdlFile *sf = (PDR_SdlFile *)calloc(1, sizeof(PDR_SdlFile));

	if (sf == NULL) {
		return Err_no_memory;
	}

	// Copy over the full path for both reads and writes
	strncpy(sf->path, path, PATH_MAX);
	*psf = sf;

	/*
	 * kiki note:
	 * Old system would open a FILE* handle here for whatever was
	 * needed. We're not doing that anymore.
	 */

	return Success;
}


/*****************************************************************************
 * Open up the file, verify file header.
 ****************************************************************************/
static Errcode open_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo)
{
	Errcode err;
	PDR_SdlFile *sf;

	assert(pd->sdl_load_image);


	sf = NULL;   /* preset these to NULL in case we take our 	*/
	*pif = NULL; /* error exit path before everything's alloc'd. */

	/*------------------------------------------------------------------------
	 * allocate main data structure, open file.
	 *----------------------------------------------------------------------*/

	err = alloc_and_open(&sf, path, "rb");
	if (err != Success) {
		close_file((Image_file **)&sf);
		return err;
	}

	/*------------------------------------------------------------------------
	 * at this point, analyze the contents of the file to make sure it
	 * contains data in the format you expected.  there is no guarantee that
	 * the file is the type you expect -- in fact, quite often it won't be.
	 * if you don't recognize the file format, return Err_pic_unknown.
	 *----------------------------------------------------------------------*/

	if (sf->surface) {
		SDL_DestroySurface(sf->surface);
		sf->surface = NULL;
	}

	sf->surface = pd->sdl_load_image(path);

	if (!sf->surface) {
		close_file((Image_file **)&sf);
		return Err_pic_unknown;
	}

	sf->width = sf->surface->w;
	sf->height = sf->surface->h;
	sf->depth = SDL_BITSPERPIXEL(sf->surface->format);

	/*------------------------------------------------------------------------
	 * fill in values we return to the host via pointers.
	 *----------------------------------------------------------------------*/

	memset(ainfo, 0, sizeof(*ainfo));
	ainfo->width = sf->width;
	ainfo->height = sf->height;
	ainfo->depth = SDL_BITSPERPIXEL(sf->surface->format);
	ainfo->num_frames = 1;
	ainfo->millisec_per_frame = DEFAULT_AINFO_SPEED;

	*pif = (Image_file *)sf;
	return Success;
}


/*****************************************************************************
 * read in 1st (er, only) image.
 ****************************************************************************/
static Errcode read_first(Image_file *ifile, Rcel *screen)
{
	PDR_SdlFile *sf = (PDR_SdlFile *)ifile;
	assert(sf);
	assert(sf->surface);


	if (sf->depth > 8) { /* we don't do rgb via this routine yet */
		// !TODO!
		return Err_rgb_convert;
	}

	/* read pixels by index for now */

	if (!SDL_LockSurface(sf->surface)) {
		fprintf(stderr, "Unable to lock surface for file load: %s\n", SDL_GetError());
		return Err_no_lock;
	}

	uint8_t *surface_buf = (uint8_t *)sf->surface->pixels;
	uint8_t buf[sf->width];

	for (int y = 0; y < sf->height; y += 1) {
		for (int x = 0; x < sf->width; x += 1) {
			buf[x] = surface_buf[y * sf->surface->pitch + x];
		}
		pj_put_hseg(screen, buf, 0, y, sf->width);
	}

	SDL_UnlockSurface(sf->surface);

	/* copy cmap */
	SDL_Palette* pal = SDL_GetSurfacePalette(sf->surface);
	if (pal == NULL) {
		fprintf(stderr, "Unable to get palette from loaded image: %s\n", SDL_GetError());
		return Err_no_palette;
	}

	for (int index = 0; index < 256; index += 1) {
		screen->cmap->ctab[index].r = pal->colors[index].r;
		screen->cmap->ctab[index].g = pal->colors[index].g;
		screen->cmap->ctab[index].b = pal->colors[index].b;
	}

	return Success;
}


/*****************************************************************************
 * create an output file (alloc, open, and write file header).
 ****************************************************************************/
static Errcode create_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo)
{
	Errcode err;
	PDR_SdlFile *sf;

	assert(pd->sdl_save_image);


	sf = NULL;
	*pif = NULL;

	/*------------------------------------------------------------------------
	 * allocate main data structure, open file.
	 *----------------------------------------------------------------------*/

	err = alloc_and_open(&sf, path, "wb");
	if (err != Success) {
		close_file((Image_file **)&sf);
		return err;
	}

	/*------------------------------------------------------------------------
	 * save what we need out of anim_info, and return success...
	 *----------------------------------------------------------------------*/

	sf->sdl_save_image = pd->sdl_save_image;
	sf->width = ainfo->width;
	sf->height = ainfo->height;

	*pif = (Image_file *)sf;
	return Success;
}

/*****************************************************************************
 * save screen image.
 ****************************************************************************/
static Errcode save_frames(Image_file *ifile, Rcel *screen, int num_frames,
						   Errcode (*seek_frame)(int ix, void *seek_data), void *seek_data,
						   Rcel *work_screen)
{
	(void)num_frames;
	(void)seek_frame;
	(void)seek_data;
	(void)work_screen;

	PDR_SdlFile *sf = (PDR_SdlFile *)ifile;


	/*------------------------------------------------------------------------
	 * write the picture file from the input screen, using output options.
	 *----------------------------------------------------------------------*/

	int y;
	int w = sf->width;
	int h = sf->height;

	sf->surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_INDEX8);
	if (!sf->surface) {
		close_file((Image_file **)&sf);
		return Err_no_surface;
	}

	Pixel *buf = malloc(w + 2);
	if (!buf) {
		SDL_DestroySurface(sf->surface);
		close_file((Image_file **)&sf);
		return Err_no_memory;
	}

	if (!SDL_LockSurface(sf->surface)) {
		SDL_DestroySurface(sf->surface);
		close_file((Image_file **)&sf);
		return Err_no_lock;
	}

	uint8_t *surface_buf = (uint8_t *)sf->surface->pixels;

	for (y = 0; y < h; y += 1) {
		pj_get_hseg(screen, buf, 0, y, w);
		for (int x = 0; x < w; x += 1) {
			surface_buf[y * w + x] = buf[x];
		}
	}

	SDL_UnlockSurface(sf->surface);

	/*
	 * Copy over palette
	 */

	SDL_Palette *palette = SDL_CreatePalette(screen->cmap->num_colors);
	if (!palette) {
		SDL_DestroySurface(sf->surface);
		close_file((Image_file **)&sf);
		return Err_no_palette;
	}

	for (int c = 0; c < screen->cmap->num_colors; c += 1) {
		palette->colors[c].r = screen->cmap->ctab[c].r;
		palette->colors[c].g = screen->cmap->ctab[c].g;
		palette->colors[c].b = screen->cmap->ctab[c].b;

		// !TODO: export with alpha channel for color 0 for formats that support it?
		palette->colors[c].a = 0xFF;
	}

	SDL_SetSurfacePalette(sf->surface, palette);
	SDL_DestroyPalette(palette);
	palette = NULL;

	/*
	 * Write Image
	 */

	if (!sf->sdl_save_image(sf->surface, sf->path)) {
		close_file((Image_file **)&sf);
		soft_continu_box("!%s", "sdl_cant_write", sf->path);
		return Err_file_access;
	}

	return Success;
}

/*----------------------------------------------------------------------------
 * Setup rex & pdr interface structures...
 *--------------------------------------------------------------------------*/
char bmp_long_description[] =
	"SDL_Image Loader: BMP Files\n\n"
	"All BMP files are saved as 8-bit.";

Pdr bmp_header = {
	{
		REX_PICDRIVER,
		PDR_VERSION,
		NOFUNC,
		NOFUNC,
		NULL,
		NULL,
		NULL,
	},

	/*
	 * !IMPORTANT
	 * If you don't pass a string literal here, it overwrites weird
	 * stuff past the memory of the config file.
	 */

	"BMP Format (SDL)", /* title_info */
	bmp_long_description,   /* long_info */

	/*
	 * Suffixes in SDL use a specific format:
	 * - suffix characters
	 * - no '.' character
	 * - separate multiple extensions with ';'
	 */

	"bmp", /* default_suffi */
	1,
	1, /* max_write_frames, max_read_frames */
	spec_best_fit,    /* (*spec_best_fit)() */
	create_file,      /* (*create_image_file)() */
	open_file,        /* (*open_image_file)() */
	close_file,       /* (*close_image_file)() */
	read_first,       /* (*read_first_frame)() */
	NULL,        /* (*read_delta_next)() */
	save_frames,      /* (*save_frames)() */
	NULL,     /* pointer to options structure */
	NULL,    /* (*rgb_seekstart)() */
	NULL,     /* (*rgb_readline() */

	SDL_SaveBMP,
	SDL_LoadBMP,
};


Local_pdr bmp_local_pdr = {
	NULL,
	"BMP.SDL",
	&bmp_header,
};
