/**
 * @file
 *  Implementation of the CMenu class.
 *
 * @author George Kudrayvtsev
 * @version 1.0.1
 **/

#include "Menus/Menu.hpp"

using game::CMenu;
using game::g_Log;
using asset::CAssetManager;

/// Cleans up memory allocated for buttons.
CMenu::~CMenu()
{
    g_Log.Flush();
    g_Log << "[DEBUG] CMenu::~CMenu called.\n";

    for(size_t i = 0; i < mp_allButtons.size(); ++i)
    {
        delete mp_allButtons[i];
    }

    mp_allButtons.clear();
}

/**
 * Sets an entity as the menu background.
 * @param obj::CEntity The background
 **/
void CMenu::SetBackground(obj::CEntity* p_Background)
{
    mp_Background = p_Background;
}

/**
 * Sets a sound to be played when buttons are hovered over.
 * @param asset::AL_Sound2D Hover sound
 **/
void CMenu::SetHoverSound(asset::CSound2D* p_OnHover)
{
    mp_OnHover = p_OnHover;
}

/**
 * Sets a title to be displayed for the menu
 * @param obj::CEntity Menu title
 **/
void CMenu::SetTitle(obj::CEntity* p_Title)
{
    mp_Title = p_Title;
}

/**
 * Adds a menu item to the list of buttons.
 *
 * @param math::CVector2 Button position
 * @param char* Filename of normal button image
 * @param char* Filename of the highlighted (on hover) button image.
 *
 * @return Unique ID for the button.
 **/
int CMenu::AddMenuItem(const math::CVector2& Position,
    const char* pnormal, const char* phighlighted)
{
    MenuItem* pTmp  = new MenuItem;
    
    pTmp->p_Normal  = new obj::CGameObject;
    pTmp->p_High    = new obj::CGameObject;
    pTmp->p_Normal->LoadFromTexture(
        CAssetManager::Create<asset::CTexture>(pnormal));
    pTmp->p_High->LoadFromTexture(
        CAssetManager::Create<asset::CTexture>(phighlighted));

    pTmp->p_Normal->Move(Position);
    pTmp->p_High->Move(Position);

    pTmp->touch = false;
    pTmp->p_Active = pTmp->p_Normal;
    pTmp->id = mp_allButtons.size();

    mp_allButtons.push_back(pTmp);

    return mp_allButtons.back()->id;
}

/**
 * Updates the menu; should be called every frame.
 * @return ID of button if clicked, -1 otherwise.
 **/
int CMenu::Update()
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
 * @return ID of button if there's a mouse-click on it.
 **/
int CMenu::CheckMouseClick()
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
            mp_allButtons[i]->touch     = false;
            mp_allButtons[i]->p_Active  = mp_allButtons[i]->p_Normal;
        }
    }

    return -1;
}

const std::vector<CMenu::MenuItem*>& CMenu::GetButtons() const
{
    return mp_allButtons;
}

obj::CEntity* const CMenu::GetTitle() const
{
    return mp_Title;
}

obj::CEntity* const CMenu::GetBackground() const
{
    return mp_Background;
}
