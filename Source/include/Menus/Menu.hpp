/**
 * @file
 *  Declarations for the CMenu class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1
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

#ifndef MENUS__MENU_HPP
#define MENUS__MENU_HPP

#include <vector>

#include "SystemEvents.hpp"

#include "Math/Math.hpp"
#include "Graphics/Graphics.hpp"
#include "Assets/AssetManager.hpp"
#include "Assets/Sound2D.hpp"
#include "World/Objects/GameObject.hpp"

namespace game
{
    /// A versatile menu class.
    class CMenu
    {
    public:
        struct MenuItem
        {
            /// Unique button id
            int id;

            /**
             * Is the button being hovered on?
             * This is for CMenu::CheckMouseClick()
             **/
            bool touch;

            /// Normal button when mouse is away
            obj::CGameObject* p_Normal;

            /// Highlighted button when mouse is on
            obj::CGameObject* p_High;

            /// Pointer to the currently active button
            obj::CGameObject* p_Active;
        };

        CMenu() : mp_Background(NULL), mp_Title(NULL), mp_OnHover(NULL){}
        ~CMenu();
    
        int AddMenuItem(const math::CVector2& Position,
            const char* pnormal,
            const char* phighlighted);
        int AddMenuItem(asset::asset_id Normal, asset::asset_id Highlighted);

        void ChangeMenuItem(const int item_id, const char* pnormal,
            const char* phighlighted);

        void PlayMusic();
        void ResumeMusic();
        void PauseMusic();
        bool MusicIsPlaying();

        int Update();

        void SetBackground(obj::CEntity* p_Background);
        void SetHoverSound(asset::CSound2D* p_OnHover);
        void SetTitle(obj::CEntity* Title);

        const std::vector<MenuItem*>& GetButtons() const;
        obj::CEntity* const GetTitle() const;
        obj::CEntity* const GetBackground() const;

    private:
        int CheckMouseClick();

        std::vector<MenuItem*> mp_allButtons;

        obj::CEntity*       mp_Title;
        obj::CEntity*       mp_Background;

        asset::CSound2D*    mp_OnHover;
    };
}

#endif // MENU_H
/// @}