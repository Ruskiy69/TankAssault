/**
 * @file
 *  CWindow class definitions.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 **/

#include "Graphics/Window.hpp"

using gfx::CWindow;
using game::g_Log;

/**
 * Creates an OpenGL window.
 *
 * @param int window width
 * @param int window height
 * @param char* (optional) text for window title-bar
 * @param char* (optional) filename for window icon image
 **/
CWindow::CWindow(const int w, const int h, const char* ptitle,
    const char* picon) : m_width(w), m_height(h), m_fs(false)
{
    g_Log.Flush();
    g_Log << "[INFO] Creating OpenGL window " << w << "x" << h << ".\n";

    // Full-screen only in the release version.
#ifdef _DEBUG
    this->Init();
#else
    this->ToggleFullscreen();
#endif // _DEBUG

    // Record OpenGL data.
    const char* pstr = (char*)glGetString(GL_VERSION);
    g_Log.Flush();
    g_Log << "[INFO] OpenGL version: " << pstr << ".\n";
    pstr = (char*)glGetString(GL_VENDOR);
    g_Log << "[INFO] OpenGL vendor: " << pstr << ".\n";
    pstr = (char*)glGetString(GL_RENDERER);
    g_Log << "[INFO] OpenGL renderer: " << pstr << ".\n";

    // Check for supported OpenGL version (minimum 2.1)
    GLint maj_v, min_v;
    glGetIntegerv(GL_MAJOR_VERSION, &maj_v);
    glGetIntegerv(GL_MINOR_VERSION, &min_v);
    GLfloat v = maj_v + (min_v / 10.0f);

    if(v < 2.1)
    {
        g_Log.Flush();
        g_Log << "[ERROR] Unsupported OpenGL version.\n";
        g_Log << "[ERROR] Your version: " << v << "\n";
        g_Log << "[ERROR] Minimum version: 2.1\n";
        g_Log << "[ERROR] Please update your video drivers.\n";
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    SDL_WM_SetCaption(ptitle, NULL);

    if(picon)
    {
        SDL_Surface* pIcon_Surface = gfx::load_image(picon);
        pIcon_Surface = gfx::optimize_surface_alpha(pIcon_Surface);
        SDL_WM_SetIcon(pIcon_Surface, NULL);
    }
}

/**
 * Toggles full screen mode.
 *  If toggling, all OpenGL textures loaded up until this point MUST be
 *  reloaded, because this creates a new OpenGL context and invalidates'
 *  all previous operations.
 *
 * @return Current window state, TRUE if full screen, FALSE otherwise.
 **/
bool CWindow::ToggleFullscreen()
{
    g_Log.Flush();
    g_Log << "[INFO] Creating OpenGL window in full-screen mode.\n";

    m_fs = !m_fs;
    this->Init(SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | (m_fs ? SDL_FULLSCREEN : 0));
    return m_fs;
}

/// Updates everything that was rendered on the OpenGL context till this point.
void CWindow::Update()
{
    SDL_GL_SwapBuffers();
}

/// Clears the screen.
void CWindow::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Retrieves window height.
 * @return int height
 **/
int CWindow::GetHeight() const
{
    return m_height;
}

/**
 * Retrieves window width.
 * @return Width
 **/
int CWindow::GetWidth() const
{
    return m_width;
}

/** 
 * Sets up OpenGL and creates a window.
 * @param int Window flags
 **/
void CWindow::Init(const int flags)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

    mp_Display = SDL_SetVideoMode(m_width, m_height, 32, flags);

    if(!mp_Display)
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to create OpenGL window!" << ".\n";
        g_Log << "[ERROR] SDL error: " << SDL_GetError() << ".\n";
        
        gk::handle_error(g_Log.GetLastLog().c_str());   
    }

    glClearColor(0, 0, 0, 0);

    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, m_height, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
}
