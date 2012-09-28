/**
 * @file
 *	Declaration of the CEngine class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.2
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Game
 */
/// @{

#ifndef ENGINE_HPP
#define ENGINE_HPP

// Enable this in release build no matter what,
// but otherwise, let me choose.
#ifndef _DEBUG
 #define REGULATE_FPS 1
#else
 #define REGULATE_FPS 1
#endif // _DEBUG

#include "Errors.hpp"
#include "Helpers.hpp"
#include "CollapseDef.hpp"
#include "SystemEvents.hpp"
#include "GameEvents.hpp"
#include "Timer.hpp"
#include "Inventory.hpp"

#include "Math/Math.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"
#include "Assets/AssetManager.hpp"
#include "Assets/Texture.hpp"
#include "Assets/Sound2D.hpp"
#include "Assets/Font.hpp"
#include "Menus/MenuManager.hpp"
#include "World/World.hpp"

#define GAME_VERSION "VERSION 0.8.2 pre-release"

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

    class CEngine
    {
    public:
    	CEngine();
    	~CEngine(){}

        bool Init();
        bool GameLoop();

    private:
        void HandleGameEvents();
        void HandleSystemEvents();
        void Intro();

        gfx::CWindow        m_GameWindow;
        gfx::CShader        m_LightingShader;
        gfx::Color          m_OffBlue;

        asset::CSound2D*    mp_IntroSong;
        asset::CFont*       mp_IntroFont;
        asset::CMusicPlayer m_MusicPlayer;

        obj::CEntity        m_IngameCursor;
        obj::CEntity        m_Splash;
        obj::CEntity*       mp_Version;

        game::CMenuManager  m_Menus;
        game::CWorld        m_World;
        game::CTimer        m_Timer;
        game::CInventory    m_Inventory;
        game::GameState     m_state;
    };
}

#endif // ENGINE_HPP

/// @}