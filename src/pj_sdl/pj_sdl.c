//
// Created by Charles Wardlaw on 2022-10-02.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_surface.h>

#include "pj_sdl.h"


/*--------------------------------------------------------------*/
SDL_Surface*  s_surface		   = NULL;
SDL_Window*   window		   = NULL;
SDL_Surface*  s_window_surface = NULL;
SDL_Renderer* renderer		   = NULL;
SDL_Texture*  render_target    = NULL;
SDL_Palette*  vga_palette      = NULL;

// for the file requestors
static char last_path[PATH_MAX] = "";


/*--------------------------------------------------------------*/
int pj_sdl_get_video_size(LONG* width, LONG* height)
{
	if (!s_surface) {
		*width = -1;
		*height = -1;
		return 0;
	}

	*width = s_surface->w;
	*height = s_surface->h;
	return 1;
}

/*--------------------------------------------------------------*/
int pj_sdl_get_window_size(int* width, int* height)
{
	if (!window) {
		*width = -1;
		*height = -1;
		return 0;
	}

	SDL_GetWindowSize(window, width, height);
	return 1;
}

/*--------------------------------------------------------------*/
int pj_sdl_get_window_scale(float* x, float* y)
{
	LONG video_w, video_h;
	int window_w, window_h;

	*x = 1.0f;
	*y = 1.0f;

	if (!pj_sdl_get_video_size(&video_w, &video_h)) {
		fprintf(stderr, "Unable to get video size!\n");
		return 0;
	}

	if (!pj_sdl_get_window_size(&window_w, &window_h)) {
		fprintf(stderr, "Unable to get window size!\n");
		return 0;
	}

	float video_wf = video_w;
	float video_hf = video_h;
	float window_wf = window_w;
	float window_hf = window_h;

	*x = window_wf / video_wf;
	*y = window_hf / video_hf;

	return 1;
}

/*--------------------------------------------------------------*/
LONG pj_sdl_get_display_scale()
{
	return 4;
}

/*--------------------------------------------------------------*/
static SDL_FRect pj_sdl_rect_convert(const SDL_Rect* rect) {
	SDL_FRect result = { rect->x, rect->y, rect->w, rect->h };
	return result;
}

/*--------------------------------------------------------------*/
SDL_FRect pj_sdl_fit_surface(const SDL_Surface* source, const int target_w, const int target_h)
{
	SDL_Rect result_rect = {.x = 0, .y = 0, .w = source->w, .h = source->h};

	const double scale_x = ((double)target_w) / ((double)source->w);
	const double scale_y = ((double)target_h) / ((double)source->h);

	const bool can_scale_x = (source->h * scale_x) < target_h;

	if (can_scale_x) {
		result_rect.w = target_w;
		result_rect.h = source->h * scale_x;
		result_rect.y = (target_h - result_rect.h) / 2;
	} else {
		result_rect.h = target_h;
		result_rect.w = source->w * scale_y;
		result_rect.x = (target_w - result_rect.w) / 2;
	}

	return pj_sdl_rect_convert(&result_rect);
}

/*--------------------------------------------------------------*/
void pj_sdl_flip_window_surface()
{
	void* pixels = NULL;
	int pitch = 0;

	// Lock the texture so we have access to its pixels for writing
	if (!SDL_LockTexture(render_target, NULL, &pixels, &pitch)) {
		SDL_Log("Could not lock texture: %s", SDL_GetError());
		SDL_DestroyTexture(render_target);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	// Copy pixel data from surface to texture
	// memcpy(pixels, s_buffer->pixels, s_buffer->h * s_buffer->pitch);
	SDL_Surface* copy_target = SDL_CreateSurfaceFrom(
									render_target->w,
									render_target->h,
									render_target->format,
									pixels,
									pitch);

	if (!copy_target) {
		SDL_Log("Could create texture from locked pixels: %s", SDL_GetError());
	}

	SDL_BlitSurface(s_surface, NULL, copy_target, NULL);
	SDL_DestroySurface(copy_target);
	copy_target = NULL;

	// Unlock the texture-- commit changes
	SDL_UnlockTexture(render_target);

	const SDL_FRect source_rect = pj_sdl_rect_convert(&(SDL_Rect){0, 0, s_surface->w, s_surface->h});
	int window_w, window_h;
	pj_sdl_get_window_size(&window_w, &window_h);
	const SDL_FRect target_rect = pj_sdl_fit_surface(s_surface, window_w, window_h);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, render_target, &source_rect, &target_rect);
	SDL_RenderPresent(renderer);
}

/*--------------------------------------------------------------*/
const char* pj_sdl_resources_path() {
	static char resources_path[PATH_MAX] = {0};
	const char* base_path = SDL_GetBasePath();

	#ifdef IS_BUNDLE
		// On macOS, resources path is very specific
		snprintf(resources_path, PATH_MAX, pj_sdl_mac_bundle_path());
	#else
		// When not in a bundle, use the resources folder next to the executable
		snprintf(resources_path, PATH_MAX, "%sresource",
			 base_path);
	#endif
	return resources_path;
}


