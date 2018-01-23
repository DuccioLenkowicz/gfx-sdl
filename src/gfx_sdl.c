#ifdef _WIN32
#include <SDL.h>
#else 
#include <SDL2/SDL.h>
#endif
#include<gfx.h>

int gfx_init(gfx_t *ctx)
{
    if (SDL_Init(SDL_INIT_VIDEO))
        return -1;

    unsigned int flags = 0;

    if (ctx->full_screen)
        flags |= SDL_WINDOW_FULLSCREEN;

    if(!ctx->width)
        ctx->width = 640;

    if(!ctx->height)
        ctx->height = 580;

    SDL_Window *window = SDL_CreateWindow(ctx->title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        ctx->width,
        ctx->height,
        flags);

    if (!window)
    {
        SDL_Quit();
        return -1;
    }

    ctx->window = window;

    flags = 0;
    flags |= SDL_RENDERER_ACCELERATED;

    if(ctx->v_sync)
        flags |= SDL_RENDERER_PRESENTVSYNC;

    SDL_Renderer *renderer = SDL_CreateRenderer(ctx->window, -1, flags);

    if (!renderer)
    {
        SDL_DestroyWindow(ctx->window);
        SDL_Quit();
        return -1;
    }

    ctx->renderer = renderer;
    
    return 0;
}

void gfx_clear(gfx_t *ctx)
{
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(ctx->renderer);
}

void gfx_update(gfx_t *ctx)
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
            ctx->closed = 1;
    }

    SDL_RenderPresent(ctx->renderer);

    unsigned long long now = SDL_GetPerformanceCounter();
    ctx->delta_time = (float)(now - ctx->last_tick)/SDL_GetPerformanceFrequency();
    ctx->last_tick = now;
}

void gfx_putpixel(gfx_t *ctx, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
    SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 0xff);
    SDL_RenderDrawPoint(ctx->renderer, x, y);
}

void gfx_destroy(gfx_t *ctx)
{  
    SDL_DestroyWindow(ctx->window);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_Quit();
}
