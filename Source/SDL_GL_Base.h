#ifndef __SDL_GL_BASE_H__
#define __SDL_GL_BASE_H__

/* Math header for trig functions */
#include <cmath>

/* OpenGL headers */
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* SOIL library for easy image loading */
#include "SOIL/SOIL.h"

/* SDL headers */
#include "SDL_Base.h"
#include "SDL/SDL_OpenGL.h"

/* Error handling */
#include "Errors.h"

/* Link libraries */
#pragma comment(lib, "SOIL.lib")
//#pragma comment(lib, "DataLib.lib")

/* Conversion from radians to degrees */
#define RAD(x) x * PI / 180.0f
#define DEG(x) x * 180.0f / PI
#define PI 3.141592653f

namespace gk_gl
{
    typedef unsigned int gl_texture;

    struct GL_Vertex2f
    {
        GL_Vertex2f(int x, int y): x(1.0f*x), y(1.0f*y){}
        GL_Vertex2f(){}
        float x, y;
    };

    struct GL_Rect
    {
        GL_Rect(){}
        GL_Rect(int x, int y, int w, int h) : Position(x, y), w(w), h(h){}
        GL_Rect(const GL_Vertex2f& Pos, int w, int h) : Position(Pos), w(w), h(h){}
        GL_Vertex2f Position;
        unsigned int w, h;
    };
    
    struct GL_Color3f
    {
        GL_Color3f(){}
        GL_Color3f(float r, float g, float b) : r(r), g(g), b(b){}
        float r, g, b;
    };

    struct GL_Pixel
    {
        GL_Pixel(){}
        GL_Pixel(int x, int y) : Pos(x, y){}
        GL_Vertex2f Pos;
        GL_Color3f Color;
    };

    bool detect_collision(const GL_Rect& One, const GL_Rect& Two);
    
    SDL_Rect GL_to_SDL_Rect(const GL_Rect& Rect);
    GL_Rect SDL_to_GL_Rect(const SDL_Rect& Rect);
    gl_texture SDL_Surface_to_texture(SDL_Surface* Source);
    gl_texture SDL_Surface_to_texture_reload(SDL_Surface* Source, const unsigned int old_texture);
    
    gl_texture LoadTexture(const char* filename, const int flags = SOIL_FLAG_INVERT_Y);
    gl_texture ReloadTexture(const char* filename, const int old_texture, const int flags = SOIL_FLAG_INVERT_Y);

    static const GL_Color3f GL_BLACK(0, 0, 0);
}

#endif // __SDL_GL_BASE_H__