/*--------------------------------------------------------------*/
const char* pj_sdl_preferences_path() {
	static char preferences_path[PATH_MAX] = {0};
	if (preferences_path[0] == '\0') {
		snprintf(preferences_path, PATH_MAX, SDL_GetPrefPath("skeletonheavy", "vpaint"));
		// eat the last separator
		preferences_path[SDL_strlen(preferences_path)-1] = '\0';
	}

	return preferences_path;
}


/*--------------------------------------------------------------*/
/*
 * File Dialog Support
 *
 * I was using nativefiledialog-extended, but it looks like it's
 * totally broken for me on Linux.  I'm punting and using the SDL
 * dialogs, only I've wrapped them in blocking calls because the
 * way they work currently is terrible.
 */
/*--------------------------------------------------------------*/

typedef struct {
    char** files;
    bool completed;
    SDL_Mutex* mutex;
    int error;
} DialogResult;


static void FreeFileList(char** files) {
    if (files) {
        for (int i = 0; files[i]; i++) SDL_free(files[i]);
        SDL_free(files);
    }
}


static void FileDialogCallback(void* userdata, const char* const* files, int filter) {
    (void)filter;
    DialogResult* result = (DialogResult*)userdata;

    SDL_LockMutex(result->mutex);

    // Clear previous results
    FreeFileList(result->files);
    result->files = NULL;
    result->error = 0;

    if (!files) {
        result->error = SDL_GetError()[0] ? -1 : 0;
    } else {
        // Copy file list
        int count = 0;
        while (files[count]) count++;

        if (count > 0) {
            result->files = SDL_malloc((count + 1) * sizeof(char*));
            if (result->files) {
                for (int i = 0; i < count; i++)
                    result->files[i] = SDL_strdup(files[i]);
                result->files[count] = NULL;
            }
        }
    }

    result->completed = true;
    SDL_UnlockMutex(result->mutex);
}

static char** ShowDialogBlocking(SDL_Window* window,
                               void (*show_dialog)(SDL_DialogFileCallback, void*, SDL_Window*,
                                                 const SDL_DialogFileFilter*, int, const char*, bool),
                               const SDL_DialogFileFilter* filters,
                               int nfilters,
                               const char* default_location,
                               bool allow_many)
{
    DialogResult result = {0};
    result.mutex = SDL_CreateMutex();
    if (!result.mutex) {
        fprintf(stderr, "Mutex creation failed: %s\n", SDL_GetError());
        return NULL;
    }

    show_dialog(FileDialogCallback, &result, window, filters, nfilters,
               default_location, allow_many);

    // Event processing loop
    SDL_Event event;
    while (1) {
        SDL_PumpEvents();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SDL_DestroyMutex(result.mutex);
                FreeFileList(result.files);
                return NULL;
            }
        }

        SDL_LockMutex(result.mutex);
        bool completed = result.completed;
        SDL_UnlockMutex(result.mutex);

        if (completed) break;
        SDL_Delay(10);
    }

    SDL_LockMutex(result.mutex);
    char** final_files = result.files;
    if (result.error) {
        fprintf(stderr, "Dialog error: %s\n", SDL_GetError());
        FreeFileList(final_files);
        final_files = NULL;
    }
    SDL_UnlockMutex(result.mutex);

    SDL_DestroyMutex(result.mutex);
    return final_files;
}

static char** ShowOpenFileDialogBlocking(SDL_Window* window,
                                const SDL_DialogFileFilter* filters,
                                int nfilters,
                                const char* default_location,
                                bool allow_many)
{
    return ShowDialogBlocking(window, SDL_ShowOpenFileDialog,
                            filters, nfilters, default_location, allow_many);
}

static char** ShowSaveFileDialogBlocking(SDL_Window* window,
                                const SDL_DialogFileFilter* filters,
                                int nfilters,
                                const char* default_location)
{
    return ShowDialogBlocking(window, (void(*)(SDL_DialogFileCallback, void*, SDL_Window*,
                                             const SDL_DialogFileFilter*, int, const char*, bool))SDL_ShowSaveFileDialog,
                            filters, nfilters, default_location, false);
}



/*--------------------------------------------------------------*/
void pj_dialog_set_last_path(const char* path) {
	if (path) {
		strncpy(last_path, path, PATH_MAX);
	}
	else {
		//!TODO: Set to home folder?
		last_path[0] = '\0';
	}
}

/*--------------------------------------------------------------*/
char* pj_dialog_file_open(const char* type_name,
						  const char* extensions,
						  const char* default_path) {
	const SDL_DialogFileFilter filters[] = {
		{type_name, extensions}
	};

	char** open_files = ShowOpenFileDialogBlocking(window, filters, 1, NULL, false);
	if (open_files) {
		strncpy(last_path, open_files[0], PATH_MAX);
		FreeFileList(open_files);
		return last_path;
	}

	return NULL;
}


/*--------------------------------------------------------------*/
char* pj_dialog_file_save(const char* type_name,
						  const char* extensions,
						  const char* default_path) {
	const SDL_DialogFileFilter filters[] = {
		{type_name, extensions}
	};

	char** open_files = ShowSaveFileDialogBlocking(window, filters, 1, strnlen(last_path, PATH_MAX) == 0 ? NULL : last_path);
	if (open_files) {
		strncpy(last_path, open_files[0], PATH_MAX);
		FreeFileList(open_files);
		return last_path;
	}

	return NULL;
}
