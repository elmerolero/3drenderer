#include "display.h"
#include "vector.h"
#include "mesh.h"

#define NUMBER_POINTS (9 * 9 * 9)
vec3_t cube_points[ NUMBER_POINTS ];
vec2_t projected_points[ NUMBER_POINTS ];

triangle_t triangles_to_render[ N_MESH_FACES ];

vec3_t camera_position = { .x = 0, .y = 0, .z = -6 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };
vec3_t speed_camera = { .x = 0, .y = 0, .z = 0 };



float fov_factor = 640;

bool is_running = false;

Uint32 previous_frame_time = 0;

const uint32_t azul = 0xFF0B3075;
const uint32_t tinto = 0xFF750B29;
bool up = false;
uint32_t color = tinto;

void setup( void ){
    // Allocates space to color buffer
    colorBuffer = (uint32_t *)malloc( sizeof( uint32_t ) * window_width * window_height );
    
    // Create a texture user to display the buffer color
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
}

void process_input( void ){
    SDL_Event event;
    SDL_PollEvent( &event );
    switch( event.type ){
    	case SDL_QUIT:
	    is_running = false;
	    break;
	case SDL_KEYDOWN:
	    if( event.key.keysym.sym == SDLK_ESCAPE ){
	    	is_running = false;
	    }
        if( event.key.keysym.sym == SDLK_UP ){
            speed_camera.z = 0.1f;
        }
        else if( event.key.keysym.sym == SDLK_DOWN ){
            speed_camera.z = -0.1f;
        }
        if( event.key.keysym.sym == SDLK_LEFT ){
            speed_camera.x = -0.1f;
        }
        else if( event.key.keysym.sym == SDLK_RIGHT ){
            speed_camera.x = 0.1f;
        }
	    break;
    case SDL_KEYUP: 
        speed_camera.z = 0.f;
        break;
    } 
}

vec2_t orthogonal_projection( vec3_t point ){
    vec2_t projected_point = {
        .x = fov_factor * point.x,
        .y = fov_factor * point.y
    };

    return projected_point;
}

vec2_t perspective_projection( vec3_t point ){
    vec2_t projected_point = {
        .x = fov_factor * (point.x / point.z),
        .y = fov_factor * (point.y / point.z)
    };

    return projected_point;
}

void update( void ){
    Uint32 time_to_wait = FRAME_TARGET_TIME - ( SDL_GetTicks() - previous_frame_time  );
    if( time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME ){
        SDL_Delay( time_to_wait );
    }
    
    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    camera_position.z += speed_camera.z;
    camera_position.x += speed_camera.x;

    // For each face
    for( unsigned int i = 0; i < N_MESH_FACES; ++i ){
        face_t mesh_face = mesh_faces[ i ];

        vec3_t face_vertices[ 3 ];
        face_vertices[ 0 ] = mesh_vertices[ mesh_face.a - 1 ];
        face_vertices[ 1 ] = mesh_vertices[ mesh_face.b - 1 ];
        face_vertices[ 2 ] = mesh_vertices[ mesh_face.c - 1 ];

        triangle_t projected_triangle;

        for( unsigned int j = 0; j < 3; j++ ){
            vec3_t transformed_vertex = face_vertices[ j ];

            transformed_vertex = vec3_rotate_x( transformed_vertex, cube_rotation.x );
            transformed_vertex = vec3_rotate_y( transformed_vertex, cube_rotation.y );
            transformed_vertex = vec3_rotate_z( transformed_vertex, cube_rotation.z );
        
            transformed_vertex.z = transformed_vertex.z - camera_position.z;

            vec2_t projected_point = perspective_projection( transformed_vertex );
            projected_point.x += ( window_width / 2 );
            projected_point.y += ( window_height / 2 );

            projected_triangle.points[ j ] = projected_point;
        }

        triangles_to_render[ i ] = projected_triangle;
    }

    // TODO:
    /*for( int i = 0; i < NUMBER_POINTS; i++ ){
        vec3_t point = cube_points[ i ];

        vec3_t transformed_point = vec3_rotate_x( point, cube_rotation.x );
        transformed_point = vec3_rotate_y( transformed_point, cube_rotation.y );
        transformed_point = vec3_rotate_z( transformed_point, cube_rotation.z );

        transformed_point.z = transformed_point.z - camera_position.z;

        // Project the current point
        vec2_t projected_point = perspective_projection( transformed_point );

        projected_points[ i ] = projected_point;
    }*/
}

void render( void ){
    color_buffer_clear( 0xFFFFFFFF);

    float ww = window_width / 2;
    float wh = window_height / 2;

    for( unsigned int i = 0; i < N_MESH_FACES; ++i ){
        triangle_t triangle = triangles_to_render[ i ];
        draw_line_dda( triangle.points[ 0 ].x, triangle.points[ 0 ].y, triangle.points[ 1 ].x, triangle.points[ 1 ].y, 0xFFAA5588 );
        draw_line_dda( triangle.points[ 1 ].x, triangle.points[ 1 ].y, triangle.points[ 2 ].x, triangle.points[ 2 ].y, 0xFFAA5588 );
        draw_line_dda( triangle.points[ 2 ].x, triangle.points[ 2 ].y, triangle.points[ 0 ].x, triangle.points[ 0 ].y, 0xFFAA5588 );
        draw_rectangle( triangle.points[ 0 ].x, triangle.points[ 0 ].y, 4, 4, color );
        draw_rectangle( triangle.points[ 1 ].x, triangle.points[ 1 ].y, 4, 4, color );
        draw_rectangle( triangle.points[ 2 ].x, triangle.points[ 2 ].y, 4, 4, color );
    }

    color_buffer_render(); 
    
    SDL_RenderPresent( renderer );
}

void finish( void ){
    // Free resources
    free( colorBuffer );
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();

    // Set them to null
    colorBuffer = NULL;
    renderer = NULL;
    window = NULL;
}

int main( int argc, char *argv[] ){
     is_running = window_initialize();

     setup();
     
    vec3_t myvector = { 0.0, 0.0, 0.0 };

     while( is_running ){
     	process_input();
        update();
        render();
     }

     return 0;
}
