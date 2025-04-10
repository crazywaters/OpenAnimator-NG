#include <assert.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

/**
 * Convert any SDL surface to an 8-bit indexed color surface
 * This function is based on the convrgb.c code from the CONVERT
 * tool in the original source code.
 * 
 * @param surface The source surface to convert
 * @param num_colors Maximum number of colors to use (between 2 and 256)
 * @return A new SDL_Surface in INDEX8 format or NULL on error
 */
SDL_Surface* sdlpdr_convert_colors(SDL_Surface* surface, size_t num_colors) {
    if (surface == NULL || num_colors < 2 || num_colors > 256) {
        SDL_SetError("Invalid parameters for color conversion");
        return NULL;
    }

    // Create histogram and related structures for color quantization
    Uint8 *histogram = NULL;
    Uint8 *rgb_bufs[3] = {NULL, NULL, NULL};
    Uint8 *linebuf = NULL;
    int width = surface->w;
    int height = surface->h;
    SDL_Surface *new_surface = NULL;
    SDL_Palette *palette = NULL;
    
    // Allocate histogram for color analysis
    size_t hist_size = (256 * 256 * 256) / 8; // Same as HIST256_SIZE in convrgb.c
    histogram = (Uint8 *)calloc(1, hist_size);
    if (!histogram) {
        SDL_SetError("Out of memory for histogram");
        goto cleanup;
    }
    
    // Allocate RGB buffers for processing
    rgb_bufs[0] = (Uint8 *)malloc(width);  // R
    rgb_bufs[1] = (Uint8 *)malloc(width);  // G
    rgb_bufs[2] = (Uint8 *)malloc(width);  // B
    linebuf = (Uint8 *)malloc(width * 3);  // RGB triplets
    
    if (!rgb_bufs[0] || !rgb_bufs[1] || !rgb_bufs[2] || !linebuf) {
        SDL_SetError("Out of memory for RGB buffers");
        goto cleanup;
    }
    
    // Lock surface for pixel reading
    if (SDL_LockSurface(surface) < 0) {
        SDL_SetError("Unable to lock source surface for color conversion");
        goto cleanup;
    }
    
    // Extract RGB values from the source surface and build histogram
    Uint8 r, g, b;
    Uint32 pixel;
    int x, y, cbyte;
    long cbit;
    static const Uint8 bitmasks[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
    
    for (y = 0; y < height; y++) {
        // Extract a row of RGB values
        for (x = 0; x < width; x++) {
            if (SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, NULL) != true) {
                // Error reading pixel
                continue;
            }
            
            rgb_bufs[0][x] = r;
            rgb_bufs[1][x] = g;
            rgb_bufs[2][x] = b;
            
            // Set bits in the histogram
            cbit = ((long)r << 16) + (g << 8) + b;
            cbyte = cbit >> 3;
            histogram[cbyte] |= bitmasks[cbit & 7];
        }
    }
    
    SDL_UnlockSurface(surface);
    
    // Create the destination surface with INDEX8 format
    new_surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
    if (!new_surface) {
        SDL_SetError("Unable to create destination surface for color conversion");
        goto cleanup;
    }
    
    // Create a palette for the new surface
    palette = SDL_CreatePalette((int)num_colors);
    if (!palette) {
        SDL_SetError("Unable to create palette for color conversion");
        goto cleanup;
    }
    
    // Count unique colors in the histogram
    int unique_colors = 0;
    for (size_t i = 0; i < hist_size; i++) {
        Uint8 byte = histogram[i];
        while (byte) {
            if (byte & 1) {
                unique_colors++;
            }
            byte >>= 1;
        }
    }
    
    // Build color table from histogram
    Uint8 *color_table = (Uint8 *)malloc(unique_colors * 3);
    if (!color_table) {
        SDL_SetError("Out of memory for color table");
        goto cleanup;
    }
    
    int color_index = 0;
    for (int r = 0; r < 256; r++) {
        for (int g = 0; g < 256; g++) {
            for (int b = 0; b < 256; ) {
                Uint8 byte = histogram[(((r << 16) | (g << 8) | b) >> 3)];
                Uint8 mask = 0x80;
                
                for (int bit = 0; bit < 8 && b < 256; bit++, b++, mask >>= 1) {
                    if (byte & mask) {
                        if (color_index < unique_colors) {
                            color_table[color_index*3 + 0] = r;
                            color_table[color_index*3 + 1] = g;
                            color_table[color_index*3 + 2] = b;
                            color_index++;
                        }
                    }
                }
            }
        }
    }
    
    // Create palette colors from the color table
    // Use color quantization if we have more colors than our limit
    if (unique_colors <= num_colors) {
        // We can use all colors directly
        SDL_Color colors[256];
        for (int i = 0; i < unique_colors; i++) {
            colors[i].r = color_table[i*3 + 0];
            colors[i].g = color_table[i*3 + 1];
            colors[i].b = color_table[i*3 + 2];
            colors[i].a = 0xFF;
        }
        SDL_SetPaletteColors(palette, colors, 0, unique_colors);
    } else {
        // Need to pack colors - use a basic clustering approach
        SDL_Color colors[256];
        int threshold = 32; // Adjust this for quality vs color count tradeoff
        int used_colors = 0;
        
        // Use the first color as a starting point
        colors[0].r = color_table[0];
        colors[0].g = color_table[1];
        colors[0].b = color_table[2];
        colors[0].a = 255;
        used_colors = 1;
        
        // Find colors that are more than threshold distance away
        for (int i = 1; i < unique_colors && used_colors < (int)num_colors; i++) {
            Uint8 r = color_table[i*3 + 0];
            Uint8 g = color_table[i*3 + 1];
            Uint8 b = color_table[i*3 + 2];
            
            // See if this color is already represented
            int min_dist = threshold * threshold;
            for (int j = 0; j < used_colors; j++) {
                int dr = r - colors[j].r;
                int dg = g - colors[j].g;
                int db = b - colors[j].b;
                int dist = dr*dr + dg*dg + db*db;
                
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
            
            // If color is sufficiently different, add it
            if (min_dist >= threshold * threshold) {
                colors[used_colors].r = r;
                colors[used_colors].g = g;
                colors[used_colors].b = b;
                colors[used_colors].a = 255;
                used_colors++;
            }
        }
        
        SDL_SetPaletteColors(palette, colors, 0, used_colors);
    }
    
    // Set the palette on the new surface
    SDL_SetSurfacePalette(new_surface, palette);
    SDL_DestroyPalette(palette); // Surface has taken ownership
    
    // Map each pixel from the source to the best matching color in the palette
    if (SDL_LockSurface(new_surface) < 0) {
        goto cleanup;
    }
    
    Uint8 *dst_pixels = (Uint8 *)new_surface->pixels;
    
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, NULL) != true) {
                continue;
            }
            
            // Find closest color in palette
            int closest_index = 0;
            unsigned int closest_diff = 765; // 255*3, max possible diff
            
            for (int i = 0; i < palette->ncolors; i++) {
                unsigned int dr = abs((int)r - palette->colors[i].r);
                unsigned int dg = abs((int)g - palette->colors[i].g);
                unsigned int db = abs((int)b - palette->colors[i].b);
                unsigned int diff = dr + dg + db;
                
                if (diff < closest_diff) {
                    closest_diff = diff;
                    closest_index = i;
                    
                    if (diff == 0) // Perfect match
                        break;
                }
            }
            
            // Write the closest color index to the indexed surface
            dst_pixels[y * new_surface->pitch + x] = closest_index;
        }
    }
    
    SDL_UnlockSurface(new_surface);
    
    // Cleanup and return
    free(color_table);
    free(linebuf);
    free(rgb_bufs[0]);
    free(rgb_bufs[1]);
    free(rgb_bufs[2]);
    free(histogram);
    
    return new_surface;
    
cleanup:
    if (new_surface) SDL_DestroySurface(new_surface);
    if (palette) SDL_DestroyPalette(palette);
    if (histogram) free(histogram);
    if (linebuf) free(linebuf);
    if (rgb_bufs[0]) free(rgb_bufs[0]);
    if (rgb_bufs[1]) free(rgb_bufs[1]);
    if (rgb_bufs[2]) free(rgb_bufs[2]);
    
    return NULL;
}


SDL_Surface* sdlpdr_convert_surface_to_palette(SDL_Surface* surface, SDL_Palette* palette)
{
	assert(surface);
	assert(palette);

	// Convert the surface to 8-bit indexed format with the specified palette
	SDL_Surface* indexedSurface = SDL_ConvertSurfaceAndColorspace(
		surface,
		SDL_PIXELFORMAT_INDEX8,
		palette,
		SDL_COLORSPACE_UNKNOWN,  // Generally use UNKNOWN for indexed formats
		0                        // No additional properties needed
	);

	if (!indexedSurface) {
		SDL_SetError("Indexed surface conversion failed.");
		return NULL;
	}

	return surface;
}