#ifndef WORLD_H
#define WORLD_H

#include "Timer.h"
#include "Map.h"

#include "Graphics/GL_Display.h"
#include "Graphics/GL_Bullet.h"
#include "Graphics/GL_Player.h"

namespace Game
{
    class Game_World
    {
    public:
        Game_World(const Game_Timer& Timer, const GFX::GL_Display& Window) :
          Window(Window), Player(Timer) {}
        ~Game_World();

        bool LoadMap(const char* filename);

        bool MovePlayer(const float speed);
        bool RotatePlayer(const float angle);
        bool Shoot();

        void Update();

        const GL_Player& GetPlayer() const;

    private:
        Game_Map    Map;
        GL_Player   Player;
        GL_Bullets  Bullets;
        const GFX::GL_Display& Window;
    };
}


#endif // WORLD_H