/**
 * @file
 *  Declarations of the CMenuManager class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1.2
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
 **/
/// @{

#ifndef MENUS__MENU_MANAGER_HPP
#define MENUS__MENU_MANAGER_HPP

#include "Errors.hpp"
#include "CollapseDef.hpp"
#include "GameEvents.hpp"

#include "Graphics/Window.hpp"
#include "Assets/AssetManager.hpp"
#include "Assets/MusicPlayer.hpp"
#include "Assets/Font.hpp"
#include "Assets/Texture.hpp"
#include "Assets/Sound2D.hpp"
#include "Menus/Menu.hpp"

namespace game
{
    /**
     * Takes care of any menu functions such as switching between them,
     * registering events, or toggling settings.
     **/
    class CMenuManager
    {
    public:
        CMenuManager(gfx::CWindow& Window, GameState& Engine_State) :
            m_Window(Window), m_state(Engine_State),
            mp_ActiveMenu(NULL) {}

        void Init();

        bool FadeOut(float rate = 0.01f);
        bool FadeIn(float rate = 0.01f);

        void Update(asset::CMusicPlayer& Music);

    private:
        void MainMenu();
        void OptionsMenu(asset::CMusicPlayer& Music);
        void CreditsMenu();
        void PauseMenu();

        gfx::CWindow&       m_Window;

        game::CMenu*        mp_ActiveMenu;
        game::CMenu         m_MainMenu;
        game::CMenu         m_OptionsMenu;
        game::CMenu         m_PauseMenu;

        obj::CEntity        m_MainMenuBackground;
        obj::CEntity        m_PauseMenuBackground;
        obj::CEntity*       mp_MenuTitle;
        obj::CEntity*       mp_PauseTitle;

        asset::CFont*       mp_MenuFont;
        asset::CSound2D*    mp_OnHover;

        GameState&          m_state;
    };
}

#endif // MENUS__MENU_MANAGER_HPP

/// @}