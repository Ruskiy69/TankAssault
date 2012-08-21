/**
 * @file
 *  Defines many functions allowing for easy manipulation of
 *  textures. Also includes simple ways to convert between SDL_Surface*'s
 *  and OpenGL-compatible textures.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Graphics
 */
/// @{

#ifndef GRAPHICS_H
#define GRAPHICS_H

// C++ stuff
#include <string>
#include <vector>
#include <sstream>

// OpenGL stuff
#include <Windows.h>
#include "GL/glew.h"
#include "GL/GLU.h"
#include <gl/GL.h>

// SDL stuff
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

// My stuff
#include "Errors.h"
#include "CollapseDef.h"
#include "Math/Math.h"

// Determine bitmasks for SDL_Surface*'s based on endianess.
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  #define rmask 0xff000000
  #define gmask 0x00ff0000
  #define bmask 0x0000ff00
  #define amask 0x000000ff
#else
  #define rmask 0x000000ff
  #define gmask 0x0000ff00
  #define bmask 0x00ff0000
  #define amask 0xff000000
#endif

namespace gfx
{
    typedef SDL_Color Color;

    /// Define some default colors
    static const Color WHITE    = {255, 255, 255,   0};
    static const Color BLACK    = {0,   0,   0,     0};
    static const Color RED      = {255, 0,   0,     0};
    static const Color GREEN    = {0,   255, 0,     0};
    static const Color BLUE     = {0,   0,   255,   0};
    static const Color YELLOW   = {255, 255, 0,     0};
    static const Color PURPLE   = {255, 0,   255,   0};

    u_int SDL_Surface_to_texture(SDL_Surface* pSrc);
    u_int load_texture(const char* pfilename);
    u_int load_texture_alpha(const char* pfilename);

    SDL_Surface* load_image(const char* pfilename);
    SDL_Surface* optimize_surface(SDL_Surface* pSrc);
    SDL_Surface* optimize_surface_alpha(SDL_Surface* pSrc);

    void blit_surface(SDL_Surface* pDst, SDL_Surface* pSrc, const int x, const int y);

    Color create_color(const int r, const int g, const int b);
    Uint32 create_uint32_color(SDL_Surface* pSrc, const Color& Pixel_Color);

    SDL_Surface* create_surface(const int w, const int h);
    SDL_Surface* create_surface(const int w, const int h, const Color& Fill_Color);
    SDL_Surface* create_surface_alpha(const int w, const int h);
    SDL_Surface* create_surface_alpha(const int w, const int h, const Color& Fill_Color);

    SDL_Surface* tile_surface(SDL_Surface* pSrc, const int x, const int y, const int end_x, const int end_y);
    SDL_Surface* tile_surface(SDL_Surface* pSrc, const math::ML_Rect& Dimensions);

    SDL_Surface* clip_sprite_sheet(SDL_Surface* pSheet, const int x, const int y, const int w, const int h);
    SDL_Surface* clip_sprite_sheet(SDL_Surface* pSheet, const math::ML_Rect& Dimentions);

    void remove_color(SDL_Surface* pSrc, const Color& Color_Key);
    void remove_color(SDL_Surface* pSrc, const int r, const int g, const int b);

    void fill_rect(SDL_Surface* pSrc, const math::ML_Rect* Fill_Area, const Color& To_Fill);

    Uint32 get_pixel(SDL_Surface* pSrc, const int x, const int y);
}

#endif // GRAPHICS_H

/// @}