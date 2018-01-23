#include <cg_math.h>

#define EPSILON 0.01
#define MAX_MARCHING_STEPS 100

typedef float(*distance_field_estimator_t)(vec3_t p, vec3_t transl, quat_t rot);

float torus_distance_field(vec3_t p, vec3_t transl, quat_t rot)
{
    p = vec3_sub(p, transl);
    p = quat_rotated_vec(p, quat_conjugated(rot));

    float r_min = 0.3f;
    float r_max = 1.0f;
    vec2_t q = vec2_create(vec2_len(vec2_create(p.x, p.z)) - r_max, p.y);
    return (vec2_len(q) - r_min);
}

vec3_t normal(vec3_t p, distance_field_estimator_t e, vec3_t transl, quat_t rot)
{
    float nx = e(vec3_create(p.x + EPSILON, p.y, p.z), transl, rot) - e(vec3_create(p.x - EPSILON, p.y, p.z), transl, rot);
    float ny = e(vec3_create(p.x, p.y + EPSILON, p.z), transl, rot) - e(vec3_create(p.x, p.y - EPSILON, p.z), transl, rot);
    float nz = e(vec3_create(p.x, p.y, p.z + EPSILON), transl, rot) - e(vec3_create(p.x, p.y, p.z - EPSILON), transl, rot);
    vec3_t n = {nx, ny, nz};
    return vec3_normalized(n);
}

int ray_march(ray_t ray, float end, float *depth, distance_field_estimator_t e, vec3_t transl, quat_t rot)
{
    for(int i = 0; i < MAX_MARCHING_STEPS; i++)
    {
        vec3_t point = vec3_add(ray.origin, vec3_scaled(ray.dir, *depth));
        float dist = e(point, transl, rot);
        *depth += dist;
        if (dist < EPSILON) 
            return 1;

        if (*depth > end) 
            return 0;
    }

    return 0;
}


