#ifndef __GL_PRIMITIVES_H__
#define __GL_PRIMITIVES_H__

#include "SDL_GL_Base.h"
#include "GL_Entity.h"

namespace gk_gl
{
    class CGL_Line : public CGL_Entity
    {
    public:
        CGL_Line(GL_Vertex2f Start, GL_Vertex2f End);
        bool CheckCollision(const GL_Rect& Box);
        std::vector<GL_Rect>& GetCollisionBox(){return this->CollisionBox;}

    private:
        std::vector<GL_Pixel*> Pixels;
        std::vector<GL_Rect> CollisionBox;
    };
}

#endif // __GL_PRIMITIVES_H__