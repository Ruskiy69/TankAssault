/**
 * @file
 *  Definitions for many graphic-manipulation functions.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 */

#include "Graphics.h"

using game::g_Log;

/**
 * Convert an SDL_Surface* to an OpenGL-compatible texture.
 *
 * @param SDL_Surface* Source
 *
 * @return Converted texture.
 */
u_int gfx::SDL_Surface_to_texture(SDL_Surface* pSrc)
{
    u_int texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(pSrc->format->BytesPerPixel == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSrc->w, pSrc->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSrc->pixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSrc->w, pSrc->h, 0, GL_RGB, GL_UNSIGNED_BYTE, pSrc->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

/**
 * Load an image and create an OpenGL texture from the pixel data.
 *
 * @param char* filename
 *
 * @return Loaded texture
 */
u_int gfx::load_texture(const char* pfilename)
{
    u_int texture;
    SDL_Surface* pImg = load_image(pfilename);
    
    if(pImg->format->BytesPerPixel != 3)
        return gfx::load_texture_alpha(pfilename);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, pImg->w, pImg->h, 0, GL_RGB, GL_UNSIGNED_BYTE, pImg->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(pImg);
    return texture;
}

/**
 * Load an image and create an OpenGL texture from the pixel data.
 *
 * Conserves alpha information.
 * 
 * @param char* filename
 *
 * @return Loaded texture
 */
u_int gfx::load_texture_alpha(const char* pfilename)
{
    u_int texture = 0;
    SDL_Surface* pImg = load_image(pfilename);
    if(pImg->format->BytesPerPixel != 4)
        return gfx::load_texture(pfilename);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, pImg->w, pImg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImg->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(pImg);

    if(texture == 0)
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to create an OpenGL texture from ";
        g_Log << pfilename << ".\n";
        g_Log << "[ERROR] " << gluErrorString(glGetError());
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    return texture;
}

/**
 * Uses the SDL_image library to load an image.
 *
 * @param char* filename
 *
 * @return Loaded surface
 */
SDL_Surface* gfx::load_image(const char* pfilename)
{
    // Check if the filename exists
    if(pfilename == NULL)
        gk::handle_error("No file to load!");

    SDL_Surface* pImage     = NULL;
    SDL_Surface* pOptimized = NULL;

    // Load the image (any file type)
    pImage = IMG_Load(pfilename);

    // Check if the image failed to load
    if(!pImage)
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load ";
        g_Log << pfilename << ".\n";

        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    return pImage;
}

/**
 * Optimizes an SDL_Surface* for the screen.
 *
 * @param SDL_Surface* Surface to optimize
 *
 * @return Optimized surface if successfully converted, original surface if not.
 */
SDL_Surface* gfx::optimize_surface(SDL_Surface* pSrc)
{
    // Attempt to optimize the image
    SDL_Surface* pOptimized = SDL_DisplayFormat(pSrc);

    // If the image failed to optimize, return the
    // unoptimized image.
    if(!pOptimized)
    {
        return pSrc;
    }
    else
    {
        SDL_FreeSurface(pSrc);
        return pOptimized;
    }
}

/**
 * Optimizes an SDL_Surface* with alpha-information for the screen.
 *
 * @param SDL_Surface* Surface to optimize
 *
 * @return Optimized surface if successfully converted, original surface if not.
 */
SDL_Surface* gfx::optimize_surface_alpha(SDL_Surface* pSrc)
{
    // Attempt to optimize the image
    SDL_Surface* pOptimized = SDL_DisplayFormatAlpha(pSrc);

    // If the image failed to optimize, return the
    // unoptimized image.
    if(!pOptimized)
    {
        return pSrc;
    }
    else
    {
        SDL_FreeSurface(pSrc);
        return pOptimized;
    }
}

/**
 * Blits one surface onto another.
 *
 * @param SDL_Surface* destination surface
 * @param SDL_Surface* source dusrface
 * @param int x-coordinate
 * @param int y-coordinate
 */
void gfx::blit_surface(SDL_Surface* pDst, SDL_Surface* pSrc, const int x, const int y)
{
    SDL_Rect Offset = {x, y, 0, 0};

    SDL_BlitSurface(pSrc, NULL, pDst, &Offset);
}

/**
 * Creates a color structure.
 *
 * @param int red
 * @param int green
 * @param int blue
 *
 * @return Color created
 *
 * @see gfx::Color
 */
gfx::Color gfx::create_color(const int r, const int g, const int b)
{
    gfx::Color Tmp = {r, g, b, 0};
    return Tmp;
}

/**
 * Converts a color structure to an integer.
 *
 * @param SDL_Surface* source surface to take format from
 * @param Color color to use
 *
 * @return Color in unsigned 32-bit integer format
 */
Uint32 gfx::create_uint32_color(SDL_Surface* pSrc, const gfx::Color& Pixel_Color)
{
    return SDL_MapRGB(pSrc->format, Pixel_Color.r, Pixel_Color.g, Pixel_Color.b);
}

/**
 * Creates a surface.
 *
 * @param int width
 * @param int height
 *
 * @return SDL_Surface* created surface
 */
SDL_Surface* gfx::create_surface(const int w, const int h)
{
    return SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, 0);
}

/**
 * Creates a surface and fills it with a color.
 *
 * @param int width
 * @param int height
 * @param Color fill color
 *
 * @return SDL_Surface* created surface
 */
SDL_Surface* gfx::create_surface(const int w, const int h, const SDL_Color& color)
{
    SDL_Surface* pFinal = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, 0);
    gfx::fill_rect(pFinal, NULL, color);
    return pFinal;
}

