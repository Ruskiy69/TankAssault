#ifndef FONT_H
#define FONT_H

#include <string>

#include "SDL/SDL_ttf.h"

#include "Graphics/GFX_Base.h"
#include "Graphics/GL_Entity.h"

namespace MediaManager
{
    class Font
    {
    public:
        typedef unsigned int f_size;
        
        Font();
        
        bool Load(const char* filename, const f_size size);
        bool Load(const std::string& filename, const f_size size);

        bool Resize(const f_size new_size);

        GFX::GL_Entity* RenderText(const std::string& text) const;
        GFX::GL_Entity* RenderText(const std::string& text, const GFX::Color& Color) const;
        SDL_Surface* RenderText_SDL(const std::string& text, const GFX::Color& Color) const;

        f_size GetSize();
        
    private:
        std::string name;
        TTF_Font* Data;
        f_size size;
    };
}

#endif // FONT_H
