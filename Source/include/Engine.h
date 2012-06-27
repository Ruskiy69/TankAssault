#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <string>

#include "Errors.h"
#include "Helpers.h"

#include "Media/MediaManager.h"
#include "Graphics/GL_Display.h"
#include "Math/MathHelper.h"

#include "Graphics/GL_Menu.h"
#include "Graphics/GL_Player.h"
#include "Graphics/HUD.h"

#include "Timer.h"
#include "Events.h"
#include "World.h"

namespace Game
{
    static const std::string INTRO_STR = \
        "I was there when the bombs fell\n"
        "When mankind fell with them\n"
        "Retreated from our own creation\n"
        "Nobody had expected them to turn on us\n"
        "Nobody had prepared for a war\n"
        "They didn't take prisoners\n"
        "They didn't make bargains\n"
        "By the end, the earth was destroyed,\n"
        "Devoid of life, dominated by metal and gears.\n"
        "And the machines became the new rulers...\n"
        "I am Sgt. Maksimov,\n"
        "And this is my last mission.\n";

    static const std::string INTRO_STR2 = "Year 2200 - 6 Days after Collapse";

    class Game_Engine
    {
    public:
        Game_Engine();
        ~Game_Engine();
        void Run();

    private:
        void Setup();
        void GameLoop();
        void GameIntro();

        enum MainIndex  { main_play_id, main_options_id, /* main_credits_id, */ main_quit_id };
        enum OptIndex   { opt_music_toggle_id, opt_ret_id };
        enum PauseIndex { /*pause_opt_id,*/ pause_ret_id, pause_quit_id };

        MediaManager::MusicPlayer   MusicPlayer;
        MediaManager::Font          MainFont;
        MediaManager::Font          MenuFont;

        GFX::GL_Display     Window;
        Game::Game_Timer    Timer;
        
        Game::GL_Menu       MainMenu;
        Game::GL_Menu       OptionsMenu;
        Game::GL_Menu       PauseMenu;

        Game::Game_Events   Events;
        Game::Game_World    World;
        
        Game::Game_HUD      HUD;

        //Game_AI           MasterAI;
        GFX::GL_Entity      Background;
        GFX::GL_Entity      Light;
        
        std::vector<int>    Main_Menu_ids;
        std::vector<int>    Opt_Menu_ids;
        std::vector<int>    Pause_Menu_ids;

        Game_Events::Game_State& Game_State;
    };
}

#endif // ENGINE_H