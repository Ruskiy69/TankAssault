#include "GL_Primitives.h"

using gk_gl::CGL_Line;
using namespace gk_gl;

CGL_Line::CGL_Line(GL_Vertex2f Start, GL_Vertex2f End)
{
    SDL_Surface* Surface = gk_sdl::create_surface_alpha(abs(End.x - Start.x), abs(End.y - Start.y));

    int dx, dy, inx, iny, e;
     
    dx = End.x - Start.x;
    dy = End.y - Start.y;
    inx = dx > 0 ? 1 : -1;
    iny = dy > 0 ? 1 : -1;
 
    dx = abs(dx);
    dy = abs(dy);
     
    if(dx >= dy) {
        dy <<= 1;
        e = dy - dx;
        dx <<= 1;
        while (Start.x != End.x)
        {
            this->Pixels.push_back(new GL_Pixel(Start.x, Start.y));
            this->Pixels.back()->Color = gk_gl::GL_BLACK;
            gk_sdl::blit_surface(Surface, gk_sdl::create_surface(
                1, 1, gk_sdl::create_color(WHITE)), Start.x, Start.y);
            if(e >= 0)
            {
                Start.y += iny;
                e-= dx;
            }
            e += dy; Start.x += inx;
        }
    } else {
        dx <<= 1;
        e = dx - dy;
        dy <<= 1;
        while (Start.y != End.y)
        {
            this->Pixels.push_back(new GL_Pixel(Start.x, Start.y));
            this->Pixels.back()->Color = GL_BLACK;
            gk_sdl::blit_surface(Surface, gk_sdl::create_surface(
                1, 1, gk_sdl::create_color(WHITE)), Start.x, Start.y);
            if(e >= 0)
            {
                Start.x += inx;
                e -= dy;
            }
            e += dx; Start.y += iny;
        }
    }
    
    this->Pixels.push_back(new GL_Pixel(Start.x, Start.y));
    this->Pixels.back()->Color = GL_BLACK;
    gk_sdl::blit_surface(Surface, gk_sdl::create_surface(
        1, 1, gk_sdl::create_color(WHITE)), Start.x, Start.y);

    this->SetEntity(Surface);
    SDL_FreeSurface(Surface);

    for(size_t i = 0; i < this->Pixels.size(); i++)
    {
        GL_Rect Rect(this->Pixels[i]->Pos.x, 
            this->Pixels[i]->Pos.y, 1, 1);
        this->CollisionBox.push_back(Rect);
    }
}

bool CGL_Line::CheckCollision(const GL_Rect& Box)
{
    GL_Rect Checker(0, 0, 1, 1);
    for(size_t i = 0; i < this->Pixels.size(); i++)
    {
        Checker.Position = this->Pixels[i]->Pos;
        if(gk_gl::detect_collision(Box, Checker))
            return true;
    }

    return false;
}
