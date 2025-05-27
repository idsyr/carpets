#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "draw_utils_interface.h"

void draw_carpet( struct Surface * surface, void * data ) {

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
  for ( int y = 0; y < 2000; ++y ) {
    for ( int x = 0; x < 2000; ++x ) {
      if ( ( ( x ^ y ) * ( x ^ y ) >> 17 ) & 1 )
        surface_putpixel( surface, pattern_color, x, y );
    }
  }
}

int main() {
  struct Viewport * viewport = create_viewport( draw_carpet, 1000, 1000, NULL );
  struct Surface *  surface  = create_surface( viewport );
  while ( viewport_poll( viewport, surface ) == PROCESS )
    ;
  destroy_surface( surface );
  destroy_viewport( viewport );
}
