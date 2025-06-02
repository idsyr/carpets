#ifndef DRAW_UTILS_INTERFACE_H
#define DRAW_UTILS_INTERFACE_H

struct Viewport;
struct Surface;

enum pollres { PROCESS, STOP };

typedef enum key {
  ekey_undef,
  ekey_printscreen,
  ekey_escape,
  ekey_up,
  ekey_down,
  ekey_left,
  ekey_right,
} key_e;

typedef struct Surface_color {
  unsigned r : 8;
  unsigned g : 8;
  unsigned b : 8;
  unsigned a : 8;
} Surface_color_t;

typedef void ( *draw_clbk_t )( struct Surface * surface, void * data );
typedef void ( *input_clbk_t )( key_e k, struct Surface * surface, struct Viewport * viewport, void * pack );

struct Viewport * create_viewport( draw_clbk_t print_callback, input_clbk_t input_clbk, int width, int height, void * data );
void              destroy_viewport( struct Viewport * viewport );
struct Surface *  create_surface( struct Viewport * viewport );
void              destroy_surface( struct Surface * surface );
enum pollres      viewport_poll( struct Viewport * viewport, struct Surface * surface );
void              surface_fillwith( struct Surface * surface, Surface_color_t color );
void              surface_putpixel( struct Surface * surface, Surface_color_t color, int x, int y );
void              make_screenshot( struct Surface * surface, struct Viewport * viewport );

#endif
