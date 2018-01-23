#include<gfx.h>
#include<stdio.h>
#include<raymarch.h>
#include<math.h>

gfx_t ctx;

int main(int argc, char **argv)
{
    ctx.title = "gfx_test";
    ctx.v_sync = 1;
    ctx.full_screen = 0;
    ctx.width = 300;
    ctx.height = 200;

    if(gfx_init(&ctx))
        fprintf(stderr, "Error: unable to intialize gfx context!");

    float vertical_size =  (float)tan(3.1415296f / 180 * 30);
    float horizontal_size = ctx.width / (float)ctx.height * vertical_size;
    float far = 20;
    float near = 1;
    float angle = 0;
    vec3_t cam   = {0, 0, -5};
    vec3_t light = {0, 0, -10};
    
    vec3_t axis = vec3_normalized(vec3_create(1, 0.01, 1));
    quat_t rot = quat_from_axis_angle(axis, 0);
    vec3_t zero = {0, 0, 0};

    while(!ctx.closed)
    {
        gfx_clear(&ctx);

        angle += ctx.delta_time;
        quat_t delta_quat = quat_from_axis_angle(axis, ctx.delta_time);
        rot = quat_mul(delta_quat, rot);

        for(int x = 0; x < ctx.width; x++)
        {
            for(int y = 0; y < ctx.height; y++)
            {
                float cam_space_x     = (2 * (x / (float)ctx.width) - 1) * horizontal_size;
                float cam_space_y     = (-2 * (y / (float)ctx.height) + 1) * vertical_size;
                float depth = near;
                vec3_t dir = vec3_normalized(vec3_create(cam_space_x, cam_space_y, near));
                ray_t ray;
                ray.origin = cam;
                ray.dir = dir;

                if(ray_march(ray, far, &depth, torus_distance_field, zero, rot))
                {
                    vec3_t point = vec3_add(ray.origin, vec3_scaled(ray.dir, depth));
                    vec3_t n = normal(point, torus_distance_field, zero, rot);
                    vec3_t origin = vec3_add(point, vec3_scaled(n, 2.0f * EPSILON));
                    vec3_t to_light = vec3_normalized(vec3_sub(light, origin));
                    float color = 0;

                    vec3_t refl = vec3_reflected(ray.dir, n);

                    float diffuse = vec3_dot(n, to_light);
                    float specular = vec3_dot(refl, to_light);

                    if(diffuse < 0)
                        diffuse = 0;

                    if(specular < 0)
                        specular = 0;

                    specular = pow(specular, 5);
                    color += diffuse + specular;

                    if(color > 1)
                            color = 1;

                    gfx_putpixel(&ctx, x, y, color * 0xff, color * 0xff, color * 0xff);
                }
            }
        }

        gfx_update(&ctx);
    }

    gfx_destroy(&ctx);
    
    return 0;
}