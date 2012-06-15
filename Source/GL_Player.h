#ifndef PLAYER_H
#define PLAYER_H

#include "SDL/SDL_mixer.h"

#include "SDL_GL_Base.h"
#include "GL_Display.h"
#include "GL_Entity.h"

#include "Bullet.h"
#include "Timer.h"

namespace gk_gl
{
    class CGL_Player: public gk_gl::CGL_Entity
    {
    public:
        CGL_Player(gk::CTimer& Timer);
        ~CGL_Player();

        void RotateBody(const float degrees);
        void RotateTower(const float degrees);
        void Move(const float speed);
        void Move(const float x, const float y)
        {
            this->CGL_Entity::Move(x, y);
        }
    
        void Shoot(gk_gl::CGL_Bullets& Bullets);

        void Render();
        void Update();

        bool IsMoving() const{return this->moving;}
        GL_Vertex2f& GetSpeed(){return this->Rate;}

    private:
        static const int SHOT_DELAY = 30;
        Mix_Chunk* Shot;
    
        gk_gl::GL_Vertex2f Rate;
        gk_gl::GL_Vertex2f Barrel_Rate;

        gk_gl::GL_Vertex2f Barrel;
        float facing_degree;
        bool moving;
        int shot_delay;
    };
}

#endif // PLAYER_H