/**
 * Creates a surface with alpha information.
 *
 * @param int width
 * @param int height
 *
 * @return SDL_Surface* created surface
 */
SDL_Surface* gfx::create_surface_alpha(const int w, const int h)
{
    SDL_Surface* pTmp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA | SDL_RLEACCEL,
                w, h, 32, rmask, gmask, bmask, amask);

    return pTmp;
}

/**
 * Creates a surface with alpha info and fills it with a color.
 *
 * @param int width
 * @param int height
 * @param Color fill color
 *
 * @return SDL_Surface* created surface
 */
SDL_Surface* gfx::create_surface_alpha(const int w, const int h, const Color& Fill_Color)
{
    SDL_Surface* pFinal = gfx::create_surface_alpha(w, h);
    gfx::fill_rect(pFinal, NULL, Fill_Color);
    return pFinal;
}

/**
 * Clips a SDL_Surface to some given dimensions.
 *
 * @param SDL_Surface* the surface to clip
 * @param int x-coordinate to start clipping
 * @param int y-coordinate to start clipping
 * @param int width to clip
 * @param int height to clip
 *
 * @return SDL_Surface* clipped surface
 */
SDL_Surface* gfx::clip_sprite_sheet(SDL_Surface* pSheet, const int x, const int y,
    const int w, const int h)
{
    SDL_Surface* pSprite = gfx::create_surface(w, h);
    gfx::blit_surface(pSprite, pSheet, x, y);

    return pSprite;
}

/**
 * Removes a color from a surface.
 *
 * @param SDL_Surface* source
 * @param Color color to remove
 */
void gfx::remove_color(SDL_Surface* pSrc, const Color& Color_Key)
{
    Uint32 color_key  = gfx::create_uint32_color(pSrc, Color_Key);
    SDL_SetColorKey(pSrc, SDL_SRCCOLORKEY | SDL_RLEACCEL, color_key);
}

/**
 * Fills a part of a surface with a color.
 *
 * @param SDL_Surface* surface
 * @param math::ML_Rect* area
 * @param Color color to fill
 *
 * @see math::ML_Rect
 */
void gfx::fill_rect(SDL_Surface* pSrc, const math::ML_Rect* pBox, const Color& Fill_Color)
{
    if(pBox == NULL)
        SDL_FillRect(pSrc, NULL, gfx::create_uint32_color(pSrc, Fill_Color));
    else
    {
        SDL_Rect Tmp = {pBox->x, pBox->y, pBox->w, pBox->h};
        SDL_FillRect(pSrc, &Tmp, gfx::create_uint32_color(pSrc, Fill_Color));
    }
}

/**
 * Creates a tiled surface of a given tile.
 *
 * @param SDL_Surface* surface to tile
 * @param int x-coordinate to start tiling at
 * @param int y-coordinate to start tiling at
 * @param int x-coordinate to end tiling at
 * @param int y-coordinate to end tiling at
 *
 * @return Tiled surface
 */
SDL_Surface* gfx::tile_surface(SDL_Surface* pSrc, const int m_x, const int m_y, const int end_x, const int end_y)
{
    int x = m_x;
    int y = m_y;

    int max_w = end_x - m_x;
    int max_h = end_y - m_y;

    SDL_Surface* pTiled_Surf = gfx::create_surface(max_w, max_h);        

    const int w = pSrc->w;
    const int h = pSrc->h;

    int w_taken = 0;
    int h_taken = 0;

    SDL_Rect blitRect = {x, y, 0, 0};

    while(true)
    {
        gfx::blit_surface(pTiled_Surf, pSrc, x, y);

        if(w_taken >= max_w && h_taken >= max_h)
            break;

        if(x >= end_x && !(y >= end_y))
        {
            x = 0;
            w_taken = 0;
            y += h;
            h_taken += h;
        }
        else
        {
            x += w;
            w_taken += w;
        }
    }

    return pTiled_Surf;
}

/**
 * Retrieves the color of a pixel.
 *
 * @param SDL_Surface* surface to get pixel from
 * @param int x-coordinate
 * @param int y-coordinate
 *
 * @return Pixel color in unsigned 32-bit integer format.
 */
Uint32 gfx::get_pixel(SDL_Surface* pSrc, int x, int y)
{
    int bpp = pSrc->format->BytesPerPixel;

    // Here p is the address to the pixel we want to retrieve
    Uint8* p = (Uint8*)pSrc->pixels + y * pSrc->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16*)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32*)p;
        break;
    }

    return 0;
}
