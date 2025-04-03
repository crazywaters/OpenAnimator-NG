/*****************************************************************************
 * sdl_bmp.c - picdriver for loading BMP files through SDL_Image
 ****************************************************************************/

/*----------------------------------------------------------------------------
 * include the usual header files...
 *--------------------------------------------------------------------------*/

#include <SDL3/SDL.h>
#include <SDL3_image/sdl_image.h>
#include <memory.h>
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
 * forward declarations
 *--------------------------------------------------------------------------*/

extern Errcode pdr_boxfmt(char *fmt, ...);

/*----------------------------------------------------------------------------
 * for debugging, the boxf() function displays text using printf-style input,
 * and waits for a keyhit.
 *--------------------------------------------------------------------------*/

#undef debug
#if 1
#define debug(...) pdr_boxfmt(__VA_ARGS__)
#else
#define debug(...)
#endif


/*----------------------------------------------------------------------------
 * BMP_file structure, our extension to host's Image_file.
 *--------------------------------------------------------------------------*/
typedef struct bmp_file {
	Image_file hdr; /* PJ Image_file, must be first in struct! */
	SDL_Surface *surface;
	int width;
	int height;
	int depth;
	char *path[PATH_MAX];
} BMP_file;


/*****************************************************************************
 * as long as it's one frame of 8 bits-per-pixel, we can do it as requested.
 * if the host has asked for anything else, force the data in the Anim_info
 * to match what we can do, and return FALSE to indicate we tweaked it.
 ****************************************************************************/
static bool spec_best_fit(Anim_info *ainfo)
{
	debug("In spec_best_fit()...\n");

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
	BMP_file *sf;

	debug("In close_file()...\n");

	if (psf == NULL || *psf == NULL) { /* these could be NULL if we get   */
		return;                        /* called by our own error cleanup.*/
	} else {
		sf = (BMP_file *)*psf; /* retrieve and recast to our type */
	}

	if (sf->surface != NULL) { /* if file is open, 			   */
		SDL_DestroySurface(sf->surface);
		sf->surface = NULL;
	}

	free(sf);    /* free our control structure	   */
	*psf = NULL; /* indicate it's now free          */
	return;
}

/*****************************************************************************
 * allocate main data structure, open file.
 ****************************************************************************/
static Errcode alloc_and_open(BMP_file **psf, char *path, char *openmode)
{
	(void)path;
	(void)openmode;
	BMP_file *sf = (BMP_file *)calloc(1, sizeof(BMP_file));

	/*------------------------------------------------------------------------
	 * obtain some storage for our control structure, punt on error.
	 *----------------------------------------------------------------------*/

	if (sf == NULL) {
		return Err_no_memory;
	}

	*psf = sf;

	/*------------------------------------------------------------------------
	 * open the file, if open fails, return error code, else return success.
	 *----------------------------------------------------------------------*/

	return Success;
}

/*****************************************************************************
 * Open up the file, verify file header.
 ****************************************************************************/
static Errcode open_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo)
{
	Errcode err;
	BMP_file *sf;

	debug("In open_file()...\nFile=%s\n", path);

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

	sf->surface = SDL_LoadBMP(path);

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
	Errcode err;
	BMP_file *sf = (BMP_file *)ifile;

	debug("In read_first()...\n");

	if (sf->depth > 8) { /* we don't do rgb via this routine */
		return Err_rgb_convert;
	}

	/*	pj_set_rast(screen, 0); 			*/ /* clear the screen */
	/*	err = read_the_picture(sf, screen); */ /* load the picture */

	return err;
}

/*****************************************************************************
 * Since we only have one frame this routine is pretty trivial.
 ****************************************************************************/
static Errcode read_next(Image_file *ifile, Rcel *screen)
{
	debug("In read_next()...\n");
	return (Success);
}

/*****************************************************************************
 * this routine sets internal control information such that the next call
 * to rgb_readline() will obtain the first line of data for the image.
 *
 * the return value is 0 (Success) if image is rightsideup in the file,
 * and 1 if the image is upside down.  this routine may be called any number
 * of times during the process of reading an rgb file. it is generally called
 * twice, because it generally requires two passes for an rgb-aware host to
 * completely process the image data.  (first pass builds color map, second
 * pass loads data to screen while fitting it to the map.)
 ****************************************************************************/
Errcode rgb_seekstart(Image_file *pif)
{
	BMP_file *sf = (BMP_file *)pif;

	debug("In rgb_seekstart()...\n");

	/*	fseek(sf->file, sf->data_offset, SEEK_SET); */ /* seek to data in file */

	return Success;
}

/*****************************************************************************
 * read the next line of rgb data and return status of read.
 *
 * generally speaking, the host will call this routine as many times as
 * there are lines in the file (based on ainfo->height set by the open_file()
 * routine).  It's best, of course, to check for EOF explicitly in your reader
 * and return Err_truncated if the host attempts to read too many lines.
 ****************************************************************************/
static Errcode rgb_readline(Image_file *pif, Rgb3 *linebuf)
{
	BMP_file *sf = (BMP_file *)pif;

	debug("In rgb_readline()...\n");

	/*	return read_some_rgb_data(sf, linebuf); */

	return Success;
}

/*****************************************************************************
 * create an output file (alloc, open, and write file header).
 ****************************************************************************/
static Errcode create_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo)
{
	Errcode err;
	BMP_file *sf;

	debug("In create_file()...\nFile=%s", path);

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

	sf->width = ainfo->width;
	sf->height = ainfo->height;
	strncpy(sf->path, path, PATH_MAX);

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
	Errcode err = Success;
	BMP_file *sf = (BMP_file *)ifile;

	debug("In save_frames()...\n");

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
		palette->colors[c].a = 0xFF;
	}

	SDL_SetSurfacePalette(sf->surface, palette);
	SDL_DestroyPalette(palette);
	palette = NULL;

	/*
	 * Write Image
	 */

	if (!SDL_SaveBMP(sf->surface, sf->path)) {
		SDL_DestroySurface(sf->surface);
		close_file((Image_file **)&sf);
		return Err_file_access;
	}

	SDL_DestroySurface(sf->surface);
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
	read_next,        /* (*read_delta_next)() */
	save_frames,      /* (*save_frames)() */
	NULL,     /* pointer to options structure */
	rgb_seekstart,    /* (*rgb_seekstart)() */
	rgb_readline,     /* (*rgb_readline() */
};


Local_pdr bmp_local_pdr = {
	NULL,
	"BMP.SDL",
	&bmp_header,
};
