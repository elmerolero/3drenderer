#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define FPS 60
#define FRAME_TARGET_TIME ( 1000 / FPS )

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern uint32_t *colorBuffer;
extern SDL_Texture *color_buffer_texture;
extern unsigned int window_width;
extern unsigned int window_height;

bool window_initialize( void );
void color_buffer_clear( uint32_t color );
void color_buffer_render( void );

void draw_pixel( unsigned int x, unsigned int y, uint32_t color );
void draw_line_dda( int x1, int y1, int x2, int y2, uint32_t color );
void draw_rectangle( unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color );
void draw_grid( void );

#endif
