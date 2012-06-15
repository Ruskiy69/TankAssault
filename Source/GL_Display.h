#ifndef __GL_DISPLAY_H__
#define __GL_DISPLAY_H__

#include "SDL_GL_Base.h"
#include "SDL_Base.h"
#include "SDL_Helper.h"

namespace gk_gl
{
    class CGL_Display
    {
    public:
        CGL_Display(const int w, const int h, const char* title = "SDL/GL Window", const char* icon = NULL);
        ~CGL_Display(){}

        bool ToggleFullscreen();

        void Clear();
        void Update();

        int GetWidth() const;
        int GetHeight() const;

    private:
        void Init(const int flags = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);

        SDL_Surface* Display;
        int width, height;
        bool fs;
    };
};

#endif // __GL_DISPLAY_H__
