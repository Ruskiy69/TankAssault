#include <sstream>

#include "SDL_GL_Base.h"
#include "SDL_Base.h"

using gk::HandleError;

bool gk_gl::detect_collision(const gk_gl::GL_Rect& One, const gk_gl::GL_Rect& Two)
{
    int top1, bottom1, right1, left1;
    int top2, bottom2, right2, left2;

    left1   = (int)One.Position.x;
    right1  = (int)One.Position.x + One.w;
    top1    = (int)One.Position.y;
    bottom1 = (int)One.Position.y + One.h;

    left2   = (int)Two.Position.x;
    right2  = (int)Two.Position.x + Two.w;
    top2    = (int)Two.Position.y;
    bottom2 = (int)Two.Position.y + Two.h;

    if(bottom1 <= top2)
        return false;
    if(top1 >= bottom2)
        return false;
    if(right1 <= left2)
        return false;
    if(left1 >= right2)
        return false;

    return true;
}

SDL_Rect gk_gl::GL_to_SDL_Rect(const GL_Rect& Rect)
{
    SDL_Rect Tmp = {(int)Rect.Position.x,
        (int)Rect.Position.y, Rect.w, Rect.h};

    return Tmp;
}

gk_gl::GL_Rect gk_gl::SDL_to_GL_Rect(const SDL_Rect& Rect)
{
    gk_gl::GL_Vertex2f Tmp1(Rect.x, Rect.y);
    gk_gl::GL_Rect Tmp2(Tmp1, Rect.w, Rect.h);

    return Tmp2;
}

gk_gl::gl_texture gk_gl::SDL_Surface_to_texture(SDL_Surface* Source)
{
    SDL_Surface* final = SDL_CreateRGBSurface(0, Source->w, Source->h, 32, rmask, gmask, bmask, amask);
    SDL_SetAlpha(Source, 0, SDL_ALPHA_TRANSPARENT);
	SDL_BlitSurface(Source, 0, final, 0);   // blit onto a purely RGB Surface
	
    unsigned int texture = SOIL_create_OGL_texture((unsigned char*)final->pixels,
            Source->w, Source->h, 4, SOIL_CREATE_NEW_ID, 0);

    SDL_FreeSurface(final);
    return texture;
}

gk_gl::gl_texture gk_gl::SDL_Surface_to_texture_reload(SDL_Surface* Source, unsigned int old_texture)
{
    SDL_Surface* final = SDL_CreateRGBSurface(0, Source->w, Source->h, 32, rmask, gmask, bmask, amask);
    SDL_SetAlpha(Source, 0, SDL_ALPHA_TRANSPARENT);
	SDL_BlitSurface(Source, 0, final, 0);   // blit onto a purely RGB Surface
	
    unsigned int texture = SOIL_create_OGL_texture((unsigned char*)final->pixels,
            Source->w, Source->h, 4, old_texture, 0);

    SDL_FreeSurface(final);
    return texture;
}

gk_gl::gl_texture gk_gl::LoadTexture(const char* filename, const int flags)
{
    if(filename == NULL)
        HandleError("No filename to load!");

    unsigned int texture = SOIL_load_OGL_texture(filename,
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);

    if(texture == 0)
    {
        std::stringstream ss;
        ss << "Unable to load \"" << filename << "\"";
        HandleError(ss.str().c_str());
    }

    return texture;
}

gk_gl::gl_texture gk_gl::ReloadTexture(const char* filename, const int old_texture, const int flags)
{
    if(filename == NULL)
        HandleError("No filename to load!");

    unsigned int texture = SOIL_load_OGL_texture(filename,
        SOIL_LOAD_AUTO, old_texture, flags);

    if(texture == 0)
    {
        std::stringstream ss;
        ss << "Unable to load \"" << filename << "\"";
        HandleError(ss.str().c_str());
    }

    return texture;
}
