#include "Graphics/GL_Display.h"

using namespace GFX;

GL_Display::GL_Display(const int w, const int h, const char* title, const char* icon): width(w), height(h), fs(false)
{
    /* If you want fullscreen off the bat, 
     * call this:
     */
    this->ToggleFullscreen();
    /* Otherwise, you wanna just call
     * the basic Init() method.
     */
    //this->Init();
    SDL_WM_SetCaption(title, NULL);

    if(icon)
        SDL_WM_SetIcon(load_image_alpha(icon), NULL);
}

bool GL_Display::ToggleFullscreen()
{
    this->fs = !this->fs;
    this->Init(SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | (this->fs ? SDL_FULLSCREEN : 0));
    return this->fs;
}

void GL_Display::Update()
{    
    SDL_GL_SwapBuffers();
}

void GL_Display::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int GL_Display::GetHeight() const
{
    return this->height;
}

int GL_Display::GetWidth() const
{
    return this->width;
}

void GL_Display::Init(const int flags)
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

    this->Display = SDL_SetVideoMode(this->width, this->height, 32, flags);

    if(!this->Display)
        gk::handle_error(SDL_GetError());

    glClearColor(0, 0, 0, 0);

    glViewport(0, 0, this->width, this->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, this->width, this->height, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
}
