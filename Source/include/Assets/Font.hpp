/**
 * @file
 *  Font class declarations.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0.2
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
 * @addtogroup Assets
 **/
/// @{

#ifndef ASSETS__FONT_HPP
#define ASSETS__FONT_HPP

#include <string>

#include "SDL/SDL_ttf.h"

#include "Graphics/Graphics.hpp"
#include "World/Objects/Entity.hpp"
#include "Assets/Asset.hpp"

namespace asset
{
    /// Forward declaration for friend attribute later on.
    class CAssetManager;

    /// Wrapper around TTF_Font. 
    class CFont : public CAsset
    {
    public:
        ~CFont();

        bool Resize(const u_int new_size);

        obj::CEntity* RenderText(const char* ptext,
                                 const gfx::Color& Text_Color) const;

        SDL_Surface* RenderText_SDL(const char* ptext) const;
        SDL_Surface* RenderText_SDL(const char* ptext,
                                    const gfx::Color& Text_Color) const;

        TTF_Font* GetRawFont() const;
        u_int GetSize() const;
        int GetTextHeight(const char* text) const;
        int GetTextWidth(const char* text)  const;

        // Only the FontManager can create fonts.
        friend class CAssetManager;

    private:
        CFont() : mp_Data(NULL), m_size(0) {}

        bool LoadFromFile(const char* pfilename);
        bool LoadFromFile(const char* pfilename, const int size);
        bool LoadFromFont(CFont* const p_Font);

        TTF_Font*   mp_Data;
        u_int       m_size;
    };
}

#endif // FONT_H
/// @}