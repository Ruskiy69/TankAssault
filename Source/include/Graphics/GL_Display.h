#ifndef GL_DISPLAY_H
#define GL_DISPLAY_H

#include "GFX_Base.h"

namespace GFX
{
    class GL_Display
    {
    public:
        GL_Display(const int w, const int h, const char* title = "SDL/GL Window", const char* icon = NULL);
        ~GL_Display(){}

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

#endif // GL_DISPLAY_H
