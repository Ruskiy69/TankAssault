#ifndef HUD_H
#define HUD_H

#include <string>

#include "SDL/SDL_gfxBlitFunc.h"

#include "Events.h"

#include "Graphics/GFX_Base.h"
#include "Media/MediaManager.h"

namespace Game
{
    typedef unsigned int element_id;
    enum Element_Type { Bar, Score, Menu };

    class Game_HUDElement
    {
    public:
        Game_HUDElement(Element_Type Type, MediaManager::Font& Font, const int initial_value);
        ~Game_HUDElement();

        void OnHover();
        void UpdateValue(const int new_value);
        void Update();

        bool CheckCollision(const int x, const int y);

        void SetName(const std::string& name);
        void SetColor(const GFX::Color& Color);
        void SetBackground(SDL_Surface* Bg);
        void SetHoverEntity(GFX::GL_Entity* OnHover);

        void Move(const MathHelper::Vector2& Position);

        std::string& GetName(){return this->prefix;}
        Element_Type GetType(){return this->Type;}

    protected:
        void CreateBar();                   // Specific to Element_Type::Bar
        void ChangeScore();                 // Specific to Element_Type::Score
        void CreateMenu(SDL_Surface* BG);   // Specific to Element_Type::Menu

        MediaManager::Font& MainFont;
        GFX::GL_Entity* HoverEntity;        // Optional entity shown on hover
        GFX::GL_Entity ElementEntity;       // Main entity shown on HUD
        
        GFX::Color ElementColor;            // Color for all text, or bar

        std::string prefix;                 // For Element_Type::Score, before the actual score        

        Element_Type Type;                  // The type of element (bar, menu, etc)
        int initial_value, value;           // Values
    };

    class Game_HUD
    {
    public:
        Game_HUD() : MenuBG(NULL) {}
        ~Game_HUD();

        void Init(MediaManager::Font& HUDFont, const MediaManager::Font::f_size hover_font_size);

        /* Set a background for menu elements. This creates
         * the effect of a textured background with
         * the menu title centered on top of it.
         * Some re-sizing may occur if the texture is too
         * small for the menu title.
         */
        void SetMenuBackground(SDL_Surface* BG);

        /* Add an element to the HUD overlay. 
         * Returns a unique element_id required for
         * subsequent calls to UpdateHUDElement()
         */
        element_id AddHUDElement(const Element_Type Type,   // What kind of element?
            const MathHelper::Vector2& Position,            // Where?
            const std::string& name,                        // What is displayed? For bars, this is shown on mouse-over 
            const GFX::Color& Color,                        // What color is the text / bar?
            const int initial_value = -1);                  // For bars, this is "full." Subsequent calls to UpdateBar()
                                                            // will decrease based on this amount. 
                                                            // For scores, this is sort of self-explanatory.

        element_id FindHUDElement(const Element_Type Type,  // What kind
            const std::string& name);                       // What's it "called"?

        /* Move an element to a certain location. */
        void MoveHUDElement(const element_id, const MathHelper::Vector2& Position);

        /* Updates the {Bar|Score} with the new_value.
         *
         * For bars, this adjusts the {Color} of the bar
         * to fill a certain amount, dependent on the
         * initial_value set with the AddHUDElement() call.
         *
         * For scores, this sets the score to new_value.
         */
        void UpdateElement(const element_id id, const int new_value);

        /* Update all of the HUD elements. Checks for
         * clicks and hovers on all of them as well.
         * Returns the element_id if there was a click,
         * returns -1 otherwise.
         */
        int Update();

    private:
        int CheckMouseClick();

        MediaManager::Font* HUDFont;
        MediaManager::Font::f_size hud_font_size, hover_font_size;

        SDL_Surface* MenuBG;
        std::vector<Game_HUDElement*> Elements;        
    };
}

#endif // HUD_H