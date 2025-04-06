#ifndef __SDL_PDR_H
#define __SDL_PDR_H

#include "animinfo.h" /* need gfx to access the screen */
#include "cmap.h"     /* need gfx to access the screen */
#include "errcodes.h" /* most PDR modules will need error codes info  */
#include "gfx.h"      /* need gfx to access the screen */
#include "picdrive.h" /* required header file */
#include "pj_sdl.h"

// from sdl_pdr.c
typedef struct pdr_sdlfile PDR_SdlFile;

bool sdlpdr_spec_best_fit(Anim_info *ainfo);
void sdlpdr_close_file(Image_file **psf);
Errcode sdlpdr_alloc_and_open(PDR_SdlFile **psf, char *path, char *openmode);
Errcode sdlpdr_open_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo);
Errcode sdlpdr_read_first(Image_file *ifile, Rcel *screen);
Errcode sdlpdr_create_file(Pdr *pd, char *path, Image_file **pif, Anim_info *ainfo);
Errcode sdlpdr_save_frames(Image_file *ifile, Rcel *screen, int num_frames,
						   Errcode (*seek_frame)(int ix, void *seek_data), void *seek_data,
						   Rcel *work_screen);

// from sdl_convert.c
SDL_Surface* sdlpdr_convert_colors(SDL_Surface* surface, size_t num_colors);


/*
 * SDLPDR Macro
 * Quick way to define the Pdr structure for each image type driver
 */

// ID, name, long description, suffixes, loader, saver
#define SDL_PDR_CREATE(ID, name, long_desc, suffix, loader, saver)      \
	Pdr ID##_header = {                                                 \
		{                                                               \
			REX_PICDRIVER,                                              \
			PDR_VERSION,                                                \
			NOFUNC,                                                     \
			NOFUNC,                                                     \
			NULL,                                                       \
			NULL,                                                       \
			NULL,                                                       \
		},                                                              \
                                                                        \
		/*                                                              \
		 * !IMPORTANT                                                   \
		 * If you don't pass a string literal here, it overwrites weird \
		 * stuff past the memory of the config file.                    \
		 */                                                             \
                                                                        \
		name,      /* title_info */                                     \
		long_desc, /* long_info */                                      \
                                                                        \
		/*                                                              \
		 * Suffixes in SDL use a specific format:                       \
		 * - suffix characters                                          \
		 * - no '.' character                                           \
		 * - separate multiple extensions with ';'                      \
		 */                                                             \
                                                                        \
		suffix, /* default_suffi */                                     \
		1,                                                              \
		1,                    /* max_write_frames, max_read_frames */   \
		sdlpdr_spec_best_fit, /* (*spec_best_fit)() */                  \
		sdlpdr_create_file,   /* (*create_image_file)() */              \
		sdlpdr_open_file,     /* (*open_image_file)() */                \
		sdlpdr_close_file,    /* (*close_image_file)() */               \
		sdlpdr_read_first,    /* (*read_first_frame)() */               \
		NULL,                 /* (*read_delta_next)() */                \
		sdlpdr_save_frames,   /* (*save_frames)() */                    \
		NULL,                 /* pointer to options structure */        \
		NULL,                 /* (*rgb_seekstart)() */                  \
		NULL,                 /* (*rgb_readline() */                    \
                                                                        \
		loader,                                                         \
		saver,                                                          \
                                                                        \
		{0},                                                            \
	};                                                                  \
                                                                        \
	Local_pdr ID##_local_pdr = {                                        \
		NULL,                                                           \
		#ID ".SDL",                                                     \
		&ID##_header,                                                   \
	};

#endif  // __SDL_PDR_H