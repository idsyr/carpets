#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "draw_utils_interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "errhand.h"

void print_sdl_driver_info(FILE *stream, int num, const SDL_RendererInfo *info) {
	fprintf(stream, "\n---\n");
	fprintf(stream, "Number: %d\n", num);
	fprintf(stream, "Name: %s\n", info->name);
	fprintf(stream, "SW: %x, ACC: %x, VSYNC: %x, TEXTURE: %x\n",
	        info->flags & SDL_RENDERER_SOFTWARE,
	        info->flags & SDL_RENDERER_ACCELERATED,
	        info->flags & SDL_RENDERER_PRESENTVSYNC,
	        info->flags & SDL_RENDERER_TARGETTEXTURE);
}


void prinfo_all_sdl_drivers(FILE *stream) {
	int sdl_drivers_num = SDL_GetNumRenderDrivers();
	if(sdl_drivers_num < 0) ERR(SDL_GetNumRenderDrivers);

	for(int i = 0; i < sdl_drivers_num; ++i){
		int res; SDL_RendererInfo info;
		res = SDL_GetRenderDriverInfo(i, &info);
		if(res < 0) ERR(SDL_GetRenderDriverInfo);
		print_sdl_driver_info(stream, i, &info);
	}
}


void prinfo_selected_sdl_driver(SDL_Renderer *renderer, FILE *stream) {
	SDL_RendererInfo info; 
	if(SDL_GetRendererInfo(renderer, &info)) ERR(SDL_GetRendererInfo);
	else print_sdl_driver_info(stream, -42, &info);
} 
