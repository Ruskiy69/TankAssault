/**
 * @file
 *  FL_Font class definitions.
 *
 * @author George Kudrayvtsev
 * @version 1.0.1
 */

#include "Assets/Font.h"

using asset::FL_Font;

/**
 * Cleans up memory.
 *
 * @bug TTF_CloseFont doesn't work right.
 */
FL_Font::~FL_Font()
{
    if(mp_Data != NULL && m_refcount == 1)
        TTF_CloseFont(mp_Data);
    m_filename.clear();
}

/**
 * Loads a font.
 *
 * @param char* Font filename
 *
 * @return TRUE if font loaded, FALSE otherwise.
 */
bool FL_Font::LoadFromFile(const char* pfilename)
{
    if(pfilename == NULL)
        return false;

    m_size = DEFAULT_FONT_SIZE;
    m_filename = pfilename;

    if(mp_Data != NULL)
        TTF_CloseFont(mp_Data);

    mp_Data = TTF_OpenFont(pfilename, m_size);

    return (mp_Data != NULL);
}

/**
 * Loads a font.
 *
 * @param char* Font filename
 * @param int Font size
 *
 * @return TRUE if font loaded, FALSE otherwise.
 */
bool FL_Font::LoadFromFile(const char* pfilename, const int size)
{
    if(pfilename == NULL)
        return false;

    m_size = size;
    m_filename = pfilename;

    if(mp_Data != NULL)
        TTF_CloseFont(mp_Data);

    mp_Data = TTF_OpenFont(pfilename, m_size);

    return (mp_Data != NULL);
}

/**
 * Loads a font from an existing one.
 *
 * @param FL_Font* The font to copy attributes from.
 *
 * @return Always TRUE.
 */
bool FL_Font::LoadFromFont(FL_Font* const p_Copy)
{
    p_Copy->IncrementReferenceCount();
    mp_Data     = p_Copy->GetRawFont();
    m_size      = p_Copy->GetSize();
    m_refcount  = p_Copy->GetReferenceCount();
    m_filename  = p_Copy->GetFilename();

    return true;
}

/**
 * Resizes the current font.
 *
 * @param int New font size
 *
 * @return TRUE if font resized properly, FALSE otherwise.
 */
bool FL_Font::Resize(const u_int new_size)
{
    if(m_size == new_size)
        return true;

    return this->LoadFromFile(m_filename.c_str(), new_size);
}

/**
 * Render some text with some color.
 *
 * @param char* text to render
 * @param gfx::Color color of text
 *
 * @return asset::GL_Entity* The entity w/ text on it.
 *
 * @see gfx::Color
 * @see asset::GL_Entity
 */
asset::GL_Entity* FL_Font::RenderText(const char* ptext,
    const gfx::Color& Text_Color) const
{
    if(mp_Data == NULL || ptext == NULL)
        return NULL;

    int skip_size   = TTF_FontLineSkip(mp_Data);
    int height      = this->GetTextHeight(ptext);
    int width       = 0;
    int index       = -1;

    SDL_Surface* pText_Surface = NULL;
    std::vector<std::string> lines;
    std::string tmp(ptext);

    do
    {
        int w = 0;
        index = tmp.find("\n", 0);

        lines.push_back(tmp.substr(0, index));

        if(index != std::string::npos)
        {
            tmp = tmp.substr(index + 1, -1);
        }
        
        TTF_SizeText(mp_Data, lines.back().c_str(), &w, &height);
        
        if(width < w)
            width = w;
    }
    while(index != std::string::npos);

    height = (lines.size() - 1) * skip_size + height;

    pText_Surface = gfx::create_surface_alpha(width, height);
    SDL_Surface* pLine_Text = NULL;

    for(unsigned int i = 0; i < lines.size(); ++i)
    {
        if(lines[i] == "")  // A blank line?
            pLine_Text = TTF_RenderText_Blended(mp_Data, " ", Text_Color);
        else
            pLine_Text = TTF_RenderText_Blended(mp_Data,
                lines[i].c_str(), Text_Color);

        SDL_SetAlpha(pLine_Text, 0, SDL_ALPHA_TRANSPARENT);

        SDL_Rect Rect = {0, i * skip_size, 0, 0};
        SDL_BlitSurface(pLine_Text, NULL, pText_Surface, &Rect);
        SDL_FreeSurface(pLine_Text);
    }

    asset_id text_id = g_TextureAssets.LoadEntityFromSurface(pText_Surface);
    SDL_FreeSurface(pText_Surface);
    return g_TextureAssets.GetEntityByID(text_id);
}

