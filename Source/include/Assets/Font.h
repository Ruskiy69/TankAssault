/**
 * @file
 *  Font class declarations.
 *
 * @author George Kudrayvtsev
 * @version 1.0.1
 *
 * @addtogroup Media
 */
/// @{

#ifndef FONT_H
#define FONT_H

#include <string>

#include "SDL/SDL_ttf.h"

#include "Graphics.h"

#include "Assets/Asset.h"
#include "Assets/TextureAssetManager.h"

namespace asset
{
    /// Forward declaration for friend attribute later on.
    class FL_AssetManager;

    /// Wrapper around TTF_Font. 
    class FL_Font : public IAsset
    {
    public:
        ~FL_Font();

        bool Resize(const u_int new_size);

        GL_Entity*   RenderText(const char* ptext, const gfx::Color& Text_Color) const;
        SDL_Surface* RenderText_SDL(const char* ptext) const;
        SDL_Surface* RenderText_SDL(const char* ptext, const gfx::Color& Text_Color) const;

        u_int GetSize() const;
        TTF_Font* GetRawFont() const;

        int GetTextHeight(const char* text) const;
        int GetTextWidth(const char* text) const;

        friend class FL_AssetManager;

    private:
        FL_Font() : mp_Data(NULL), m_size(0) {}

        bool LoadFromFile(const char* pfilename);
        bool LoadFromFile(const char* pfilename, const int size);
        bool LoadFromFont(FL_Font* const p_Font);

        static const int DEFAULT_FONT_SIZE = 12;
        TTF_Font* mp_Data;
        u_int m_size;
    };
}

#endif // FONT_H
/// @}