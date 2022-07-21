#include <math.h>
#include "vector.h"

// Rota al rededor del eje x
vec3_t vec3_rotate_x( vec3_t v, float angle ){
    float ca = cos( angle );
    float sa = sin( angle );
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * ca - v.z * sa,
        .z = v.y * sa + v.z * ca, 
    };
    return rotated_vector;
}

// Rota al rededor del eje y
vec3_t vec3_rotate_y( vec3_t v, float angle ){
    float ca = cos( angle );
    float sa = sin( angle );
    vec3_t rotated_vector = {
        .x = v.x * cos( angle ) - v.z * sin( angle ),
        .y = v.y,
        .z = v.x * sin( angle ) + v.z * cos( angle )
    };
    return rotated_vector;
}

vec3_t vec3_rotate_z( vec3_t v, float angle ){
    float ca = cos( angle );
    float sa = sin( angle );
    vec3_t rotated_vector = {
        .x = v.x * ca - v.y * sa,
        .y = v.x * sa + v.y * ca,
        .z = v.z
    };
    return rotated_vector;
}