/**
 * Render some text with black color, but return an SDL_Surface*.
 *
 * @param char* text to render
 * @param gfx::Color color of text
 *
 * @return SDL_Surface* surface with text on it.
 *
 * @see gfx::Color
 */
SDL_Surface* FL_Font::RenderText_SDL(const char* ptext) const
{
    return this->RenderText_SDL(ptext, gfx::BLACK);
}

/**
 * Render some text with some color, but return an SDL_Surface*.
 *
 * @param char* text to render
 * @param gfx::Color color of text
 *
 * @return SDL_Surface* surface with text on it.
 *
 * @see gfx::Color
 */
SDL_Surface* FL_Font::RenderText_SDL(const char* ptext,
    const gfx::Color& Text_Color) const
{
    if(mp_Data == NULL || ptext == NULL)
        return NULL;

    int skip_size   = TTF_FontLineSkip(mp_Data);
    int height      = this->GetTextHeight(ptext);
    int width       = 0;
    int index       = -1;

    SDL_Surface* pText_Surface = NULL;
    std::vector<std::string> lines;
    std::string tmp(ptext);

    do
    {
        int w = 0;
        index = tmp.find("\n", 0);

        lines.push_back(tmp.substr(0, index));

        if(index != std::string::npos)
        {
            tmp = tmp.substr(index + 1, -1);
        }
        
        TTF_SizeText(mp_Data, lines.back().c_str(), &w, &height);
        
        if(width < w)
            width = w;
    }
    while(index != std::string::npos);

    height = (lines.size() - 1) * skip_size + height;

    pText_Surface = gfx::create_surface_alpha(width, height);
    SDL_Surface* pLine_Text = NULL;

    for(unsigned int i = 0; i < lines.size(); ++i)
    {
        if(lines[i] == "")  // A blank line?
            pLine_Text = TTF_RenderText_Blended(mp_Data, " ", Text_Color);
        else
            pLine_Text = TTF_RenderText_Blended(mp_Data,
                lines[i].c_str(), Text_Color);

        SDL_SetAlpha(pLine_Text, 0, SDL_ALPHA_TRANSPARENT);

        SDL_Rect Rect = {0, i * skip_size, 0, 0};
        SDL_BlitSurface(pLine_Text, NULL, pText_Surface, &Rect);
        SDL_FreeSurface(pLine_Text);
    }

    return pText_Surface;
}

/**
 * Retrieves font size.
 *
 * @return int font size
 */
u_int FL_Font::GetSize() const
{
    return m_size;
}

/**
 * Gets the height, in pixels, of text if rendered using current font.
 *
 * @param char* text to check height for
 *
 * @return int height
 *    Text height usually, -1 if no font loaded.
 */
int FL_Font::GetTextHeight(const char* ptext) const
{
    if(mp_Data == NULL || ptext == NULL)
        return -1;

    int h;
    TTF_SizeText(mp_Data, ptext, NULL, &h);
    return h;
}

/**
 * Gets the width, in pixels, of text if rendered using current font.
 *
 * @param char* text to check width for
 *
 * @return int width
 *    Text width usually, -1 if no font loaded.
 */
int FL_Font::GetTextWidth(const char* ptext) const
{

    if(mp_Data == NULL || ptext == NULL)
        return -1;

    int w;
    TTF_SizeText(mp_Data, ptext, &w, NULL);
    return w;
}

TTF_Font* FL_Font::GetRawFont() const
{
    return mp_Data;
}
