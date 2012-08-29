/**
 * @file
 *  GL_Window class definitions.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 */

#include "Window.h"

using gfx::GL_Window;
using game::g_Log;

/**
 * Creates an OpenGL window.
 *
 * @param int window width
 * @param int window height
 * @param char* (optional) text for window title-bar
 * @param char* (optional) filename for window icon image
 */
GL_Window::GL_Window(const int w, const int h, const char* ptitle,
    const char* picon) : m_width(w), m_height(h), m_fs(false)
{
    g_Log.Flush();
    g_Log << "[INFO] Creating OpenGL window " << w << "x" << h << ".\n";

    // Full-screen only in the release version
#ifdef _DEBUG
    this->Init();
#else
    this->ToggleFullscreen();
#endif // _DEBUG

    // Record OpenGL version.
    const char* pstr = (char*)glGetString(GL_VERSION);
    g_Log.Flush();
    g_Log << "[INFO] OpenGL version: " << pstr << ".\n";
    pstr = (char*)glGetString(GL_VENDOR);
    g_Log << "[INFO] OpenGL vendor: " << pstr << ".\n";
    pstr = (char*)glGetString(GL_RENDERER);
    g_Log << "[INFO] OpenGL renderer: " << pstr << ".\n";

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
 *
 * If toggling, all OpenGL textures loaded up until this point MUST be reloaded,
 * because this creates a new OpenGL context and invalidates all previous operations.
 *
 * @return bool current window state, TRUE if full screen, FALSE otherwise
 */
bool GL_Window::ToggleFullscreen()
{
    g_Log.Flush();
    g_Log << "[INFO] Creating OpenGL window in full-screen mode.\n";

    m_fs = !m_fs;
    this->Init(SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | (m_fs ? SDL_FULLSCREEN : 0));
    return m_fs;
}

/**
 * Updates everything that was rendered on the OpenGL context till this point.
 */
void GL_Window::Update()
{
    SDL_GL_SwapBuffers();
}

/**
 * Clears the screen.
 */
void GL_Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Retreives window height.
 *
 * @return int height
 */
int GL_Window::GetHeight() const
{
    return m_height;
}

/**
 * Retreives window width.
 *
 * @return int width
 */
int GL_Window::GetWidth() const
{
    return m_width;
}

/** 
 * Sets up OpenGL and creates a window.
 *
 * @param int window flags
 */
void GL_Window::Init(const int flags)
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
