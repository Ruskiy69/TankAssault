#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include <string>
#include <iostream>

#include "SDL_GL_Base.h"
#include "CGL_Display.h"
#include "CGL_Menu.h"

#include "CGL_Entity.h"
#include "CGL_Player.h"

#include "Timer.h"
#include "Settings.h"

#include "Bullet.h"
#include "Map.h"

class CEngine
{
public:
    CEngine();
    ~CEngine(){}

    void Run();

private:
    void HandleEvents(int&, int&);
    void Logic(int&, int&);
    void RenderAll();
    
    enum GameState { Play, Menu, Quit } State;

    gk_gl::CGL_Display Display;
    gk_gl::CGL_Entity  Background;
    gk_gl::CGL_Menu    MainMenu;
    gk_gl::CGL_Player  Player;
    gk_gl::CGL_Bullets Bullets;

    CTimer      Timer;
    CSettings   Settings;
    gk::CMap    Map;

    TTF_Font*   Menu_Font;

    bool        quit;
};

#endif // ENGINE_H