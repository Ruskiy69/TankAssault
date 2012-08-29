/**
 * @file
 *  Game engine. 
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Game
 *  All of the game-specific implementations.
 */
/// @{

/// For testing frame rate impact
#define REGULATE_FPS    1

#include "CollapseDef.h"
#include "Errors.h"
#include "SystemEvents.h"

#include "Timer.h"

#include "Math/Math.h"

#include "Lighting.h"
#include "Graphics.h"
#include "Window.h"

#include "Assets/AssetManagers.h"
#include "Assets/MusicPlayer.h"
#include "Menus/MenuManager.h"

#include "World/World.h"

namespace game
{
    /// All of the lines in the intro sequence.
    static const int INTRO_STR_SIZE = 11;
    static const char* const INTRO_STR[INTRO_STR_SIZE] = 
    {
        "I was there when the bombs fell\n",
        "When mankind fell with them\n",
        "Retreated from our own creation\n",
        "Nobody had expected them to turn on us\n",
        "Nobody had prepared for a war\n",
        "They didn't take prisoners\n",
        "They didn't make bargains\n",
        "Earth became a barren wasteland,\n",
        "Ruthlessly ruled by the machines\n",
        "I am the last surviving human,\n",
        "And this is my revenge.\n"
    };

    /**
     * Handles all high-level game functionality. Loads
     * menus, assets, and settings. Executes game logic.
     */
    class CL_Engine
    {
    public:
        CL_Engine() : m_Window(800, 600, "Collapse", "Data/Images/tank.ico"),
            m_Menus(m_Window, m_state), m_World(m_state)
        {
        }

        void Init();
        void GameLoop();

    private:
        void Intro();
        void HandleSystemEvents();

        gfx::GL_Window          m_Window;
        gfx::GL_Lighting        m_Lighting;
        
        asset::GL_Entity*       mp_Cursor;
        asset::AL_MusicPlayer   m_MusicPlayer;

        // Assets used by intro/splash screens
        asset::asset_id         m_introfont_id;
        asset::asset_id         m_introsong_id;
        gfx::Color              m_OffBlue;

        game::CL_Timer          m_Timer;
        game::CL_MenuManager    m_Menus;
        game::CL_World          m_World;

        CL_GameState            m_state;
    };
}

/// @}