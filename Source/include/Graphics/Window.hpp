/**
 * @file
 *  CWindow class declarations.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Graphics
 **/
/// @{

#ifndef GRAPHICS__WINDOW_HPP
#define GRAPHICS__WINDOW_HPP

#include "CollapseDef.hpp"
#include "Graphics/Graphics.hpp"

namespace gfx
{
    /// Creates and maintains an OpenGL window context.
    class CWindow
    {
    public:
        CWindow(const int w, const int h,
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

#endif // GRAPHICS_WINDOW_HPP

/// @}