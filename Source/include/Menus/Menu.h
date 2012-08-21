/**
 * @file
 *  Declarations for the CL_Menu class.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Game
 */
/// @{

#ifndef MENU_H
#define MENU_H

#include <vector>

#include "SystemEvents.h"
#include "Graphics.h"

#include "Math/Math.h"
#include "Assets/AssetManagers.h"

namespace game
{
    /// A versatile menu class.
    class CL_Menu
    {
    public:
        CL_Menu() : mp_Background(NULL), mp_Title(NULL), mp_OnHover(NULL){}
        ~CL_Menu();
    
        void SetBackground(asset::GL_Entity* p_Background);
        void SetHoverSound(asset::AL_Sound2D* p_OnHover);
        void SetTitle(asset::GL_Entity* Title);

        int AddMenuItem(const math::ML_Vector2& Position,
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

    private:
        int CheckMouseClick();

        struct MenuItem
        {
            /// Unique button id
            int id;

            /**
             * Is the button being hovered on?
             * This is for CL_Menu::CheckMouseClick()
             */
            bool touch;

            /// Normal button when mouse is away
            asset::GL_Entity* p_Normal;

            /// Highlighted button when mouse is on
            asset::GL_Entity* p_High;

            /// Pointer to the currently active button
            asset::GL_Entity* p_Active;
        };

        std::vector<MenuItem*> mp_allButtons;

        asset::GL_Entity*   mp_Title;
        asset::GL_Entity*   mp_Background;

        asset::AL_Sound2D*  mp_OnHover;
    };
}

#endif // MENU_H
/// @}