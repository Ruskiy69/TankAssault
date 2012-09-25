/**
 * @file
 *  Implementation of the CMenuManager class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.1
 **/

#include "Menus/MenuManager.hpp"

using game::CMenuManager;
using game::g_Settings;
using asset::CAssetManager;

/**
 * Initializes all menus and loads their respective files. Also loads fonts,
 * background images, and adds all menu items.
 *
 * @todo Add a credits menu item
 **/
void CMenuManager::Init()
{
    math::CVector2 Position(150, 70);
    gfx::Color Main_Color = gfx::create_color(55, 170, 250);

    g_Log.Flush();
    g_Log << "[INFO] Loading menu images.\n";

    std::string texture_root = g_Settings.GetValueAt("TextureRoot");
    std::string font_root    = g_Settings.GetValueAt("FontRoot");
    std::string audio_root   = g_Settings.GetValueAt("AudioRoot");

    m_MainMenuBackground.LoadFromTexture(
        CAssetManager::Create<asset::CTexture>(
        gk::combine(texture_root, "Menus/Menu_BG.png").c_str()));

    m_PauseMenuBackground.LoadFromTexture(
        CAssetManager::Create<asset::CTexture>(
        gk::combine(texture_root, "Background.png").c_str()));

    mp_MenuFont = (asset::CFont*)CAssetManager::Create<asset::CFont>(
        gk::combine(font_root, "MenuFont.ttf").c_str());
    mp_MenuFont->Resize(60);

    mp_OnHover = (asset::CSound2D*)CAssetManager::Create<asset::CSound2D>(
        gk::combine(audio_root, "Sounds/MenuHover.wav").c_str());

    mp_MenuTitle    = mp_MenuFont->RenderText("Collapse", Main_Color);
    mp_PauseTitle   = mp_MenuFont->RenderText("Paused", Main_Color);

    mp_MenuTitle->Move(Position);
    mp_PauseTitle->Move(Position);

    // Add all main menu items
    Position.Move(40, 160);

    m_MainMenu.SetTitle(mp_MenuTitle);
    m_MainMenu.SetBackground(&m_MainMenuBackground);
    m_MainMenu.SetHoverSound(mp_OnHover);

    m_MainMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Play.png",
        "Data/Textures/Menus/Menu_Play_High.png");
    Position.y += 100;

    m_MainMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Options.png",
        "Data/Textures/Menus/Menu_Options_High.png");
    Position.y += 100;

    m_MainMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Exit.png",
        "Data/Textures/Menus/Menu_Exit_High.png");

    // Add all options menu items
    Position.Move(40, 160);
    
    m_OptionsMenu.SetTitle(mp_MenuTitle);
    m_OptionsMenu.SetBackground(&m_MainMenuBackground);
    m_OptionsMenu.SetHoverSound(mp_OnHover);

    m_OptionsMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Options_Music.png",
        "Data/Textures/Menus/Options_Music_High.png");
    Position.y += 100;

    m_OptionsMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Return.png",
        "Data/Textures/Menus/Menu_Return_High.png");

    // Add all pause menu items
    Position.Move(40, 160);

    m_PauseMenu.SetTitle(mp_PauseTitle);
    m_PauseMenu.SetBackground(&m_PauseMenuBackground);
    m_PauseMenu.SetHoverSound(mp_OnHover);

    m_PauseMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Return.png",
        "Data/Textures/Menus/Menu_Return_High.png");
    Position.y += 100;

    m_PauseMenu.AddMenuItem(Position,
        "Data/Textures/Menus/Menu_Exit.png",
        "Data/Textures/Menus/Menu_Exit_High.png");

    mp_ActiveMenu = &m_MainMenu;
}

/// Runs the main menu, taking action accordingly with button presses.
void CMenuManager::MainMenu()
{
    int status = m_MainMenu.Update();

    if(status == -1)
        return;

    game::GameEvent* pLatest= new game::GameEvent;
    pLatest->evt_type       = game::e_STATE_CHANGE;

    if(status == 0)
        pLatest->new_state  = game::e_INTRO;
    else if(status == 1)
        pLatest->new_state  = game::e_OPTIONSMENU;
    else if(status == 2)
        pLatest->new_state  = game::e_QUIT;

    game::g_GameEventQueue.PushEvent(pLatest);

    // I pulled a tiny trick here. Technically, if the game state changes,
    // there should be a delay of 200ms so that the mouse click isn't
    // registered again (user can lift finger off of mouse button).
    // So, if there was no state change, return from the function, but if
    // there was, do the delay.
    SDL_Delay(200);
}

/**
 * Runs the options menu, taking action accordingly with button presses.
 *
 * @param MusicPlayer The music player currently in use, to toggle music.
 * @see media::MusicPlayer
 **/
void CMenuManager::OptionsMenu(asset::CMusicPlayer& Music)
{
    int status = m_OptionsMenu.Update();

    if(status == -1)
        return;
    else if(status == 0)
        Music.Pause();  /// @todo Actually show ON/OFF on menu item
    else if(status == 1)
    {
        game::GameEvent* pLatest= new game::GameEvent;
        pLatest->evt_type   = game::e_STATE_CHANGE;
        pLatest->new_state  = game::e_MAINMENU;
        game::g_GameEventQueue.PushEvent(pLatest);
    }

    SDL_Delay(200);
}

/// Runs the pause menu, taking action accordingly with button presses.
void CMenuManager::PauseMenu()
{
    int status = m_PauseMenu.Update();

    if(status == -1)
        return;
    
    game::GameEvent* pLatest= new game::GameEvent;
    pLatest->evt_type       = game::e_STATE_CHANGE;

    if(status == 0)
        pLatest->new_state  = game::e_GAME;
    else if(status == 1)
        pLatest->new_state  = game::e_QUIT;

    SDL_Delay(200);
}

/**
 * Fades in the currently active menu.
 *  This should be called once per frame in a game loop.
 *
 * @param float Rate to fade in at (optional)
 * @return TRUE once done fading, FALSE otherwise.
 **/
bool CMenuManager::FadeIn(float rate)
{
    static float alpha = 0.0f;

    if(mp_ActiveMenu == NULL)    // No menu to fade
        return true;

    mp_ActiveMenu->Update();
    glColor4f(1, 1, 1, alpha);
    alpha += rate;

    if(alpha > 1.0f)
    {
        alpha = 0.0f;
        return true;
    }
    else
        return false;
}

/**
 * Fades out the currently active menu.
 *  This should be called once per frame in a game loop.
 *
 * @param float Rate to fade out at (optional)
 * @return TRUE once done fading, FALSE otherwise.
 **/
bool CMenuManager::FadeOut(float rate)
{
    static float alpha = 1.0f;

    if(mp_ActiveMenu == NULL)    // No menu to fade
        return true;

    mp_ActiveMenu->Update();
    glColor4f(1, 1, 1, alpha);
    alpha -= rate;

    if(alpha < 0.0f)
    {
        alpha = 1.0f;
        return true;
    }
    else
        return false;
}

void CMenuManager::Update(asset::CMusicPlayer& Player)
{
    if(m_state == game::e_MAINMENU) this->MainMenu();
    else if(m_state == game::e_OPTIONSMENU) this->OptionsMenu(Player);
    else if(m_state == game::e_PAUSEMENU) this->PauseMenu();
}
