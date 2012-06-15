#ifndef __GL_MENU_H__
#define __GL_MENU_H__

#include <vector>

#include "SDL_GL_Base.h"
#include "SDL_Base.h"
#include "GL_Entity.h"

#include "Events.h"

namespace gk_gl
{
    class CGL_Menu
    {
    public:
        CGL_Menu();
        ~CGL_Menu();
        
        /* Basic set up of the menu. */
        void SetBackground(const char* filename);
        void SetBackground(CGL_Entity& Background);
        
        void SetMusic(const char* filename);
        void SetMusic(Mix_Music* Music);
        
        void SetHoverSound(const char* filename);
        void SetHoverSound(Mix_Chunk* On_Hover);
        
        void SetTitle(const char* font_name, const int size,
            const SDL_Color& Color, const GL_Vertex2f& Position,
            const char* title);
        void SetTitle(CGL_Entity& Title);
        void SetTitle(TTF_Font* Font, const SDL_Color& Color,
            const GL_Vertex2f& Position, const char* title);
        
        /* Add items to the menu, returns unique menu button id */
        int AddMenuItem(const GL_Vertex2f& Position, const char* normal, const char* highlighted);
        int AddMenuItem(CGL_Entity& Normal, CGL_Entity& Highlighted);
        
        /* Music controls */
        void PlayMusic();
        void ResumeMusic();
        void PauseMusic();
        bool IsPlaying(){return this->playing_music;}
        
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
            CGL_Entity* Normal;     // Without mouse-over
            CGL_Entity* High;       // On mouse-over
            CGL_Entity* Active;     // Currently showing (points to either Normal or Active)
        };
        
        std::vector<GL_MenuItem*> Buttons;
        
        CGL_Entity* Title;
        CGL_Entity* Background;
        
        Mix_Music*  Menu_Music;
        Mix_Chunk*  On_Hover;
        
        bool        playing_music;
    };
}

#endif // __GL_MENU_H__