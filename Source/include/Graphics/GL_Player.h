#ifndef PLAYER_H
#define PLAYER_H

#include "SDL/SDL_mixer.h"

#include "Timer.h"

#include "Math/MathHelper.h"

#include "Graphics/GFX_Base.h"
#include "Graphics/GL_Entity.h"
#include "Graphics/GL_Bullet.h"

#include "Media/SoundEffect.h"

namespace Game
{
    class GL_Player: public GFX::GL_Entity
    {
    public:
        GL_Player(const Game::Game_Timer& Timer);
        ~GL_Player();

        void RotateBody(const float degrees);
        void RotateTower(const float degrees);
        void Move(const float speed);
        void Move(const float x, const float y)
        {
            this->GL_Entity::Move(x, y);
        }

        void Shoot(Game::GL_Bullets& Bullets);

        void Render();
        void Update();

        bool IsMoving() const{return this->moving;}
        MathHelper::Vector2& GetSpeed(){return this->Rate;}

        static const int PLAYER_SPEED = 3;

    private:
        static const int SHOT_DELAY = 30;
        MediaManager::SoundEffect Shot;
        MediaManager::SoundEffect MoveSound;

        MathHelper::Vector2 Rate;
        MathHelper::Vector2 Barrel_Rate;

        MathHelper::Vector2 Barrel;
        float facing_degree;
        bool moving;
        int shot_delay;
        int mv_sound_channel;
    };
}

#endif // PLAYER_H