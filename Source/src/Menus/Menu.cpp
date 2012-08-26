/**
 * @file
 *  Implementation of the Menu class.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 */

#include "Menus/Menu.h"

using game::CL_Menu;
using game::g_Log;

/// Cleans up memory allocated for buttons.
CL_Menu::~CL_Menu()
{
    g_Log.Flush();
    g_Log << "[DEBUG] CL_Menu::~CL_Menu called.\n";

    for(size_t i = 0; i < mp_allButtons.size(); ++i)
    {
        delete mp_allButtons[i];
    }

    mp_allButtons.clear();
}

/**
 * Sets an entity as the menu background.
 *
 * @param asset::GL_Entity The background
 */
void CL_Menu::SetBackground(asset::GL_Entity* p_Background)
{
    mp_Background = p_Background;
}

/**
 * Sets a sound to be played when buttons are hovered over.
 *
 * @param asset::AL_Sound2D Hover sound
 */
void CL_Menu::SetHoverSound(asset::AL_Sound2D* p_OnHover)
{
    mp_OnHover = p_OnHover;
}

/**
 * Sets a title to be displayed for the menu
 *
 * @param asset::GL_Entity Menu title
 */
void CL_Menu::SetTitle(asset::GL_Entity* p_Title)
{
    mp_Title = p_Title;
}

/**
 * Adds a menu item to the list of buttons.
 *
 * @param math::ML_Vector2 Button position
 * @param char* Filename of normal button image
 * @param char* Filename of the highlighted (on hover) button image.
 *
 * @return Unique ID for the button.
 */
int CL_Menu::AddMenuItem(const math::ML_Vector2& Position,
    const char* pnormal, const char* phighlighted)
{
    g_Log.Flush();
    g_Log << "[DEBUG] Creating menu item.\n";

    MenuItem* pTmp = new MenuItem;
    
    pTmp->p_Normal  = asset::g_TextureAssets.GetEntityByID(
        asset::g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(pnormal));
    pTmp->p_High    = asset::g_TextureAssets.GetEntityByID(
        asset::g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(phighlighted));

    pTmp->p_Normal->Move(Position);
    pTmp->p_High->Move(Position);

    pTmp->touch = false;
    pTmp->p_Active = pTmp->p_Normal;
    pTmp->id = mp_allButtons.size();

    mp_allButtons.push_back(pTmp);

    return mp_allButtons.back()->id;
}

/**
 * Adds a menu item to a list of buttons.
 *
 * @overload CL_Menu::AddMenuItem(math::ML_Vector2&,
 *      const char* normal, const char* highlighted)
 *
 * @param asset::GL_Entity Normal button entity
 * @param asset::GL_Entity High button entity
 *
 * @return Unique ID of the button.
 */
int CL_Menu::AddMenuItem(asset::asset_id normal, asset::asset_id high)
{
    MenuItem* pTmp = new MenuItem;

    pTmp->p_Normal = asset::g_TextureAssets.GetEntityByID(normal);
    pTmp->p_High = asset::g_TextureAssets.GetEntityByID(high);
    pTmp->p_Active = pTmp->p_Normal;
    pTmp->touch = false;
    pTmp->id = mp_allButtons.size() + 1;

    mp_allButtons.push_back(pTmp);
    return mp_allButtons.back()->id;
}

/**
 * Updates the menu; should be called every frame.
 *
 * @return ID of button if clicked, -1 otherwise.
 */
int CL_Menu::Update()
{
    if(mp_Background != NULL)
        mp_Background->Update();
    if(mp_Title != NULL)
        mp_Title->Update();

    for(size_t i = 0; i < mp_allButtons.size(); ++i)
        mp_allButtons[i]->p_Active->Update();

    return this->CheckMouseClick();
}

/**
 * Checks if the mouse is hovering over any of the buttons.
 *
 * @return ID of button if there's a mouse-click on it.
 */
int CL_Menu::CheckMouseClick()
{
    static int mouse_x, mouse_y;
    game::GetMousePosition(mouse_x, mouse_y);

    for(size_t i = 0; i < mp_allButtons.size(); ++i)
    {
        if(mp_allButtons[i]->p_Active->CheckCollision(mouse_x, mouse_y))
        {
            if(!mp_allButtons[i]->touch)
            {
                mp_allButtons[i]->touch = true;
                mp_OnHover->Play();
            }

            mp_allButtons[i]->p_Active = mp_allButtons[i]->p_High;

            if(game::IsPressed(SDL_BUTTON_LEFT))
                return mp_allButtons[i]->id;
        }
        else
        {
            mp_allButtons[i]->touch = false;
            mp_allButtons[i]->p_Active = mp_allButtons[i]->p_Normal;
        }
    }

    return -1;
}

