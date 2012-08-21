/**
 * @file
 *  Declarations of the MenuManager class.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 *
 * @addtogroup Game
 */
/// @{

#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Errors.h"
#include "CollapseDef.h"

#include "Window.h"

#include "Assets/Entity.h"
#include "Assets/Font.h"
#include "Assets/Sound2D.h"

#include "Menus/Menu.h"

namespace game
{
    /**
     * Takes care of any menu functions such as switching between them,
     * registering events, or toggling settings.
     */
    class CL_MenuManager
    {
    public:
        CL_MenuManager(gfx::GL_Window& Window, CL_GameState& Engine_State) : 
            m_Window(Window), m_state(Engine_State), mp_ActiveMenu(NULL) {}

        void Init();

        void MainMenu();
        void OptionsMenu();//media::MusicPlayer& Music);
        void CreditsMenu();
        void PauseMenu();

        bool FadeOut(float rate = 0.01f);
        bool FadeIn(float rate = 0.01f);

    private:
        gfx::GL_Window&     m_Window;

        game::CL_Menu*      mp_ActiveMenu;
        game::CL_Menu       m_MainMenu;
        game::CL_Menu       m_OptionsMenu;
        game::CL_Menu       m_PauseMenu;

        asset::GL_Entity*   mp_MainMenuBackground;
        asset::GL_Entity*   mp_PauseMenuBackground;
        asset::GL_Entity*   mp_MenuTitle;
        asset::GL_Entity*   mp_PauseTitle;

        asset::FL_Font*     mp_MenuFont;
        asset::AL_Sound2D*  mp_OnHover;

        CL_GameState&       m_state;
    };
}

#endif // MENU_MANAGER_H

/// @}