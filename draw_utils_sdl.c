#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "draw_utils_interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "errhand.h"
#include "prinfo.h"

struct Surface {
  SDL_Renderer * renderer;
};

typedef struct Viewport {
  int          width, height;
  SDL_Window * window;
  draw_clbk_t  callback;
  input_clbk_t input_clbk;
  void *       data;
} Viewport_t;

struct Surface * create_surface( struct Viewport * viewport ) {
  struct Surface * surface = malloc( sizeof( struct Surface ) );

  surface->renderer = SDL_CreateRenderer( viewport->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
  if ( !surface->renderer )
    ERR( SDL_CreateRenderer, true );

  FILE * log_stream = fopen( "output/device.log", "w" );
  prinfo_all_sdl_drivers( log_stream );
  prinfo_selected_sdl_driver( surface->renderer, log_stream );
  fclose( log_stream );

  return surface;
}

void destroy_surface( struct Surface * surface ) {
  SDL_DestroyRenderer( surface->renderer );
  free( surface );
}

Viewport_t * create_viewport( draw_clbk_t callback, input_clbk_t input_clbk, int width, int height, void * data ) {
  if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    ERR( SDL_Init, true );
  atexit( SDL_Quit );

  Viewport_t * viewport = malloc( sizeof( struct Viewport ) );
  viewport->width       = width;
  viewport->height      = height;
  viewport->input_clbk  = input_clbk;

  viewport->window = SDL_CreateWindow( "__carpets__", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, viewport->width,
                                       viewport->height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

  if ( !viewport->window )
    ERR( SDL_CreateWindow, true );

  viewport->callback = callback;
  viewport->data     = data;
  return viewport;
}

void destroy_viewport( struct Viewport * viewport ) { SDL_DestroyWindow( viewport->window ); }

void make_screenshot( struct Surface * surface, struct Viewport * viewport ) {
  SDL_Texture * tex = SDL_CreateTexture( surface->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 1000, 1000 );
  SDL_SetRenderTarget( surface->renderer, tex );
  viewport->callback( surface, NULL );
  int width, height;
  SDL_QueryTexture( tex, NULL, NULL, &width, &height );
  SDL_Surface * buff = SDL_CreateRGBSurface( 0, width, height, 32, 0, 0, 0, 0 );
  SDL_RenderReadPixels( surface->renderer, NULL, buff->format->format, buff->pixels, buff->pitch );
  SDL_SaveBMP( buff, "output/carpet.bmp" );
  SDL_FreeSurface( buff );
  SDL_SetRenderTarget( surface->renderer, NULL );
}

key_e whichkey( SDL_KeyCode sdl_keycode ) {
  switch ( sdl_keycode ) {
    case ( SDLK_PRINTSCREEN ):
      return ekey_printscreen;
    case ( SDLK_ESCAPE ):
      return ekey_escape;
  }
  return ekey_undef;
}

enum pollres viewport_poll( Viewport_t * viewport, struct Surface * surface ) {
  int       pending;
  SDL_Event event;
  pending = SDL_PollEvent( &event );

  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE ) {
    exit( EXIT_SUCCESS );
  }

  if ( pending && event.type == SDL_KEYDOWN ) {
    SDL_Keycode kc = event.key.keysym.sym;
    viewport->input_clbk( whichkey( kc ), surface, viewport, viewport->data );
  }

  Uint32 start, elapsed, estimated = 1000 / 10;
  start = SDL_GetTicks();

  viewport->callback( surface, viewport->data );
  SDL_RenderPresent( surface->renderer );

  elapsed = SDL_GetTicks() - start;
  if ( elapsed < estimated )
    SDL_Delay( estimated - elapsed );

  return PROCESS;
}

void surface_putpixel( struct Surface * surface, Surface_color_t color, int x, int y ) {
  SDL_SetRenderDrawColor( surface->renderer, color.r, color.g, color.b, color.a );
  SDL_RenderDrawPoint( surface->renderer, x, y );
}

void surface_fillwith( struct Surface * surface, Surface_color_t color ) {
  SDL_SetRenderDrawColor( surface->renderer, color.r, color.g, color.b, color.a );
  SDL_RenderClear( surface->renderer );
}
