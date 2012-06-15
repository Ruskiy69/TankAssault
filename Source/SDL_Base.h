#ifndef __SDL_BASE_H__
#define __SDL_BASE_H__

/* SDL headers */
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "Errors.h"

/* Link libraries */
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_mixer.lib")

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask            0xff000000
#define gmask            0x00ff0000
#define bmask            0x0000ff00
#define amask            0x000000ff
#else
#define rmask            0x000000ff
#define gmask            0x0000ff00
#define bmask            0x00ff0000
#define amask            0xff000000
#endif

#define ALIGN_CENTER    0x01
#define CREATE_SURFACE  0x02
#define TRANSPARENT_BG  0x04

#define WHITE           0xFF, 0xFF, 0xFF
#define BLACK           0x00, 0x00, 0x00
#define RED             0xFF, 0x00, 0x00
#define GREEN           0x00, 0xFF, 0x00
#define BLUE            0x00, 0x00, 0xFF
#define YELLOW          0xFF, 0xFF, 0x00
#define PURPLE          0xFF, 0x00, 0xFF

namespace gk_sdl
{
    /* Weird Windows BS */
#ifdef LoadImage
#undef LoadImage
#endif // LoadImage

    struct SDL_Circle
    {
        int x;
        int y;
        unsigned int radius;
    };
    
    SDL_Surface* LoadImage(const char* filename);
    SDL_Surface* LoadImage_Alpha(const char* filename);

    void blit_surface(SDL_Surface* Dst, SDL_Surface* Src, const int x, const int y);
    void blit_surface(SDL_Surface* Dst, SDL_Surface* Src, SDL_Rect& Pos);
}



#endif // __SDL_BASE_H__
