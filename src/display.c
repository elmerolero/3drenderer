#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
 
uint32_t *colorBuffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
unsigned int window_width = 800;
unsigned int window_height = 480;

bool window_initialize( void ){
     if( SDL_Init( SDL_INIT_VIDEO ) != 0 ){
     	fprintf( stderr, "Error initializing SDL.\n" );
	    return false;
     }
     
     SDL_DisplayMode displayMode;
     SDL_GetCurrentDisplayMode( 0, &displayMode );
     
     window_width = displayMode.w;
     window_height = displayMode.h;
     printf( "ww:%i\nwh:%i\n", window_width, window_height );

     // Create a SDL_Window	
     window = SDL_CreateWindow( "Program", 
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				window_width,
				window_height,
				SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP );

     if( window == NULL ){
         fprintf( stderr, "Error, creating SDL window.\n" );
     }
     
     // Create a SDL renderer
     renderer = SDL_CreateRenderer( window, -1, 0 );
     if( renderer == NULL ){
     	fprintf( stderr, "Error creating SDL renderer.\n" );
	    return false;
     }

     return true;
}

void color_buffer_clear( uint32_t color ){
    // For each row
    for( unsigned int y = 0; y < window_height; ++y ){
	    for( unsigned int x = 0; x < window_width; ++x ){
	        colorBuffer[ ( y * window_width ) + x ] = color;
	    }
    }
}

void color_buffer_render( void ){
    SDL_UpdateTexture(
	    color_buffer_texture,
	    NULL,
	    colorBuffer,
	    (int)( window_width * sizeof( uint32_t ) )
    );
    
    SDL_RenderCopy( renderer, color_buffer_texture, NULL, NULL );
}

void draw_pixel( unsigned int x, unsigned int y, uint32_t color ){
    colorBuffer[ (y * window_width) + x ] = color;
}

void draw_rectangle( unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color ){
    if( x < window_width && y < window_height && x + w < window_width && y + h < window_height ){
        unsigned int posX;
        unsigned int posY;
        for( posY = y; posY < y + h; ++posY ){
            for( posX = x; posX < x + w; ++posX ){
                colorBuffer[ (posY * window_width) + posX ] = color;
            }
        }
    }
}

void draw_line_dda( int x1, int y1, int x2, int y2, uint32_t color ){
    // Obtiene el cambio entre x y y de ambos puntos
    int delta_x = x2 - x1;
    int delta_y = y2 - y1;

    // Obtiene el largo de x
    int abs_x = abs( delta_x );
    int abs_y = abs( delta_y );
    int side_length = abs_x < abs_y ? abs_y : abs_x;

    // Obtiene el incremente en x y el cambio en y
    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x1;
    float current_y = y1;

    for( int i = 0; i < side_length; ++i ){
        colorBuffer[ ( (int)round( current_y ) * window_width ) + (int)round( current_x ) ] = color;
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_grid( void ){
    unsigned int x = 0;
    unsigned int y = 0;
    uint32_t color = 0xFF000000;
    for( y = 0; y < window_height; ++y ){
        // Is in a 10th position?
        if( y % 10 != 0){
            for( x = 0; x < window_width; ++x ){
                if( x % 10 == 0 ){
                    colorBuffer[ ( y * window_width ) + x ] = color;
                }
            }
        }
        else{
            for( x = 0; x < window_width; ++x ){
                colorBuffer[ ( y * window_width ) + x ] = color;
            }
        }
    }
}