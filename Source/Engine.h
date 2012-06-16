#ifndef ENGINE_H
#define ENGINE_H

#include <cstdlib>
#include <ctime>

#include <fstream>
#include <string>
#include <iostream>

#include "SDL_GL_Base.h"

#include "Timer.h"
#include "Settings.h"

#include "Bullet.h"
#include "Map.h"

#include "GL_Display.h"
#include "GL_Menu.h"

#include "GL_Entity.h"
#include "GL_Primitives.h"
#include "GL_Player.h"
#include "GL_Enemy.h"

namespace gk
{
    class CEngine
    {
    public:
        CEngine();
        ~CEngine();

        void Run();

    private:
        void HandleEvents(int&, int&);
        void Logic(int&, int&);
        void RenderAll();
    
        enum GameState { Play, Menu, Options, Quit } State;

        gk_gl::CGL_Display  Display;
        gk_gl::CGL_Entity   Background;
        gk_gl::CGL_Menu     MainMenu;
        gk_gl::CGL_Menu     OptionsMenu;
        gk_gl::CGL_Player   Player;
        gk_gl::CGL_Bullets  Bullets;

        gk::CTimer          Timer;
        gk::CSettings       Settings;
        gk::CMap            Map;

        TTF_Font*           Menu_Font;

        gk_gl::CGL_Enemies  Enemies;

        bool                quit;
    };
}

#endif // ENGINE_H