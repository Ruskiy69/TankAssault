#include "Assets/Texture.hpp"

using asset::CTexture;

bool CTexture::LoadFromFile(const char* pfilename)
{
    if((m_texture = gfx::load_texture(pfilename)) <= 0)
        return false;
    else
    {
        int w, h;

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_Size.Resize(w, h);
        m_filename  = pfilename;
        m_loaded    = true;
        return true;
    }
}

GLuint CTexture::GetTexture() const
{
    return m_texture;
}

GLint CTexture::GetW() const
{
    return m_Size.w;
}

GLint CTexture::GetH() const
{
    return m_Size.h;
}

bool CTexture::LoadFromSurface(SDL_Surface* pSurface)
{
    if(pSurface == NULL) return false;

    if((m_texture = gfx::SDL_Surface_to_texture(pSurface)) < 0)
        return false;
    else
    {
        int w, h;

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_Size.Resize(w, h);
        m_loaded = true;
        return true;
    }
}

void CTexture::Resize(const u_int w, const u_int h)
{
    m_Size.Resize(w, h);
}
