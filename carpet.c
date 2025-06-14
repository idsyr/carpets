#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "draw_utils_interface.h"

struct Viewport * viewport;
struct Surface *  surface;

typedef struct {
  signed xpos;
  signed ypos;
} packof_position_t;

void draw_carpet( struct Surface * surface, packof_position_t * data );
void input_clbk( key_e key, struct Surface * surface, struct Viewport * viewport, packof_position_t * pack );
void main_cleanup();

static struct {
  unsigned h;
  unsigned w;
} conf = {
    .h = 1000,
    .w = 1000,
};

int main() {
  if ( atexit( main_cleanup ) ) {
    printf( "failed to register the ATEXIT function at: %s:%d", __FILE__, __LINE__ );
  }
  printf( "_____________________HOW_TO_USE___________________\n" );
  printf( "exit -> ESC\n" );
  printf( "screenshot -> PRINTSC\n" );
  packof_position_t pack = { .xpos = 0, .ypos = 0 };
  viewport               = create_viewport( ( draw_clbk_t )draw_carpet, ( input_clbk_t )input_clbk, conf.w, conf.h, &pack );
  surface                = create_surface( viewport );
  while ( viewport_poll( viewport, surface ) == PROCESS )
    ;
}

void draw_carpet( struct Surface * surface, packof_position_t * pack ) {
  Surface_color_t pattern_color = {
      .r = 0x5F,
      .g = 0xA0,
      .b = 0x9D,
      .a = 0x00, // european color matching
  };

  Surface_color_t background_color = {
      .r = 0xF4,
      .g = 0xE9,
      .b = 0xBE,
      .a = 0x00,
  };
  surface_fillwith( surface, background_color );
  for ( int y = 0; y < conf.h; ++y ) {
    for ( int x = 0; x < conf.w; ++x ) {
      if ( ( ( ( x + pack->xpos ) ^ ( y + pack->ypos ) ) * ( ( x + pack->xpos ) ^ ( y + pack->ypos ) ) >> 17 ) & 1 )
        surface_putpixel( surface, pattern_color, x, y );
    }
  }
}

void input_clbk( key_e key, struct Surface * surface, struct Viewport * viewport, packof_position_t * pack ) {
  switch ( key ) {
    case ( ekey_escape ):
      exit( EXIT_SUCCESS );
    case ( ekey_printscreen ):
      make_screenshot( surface, viewport );
    case ( ekey_up ):
      pack->ypos += 50;
    case ( ekey_down ):
      pack->ypos -= 50;
    case ( ekey_right ):
      pack->xpos += 50;
    case ( ekey_left ):
      pack->xpos -= 50;
  }
}

void main_cleanup() {
  destroy_surface( surface );
  destroy_viewport( viewport );
  printf( "________the program has successfully died, good luck!_______\n" );
}
