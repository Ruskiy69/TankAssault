#ifndef __GL_MENU_H__
#define __GL_MENU_H__

#include <vector>

#include "GenericEvents.h"

#include "Math/MathHelper.h"
#include "Media/MediaManager.h"
#include "Graphics/GFX_Base.h"
#include "Graphics/GL_Entity.h"

namespace Game
{
    class GL_Menu
    {
    public:
        GL_Menu();
        ~GL_Menu();

        /* Basic set up of the menu. */
        void SetBackground(const char* filename);
        void SetBackground(GFX::GL_Entity& Background);

        void SetHoverSound(const char* filename);
        void SetHoverSound(MediaManager::SoundEffect& On_Hover);

        void SetTitle(const char* font_name, const int size,
            const SDL_Color& Color, const MathHelper::Vector2& Position,
            const char* title);
        void SetTitle(GFX::GL_Entity& Title);
        void SetTitle(const MediaManager::Font& MenuFont, const GFX::Color& Color,
            const MathHelper::Vector2& Position, const char* title);

        void Shift(const int pixels);

        /* Add items to the menu, returns unique menu button id */
        int AddMenuItem(const MathHelper::Vector2& Position, const char* normal, const char* highlighted);
        int AddMenuItem(GFX::GL_Entity& Normal, GFX::GL_Entity& Highlighted);

        /* Music controls */
        void PlayMusic();
        void ResumeMusic();
        void PauseMusic();
        bool IsPlaying(){return this->playing_music;}

        int GetOffset(){return this->offset;}

        /* Render everything, returns menu id or -1 if none */
        int Update();

    private:
        /* Check if mouse is on any button, 
         * if it is, return id, -1 if not.
         */
        int CheckMouseover();

        struct GL_MenuItem
        {
            int id;                 // Unique button id

            bool touch;             // Is the button being hovered over.
                                    // This is mostly for the On_Hover sound
            GFX::GL_Entity* Normal; // Without mouse-over
            GFX::GL_Entity* High;   // On mouse-over
            GFX::GL_Entity* Active; // Currently showing (points to either Normal or Active)
        };

        std::vector<GL_MenuItem*> Buttons;

        GFX::GL_Entity* Title;
        GFX::GL_Entity* Background;

        MediaManager::SoundEffect* On_Hover;

        bool    playing_music;
        int     offset;             // From Shift()
    };
}

#endif // __GL_MENU_H__