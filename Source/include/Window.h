/**
 * @file
 *  GL_Window class declarations.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Graphics
 */
/// @{

#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "CollapseDef.h"
#include "Graphics.h"

namespace gfx
{
    /// Creates and maintains an OpenGL window context.
    class GL_Window
    {
    public:
        GL_Window(const int w, const int h,
            const char* title = "Window",
            const char* icon = NULL);

        bool ToggleFullscreen();

        void Clear();
        void Update();

        int GetWidth() const;
        int GetHeight() const;

    private:
        void Init(const int flags = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);

        SDL_Surface* mp_Display;
        int m_width, m_height;
        bool m_fs;
    };
}

#endif // OPENGL_WINDOW_H

/// @}