#include "Graphics/GFX_Base.h"

using namespace GFX;

using std::vector;
using std::string;

MathHelper::Rect GFX::create_rect(const int x, const int y, const int w, const int h)
{
    return MathHelper::Rect(x, y, w, h);
}

Color GFX::create_color(const int r, const int g, const int b)
{
    Color color = {r, g, b, 0};
    return color;
}

Uint32 GFX::create_uint32_color(SDL_Surface* source, const SDL_Color& color)
{
    return SDL_MapRGB(source->format, color.r, color.g, color.b);
}

SDL_Surface* GFX::create_surface(const int w, const int h)
{
    return SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, 0);
}

SDL_Surface* GFX::create_surface(const int w, const int h, const SDL_Color& color)
{
    SDL_Surface* final = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,32, rmask, gmask, bmask, 0);
    GFX::fill_rect(final, NULL, color);
    return final;
}

SDL_Surface* GFX::create_surface_alpha(const int w, const int h)
{
    SDL_Surface* tmp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA | SDL_RLEACCEL,
                w, h, 32, rmask, gmask, bmask, amask);

    return tmp;
}

SDL_Surface* GFX::create_surface_alpha(const int w, const int h, const SDL_Color& color)
{
    SDL_Surface* final = GFX::create_surface_alpha(w, h);
    GFX::fill_rect(final, NULL, color);
    return final;
}

SDL_Surface* GFX::clip_sprite_sheet(SDL_Surface* sheet, const int x, const int y,
    const int w, const int h)
{
    SDL_Surface* sprite = GFX::create_surface(w, h);
    GFX::blit_surface(sprite, sheet, x, y);

    return sprite;
}

void GFX::remove_color(SDL_Surface* source, const SDL_Color& color)
{
    Uint32 colorkey  = GFX::create_uint32_color(source, color);
    SDL_SetColorKey(source, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
}

void GFX::fill_rect(SDL_Surface* source, SDL_Rect& box, const SDL_Color& color)
{
    SDL_FillRect(source, &box, GFX::create_uint32_color(source, color));
}

void GFX::fill_rect(SDL_Surface* source, SDL_Rect* box, const SDL_Color& color)
{
    SDL_FillRect(source, box, GFX::create_uint32_color(source, color));
}

SDL_Surface* GFX::tile_surface(SDL_Surface* source, const int w, const int h)
{
    SDL_Surface* tiledSurf  = GFX::create_surface(w, h, GFX::create_color(BLACK));
    const int surfWidth     = source->clip_rect.w;
    const int surfHeight    = source->clip_rect.h;

    int surfsMade           = 0;
    int widthTaken          = 0;
    int heightTaken         = 0;

    int current_x           = 0;
    int current_y           = 0;

    SDL_Rect whereToBlit    = {current_x, current_y, 0, 0};

    while(true)
    {
        SDL_BlitSurface(source, NULL, tiledSurf, &whereToBlit);

        if(widthTaken >= w && !(heightTaken >= h))
            // Move to the next line
        {
            widthTaken  = 0;
            heightTaken += surfHeight;
            current_y   += surfHeight;
            current_x   = 0;
        }
        else
        {
            // Move one tile over
            current_x   += surfWidth;
        }

        if(widthTaken >= w && heightTaken >= h)
            // Meaning the whole screen is tiled
            break;

        widthTaken    += surfWidth;

        whereToBlit.x = current_x;
        whereToBlit.y = current_y;

        surfsMade++;
    }

    return tiledSurf;
}

SDL_Surface* GFX::tile_surface(SDL_Surface* source, const int m_x, const int m_y, const int end_x, const int end_y)
{
    int x = m_x;
    int y = m_y;

    int max_w = end_x - m_x;
    int max_h = end_y - m_y;

    SDL_Surface* tiledSurf = GFX::create_surface(max_w, max_h);        

    const int w = source->clip_rect.w;
    const int h = source->clip_rect.h;

    int w_taken = 0;
    int h_taken = 0;

    SDL_Rect blitRect = {x, y, 0, 0};

    while(true)
    {
        SDL_BlitSurface(source, NULL, tiledSurf, &blitRect);

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

        blitRect.x = x;
        blitRect.y = y;
    }

    return tiledSurf;
}

int GFX::get_text_width(TTF_Font* font, const string& text)
{
    int width = 0;
    TTF_SizeText(font, text.c_str(), &width, NULL);
    return width;
}

int GFX::get_text_height(TTF_Font* font, const string& text)
{
    int height = 0;
    TTF_SizeText(font, text.c_str(), NULL, &height);
    return height;
}

Uint32 GFX::get_pixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
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

SDL_Surface* GFX::load_image(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        gk::handle_error("No file to load!");

    /* We need to set up our surfaces for
     * the original image and the version
     * optimized for our display.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    /* Use the SDL_image library to load
     * the image. This allows us to load many
     * types of image files, like PNG and JPG,
     * rather than be limited to BMP's by
     * the SDL defaults.
     */
    image = IMG_Load(filename);

    /* If the image failed to load, show
     * the user an error message.
     */
    if(!image)
    {
        gk::handle_error(SDL_GetError());
    }

    /* Optimize the image */
    optimized = SDL_DisplayFormat(image);

    /* If we could not optimize it, simply
     * return the unoptimized version, otherwise
     * we can return the optimized image.
     */
    if(!optimized)
    {
        return image;
    }
    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
}

SDL_Surface* GFX::load_image_alpha(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        gk::handle_error("No file to load!");

    /* This function loads images in the same way
     * that LoadImage() does, except that it will
     * maintain alpha-transparency for images such 
     * as transparent PNG's.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    image = IMG_Load(filename);

    if(!image)
    {
        gk::handle_error(SDL_GetError());
    }

    return image;
    /* the following is pure SDL (NO OPENGL) code
    optimized = SDL_DisplayFormatAlpha(image);

    if(!optimized)
        return image;

    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
    */
}

void GFX::blit_surface(SDL_Surface* Dest, SDL_Surface* Src, const int x, const int y)
{
    /* Blitting is basically placing a surface on another
     * surface at a certain location. 
     */
    SDL_Rect offset = {x, y, 0, 0};

    SDL_BlitSurface(Src, NULL, Dest, &offset);
}

gl_texture GFX::SDL_Surface_to_texture(SDL_Surface* Img)
{
    gl_texture texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(Img->format->BytesPerPixel == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Img->w, Img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img->pixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Img->w, Img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, Img->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

gl_texture GFX::load_texture(const char* filename)
{
    gl_texture texture;
    SDL_Surface* Img = load_image_alpha(filename);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(Img->format->BytesPerPixel == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, 4, Img->w, Img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img->pixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, 3, Img->w, Img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, Img->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(Img);
    return texture;
}