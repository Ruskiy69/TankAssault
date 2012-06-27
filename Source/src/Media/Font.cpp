/************************************************************
 * The definitions for the MediaManager::Font class         *
 ***********************************************************/

#include "Media/Font.h"

using namespace MediaManager;
 
Font::Font() : Data(NULL), size(0) {}

bool Font::Load(const char* filename, const f_size size)
{
    this->size = size;
    this->name = filename;
    
    if(this->Data != NULL)  // A font is already loaded
        TTF_CloseFont(this->Data);
    
    this->Data = TTF_OpenFont(filename, size);
    
    return (this->Data == NULL) ? false : true;
}

bool Font::Load(const std::string& filename, const f_size size)
{
    return this->Load(filename.c_str(), size);
}

bool Font::Resize(const f_size size)
{
    if(this->size == size)
        return true;

    return this->Load(this->name, size);
}

GFX::GL_Entity* Font::RenderText(const std::string& text) const
{
    static const GFX::Color Black = {BLACK};     // Default color is black
    return this->RenderText(text, Black);
}

GFX::GL_Entity* Font::RenderText(const std::string& text, const GFX::Color& Color) const
{
    if(this->Data == NULL)
        return NULL;

    int skip_size   = TTF_FontLineSkip(this->Data);
    int height      = GFX::get_text_height(this->Data, text);
    int width       = 0;
    int index       = -1;

    SDL_Surface* Text_Surface = NULL;
    std::vector<std::string> Lines;
    std::string tmp(text);

    do
    {
        int w = 0;
        index = tmp.find("\n", 0);

        Lines.push_back(tmp.substr(0, index));

        if(index != std::string::npos)
        {
            tmp = tmp.substr(index + 1, -1);
        }
        
        TTF_SizeText(this->Data, Lines.back().c_str(), &w, &height);
        
        if(width < w)
            width = w;
    }
    while(index != std::string::npos);

    height = (Lines.size() - 1) * skip_size + height;

    Text_Surface = GFX::create_surface_alpha(width, height);
    SDL_Surface* Line_Text = NULL;

    for(unsigned int i = 0; i < Lines.size(); i++)
    {
        if(Lines[i] == "")  // A blank line?
            Line_Text = TTF_RenderText_Blended(this->Data, " ", Color);
        else
            Line_Text = TTF_RenderText_Blended(this->Data, Lines[i].c_str(), Color);

        SDL_SetAlpha(Line_Text, 0, SDL_ALPHA_TRANSPARENT);

        SDL_Rect Rect = {0, i * skip_size, 0, 0};
        SDL_BlitSurface(Line_Text, NULL, Text_Surface, &Rect);
        SDL_FreeSurface(Line_Text);
    }

    GFX::GL_Entity* Text = new GFX::GL_Entity();
    Text->SetEntity(Text_Surface);
    return Text;
}

SDL_Surface* Font::RenderText_SDL(const std::string& text, const GFX::Color& Color) const
{
    if(this->Data == NULL)
        return NULL;

    int skip_size   = TTF_FontLineSkip(this->Data);
    int height      = GFX::get_text_height(this->Data, text);
    int width       = 0;
    int index       = -1;

    SDL_Surface* Text_Surface = NULL;
    std::vector<std::string> Lines;
    std::string tmp(text);

    do
    {
        int w = 0;
        index = tmp.find("\n", 0);

        Lines.push_back(tmp.substr(0, index));

        if(index != std::string::npos)
        {
            tmp = tmp.substr(index + 1, -1);
        }
        
        TTF_SizeText(this->Data, Lines.back().c_str(), &w, &height);
        
        if(width < w)
            width = w;
    }
    while(index != std::string::npos);

    height = (Lines.size() - 1) * skip_size + height;

    Text_Surface = GFX::create_surface_alpha(width, height);
    SDL_Surface* Line_Text = NULL;

    for(unsigned int i = 0; i < Lines.size(); i++)
    {
        if(Lines[i] == "")  // A blank line?
            Line_Text = TTF_RenderText_Blended(this->Data, " ", Color);
        else
            Line_Text = TTF_RenderText_Blended(this->Data, Lines[i].c_str(), Color);

        SDL_SetAlpha(Line_Text, 0, SDL_ALPHA_TRANSPARENT);

        SDL_Rect Rect = {0, i * skip_size, 0, 0};
        SDL_BlitSurface(Line_Text, NULL, Text_Surface, &Rect);
        SDL_FreeSurface(Line_Text);
    }

    return Text_Surface;
}

Font::f_size Font::GetSize()
{
    return this->size;
}