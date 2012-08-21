/**
 * @file
 *  Implementation of the MenuManager class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.1
 */

#include "Menus/MenuManager.h"

using game::CL_MenuManager;

/**
 * Initializes all menus and loads their respective files. Also loads fonts,
 * background images, and adds all menu items.
 *
 * @todo Add a credits menu item
 */
void CL_MenuManager::Init()
{
    math::ML_Vector2 Position(150, 70);
    gfx::Color Main_Color = gfx::create_color(55, 170, 250);

    g_Log.Flush();
    g_Log << "[INFO] Loading menu images.\n";

    mp_MainMenuBackground = asset::g_TextureAssets.GetEntityByID(
        asset::g_TextureAssets.LoadEntityFromFile(
        "Data/Images/Menu/Menu_BG.png"));

    mp_PauseMenuBackground = asset::g_TextureAssets.GetEntityByID(
        asset::g_TextureAssets.LoadEntityFromFile(
        "Data/Images/Background.png"));

    mp_MenuFont = asset::g_FontAssets.GetFontByID(
        asset::g_FontAssets.LoadFontFromFile(
        "Data/Fonts/MenuFont.ttf", 60));        

    mp_OnHover = asset::g_AudioAssets.GetAudioByID(
        asset::g_AudioAssets.LoadAudioFromFile(
        "Data/Sounds/Menu_Hover.wav"));

    mp_MenuTitle    = mp_MenuFont->RenderText("Collapse", Main_Color);
    mp_PauseTitle   = mp_MenuFont->RenderText("Paused", Main_Color);

    mp_MenuTitle->Move(Position);
    mp_PauseTitle->Move(Position);

    // Add all main menu items
    Position.Move(40, 160);

    m_MainMenu.SetTitle(mp_MenuTitle);
    m_MainMenu.SetBackground(mp_MainMenuBackground);
    m_MainMenu.SetHoverSound(mp_OnHover);

    m_MainMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Play.png",
        "Data/Images/Menu/Menu_Play_High.png");
    Position.y += 100;

    m_MainMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Options.png",
        "Data/Images/Menu/Menu_Options_High.png");
    Position.y += 100;

    m_MainMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Exit.png",
        "Data/Images/Menu/Menu_Exit_High.png");

    // Add all options menu items
    Position.Move(40, 160);
    
    m_OptionsMenu.SetTitle(mp_MenuTitle);
    m_OptionsMenu.SetBackground(mp_MainMenuBackground);
    m_OptionsMenu.SetHoverSound(mp_OnHover);

    m_OptionsMenu.AddMenuItem(Position,
        "Data/Images/Menu/Options_Music.png",
        "Data/Images/Menu/Options_Music_High.png");
    Position.y += 100;

    m_OptionsMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Return.png",
        "Data/Images/Menu/Menu_Return_High.png");

    // Add all pause menu items
    Position.Move(40, 160);

    m_PauseMenu.SetTitle(mp_PauseTitle);
    m_PauseMenu.SetBackground(mp_PauseMenuBackground);
    m_PauseMenu.SetHoverSound(mp_OnHover);

    m_PauseMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Return.png",
        "Data/Images/Menu/Menu_Return_High.png");
    Position.y += 100;

    m_PauseMenu.AddMenuItem(Position,
        "Data/Images/Menu/Menu_Exit.png",
        "Data/Images/Menu/Menu_Exit_High.png");

    mp_ActiveMenu = &m_MainMenu;
}

/**
 * Runs the main menu, taking action accordingly with button presses.
 */
void CL_MenuManager::MainMenu()
{
    mp_ActiveMenu = &m_MainMenu;

    int status = m_MainMenu.Update();

    if(status == -1)
        return;
    else if(status == 0)
        m_state = game::e_INTRO;
    else if(status == 1)
        m_state = game::e_OPTIONSMENU;
    else if(status == 2)
        m_state = game::e_QUIT;

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
 *
 * @see media::MusicPlayer
 */
void CL_MenuManager::OptionsMenu()//media::MusicPlayer& Music)
{
    mp_ActiveMenu = &m_OptionsMenu;

    int status = m_OptionsMenu.Update();

    if(status == -1)
        return;
    else if(status == 0)
    {
        /// @todo Actually show ON/OFF on menu item
        //Music.Pause();
    }
    else if(status == 1)
        m_state = game::e_MAINMENU;
    else
        return;

    SDL_Delay(200);
}

/**
 * Runs the pause menu, taking action accordingly with button presses.
 */
void CL_MenuManager::PauseMenu()
{
    mp_ActiveMenu = &m_PauseMenu;

    int status = m_PauseMenu.Update();

    if(status == -1)
        return;
    else if(status == 0)
        m_state = game::e_GAME;
    else if(status == 1)
        m_state = game::e_QUIT;
    else
        return;

    SDL_Delay(200);
}

/**
 * Fades in the currently active menu. This should be called once per frame in a game loop.
 *
 * @param float Rate to fade in at (optional)
 * 
 * @return TRUE once done fading, FALSE otherwise.
 */
bool CL_MenuManager::FadeIn(float rate)
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
 * Fades out the currently active menu. This should be called once per frame in a game loop.
 *
 * @param float Rate to fade out at (optional)
 * 
 * @return TRUE once done fading, FALSE otherwise.
 */
bool CL_MenuManager::FadeOut(float rate)
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