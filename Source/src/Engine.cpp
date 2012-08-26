/**
 * @file
 *  Engine class definitions.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 * 
 * @todo Fix menu glitch in Release build (after splash screen)
 * @todo Add fadeout from loading screen.
 * @todo [MAYBE] Create a separate thread for loading while
 *       the screen fades in/out.
 */

#include "Engine.h"

using game::CL_Engine;

using game::g_Log;
using asset::g_AudioAssets;
using asset::g_FontAssets;
using asset::g_TextureAssets;

/**
 * Initializes everything for the game.
 */
void CL_Engine::Init()
{
    m_Timer.SetFrameRate(60);

    // Menu font.
    m_introfont_id = g_FontAssets.LoadFontFromFile(
        "Data/Fonts/GameFont.ttf", 72);

    // Text color, blue-ish.
    m_OffBlue = gfx::create_color(55, 170, 250);

    // Loading screen.
    asset::GL_Entity* pLoad = g_FontAssets.GetFontByID(
        m_introfont_id)->RenderText("Loading...", m_OffBlue);
    pLoad->Move(m_Window.GetWidth() / 2 - pLoad->GetW() / 2, 
        m_Window.GetHeight() / 2);
    g_FontAssets.GetFontByID(m_introfont_id)->Resize(32);
    
    m_Window.Clear();
    pLoad->Update();
    m_Window.Update();

    // Intro song.
    m_introsong_id = g_AudioAssets.LoadAudioFromFile("Data/Sounds/Intro.ogg");

    // Menu song #1.
    m_MusicPlayer.AddSongToQueue("Data/Sounds/Menu_Music1.ogg");

    // In-game 'aim' cursor.
    mp_Cursor = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
        "Data/Images/Crosshairs.png"));

    m_Menus.Init();
    m_state = game::e_SPLASH;
}

/**
 * The main game loop that processes all logic.
 */
void CL_Engine::GameLoop()
{
    bool first = true;

#if !REGULATE_FPS
#ifdef _WIN32
    // Disable VSYNC
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)
        wglGetProcAddress("wglSwapIntervalEXT");
    wglSwapIntervalEXT(0);

    long frame = 0;
    Uint32 start_time = SDL_GetTicks();
#endif // _WIN32
#endif // REGULATE_FPS

    float alpha = 0.0f;

    g_Log.Flush();
    g_Log << "[INFO] Asset count breakdown:";
    g_Log << "\n[INFO] Textures: " << g_TextureAssets.GetEntityCount();
    g_Log << "\n[INFO] Audio   : " << g_AudioAssets.GetAudioCount();
    g_Log << "\n[INFO] Fonts   : " << g_FontAssets.GetFontCount();
    g_Log.ShowLastLog();

    while(m_state != game::e_QUIT)
    {
#if REGULATE_FPS
        // No timer
        m_Timer.Start();
        int frame = m_Timer.GetFrame();
#else
        frame++;
#endif // REGULATE_FPS
        if(frame % 120 == 0)
        {
            g_Log.Flush();
            g_Log << "[DEBUG] Frame #" << frame;
            g_Log << " engine state: " << m_state << ".\n";
        }

        this->HandleSystemEvents();

        m_Window.Clear();

        switch(m_state)
        {
        case game::e_MAINMENU:
#ifndef _DEBUG
            if(first)
            {
                if(m_Menus.FadeIn(0.005f))
                    first = false;
            }
            else
#endif // _DEBUG
                m_Menus.MainMenu();
            break;

        case game::e_OPTIONSMENU:
            m_Menus.OptionsMenu(m_MusicPlayer);
            break;

        case game::e_PAUSEMENU:
            m_Menus.PauseMenu();
            break;

        case game::e_SPLASH:
            {
                // Show the splash logo but not in debug builds.
#ifndef _DEBUG
                asset::GL_Entity* p_Fader = g_TextureAssets.GetEntityByID(
                    g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
                    "Data/Images/Splash.png"));

                while(m_state == game::e_SPLASH)
                {
                    m_Timer.Start();
                    m_Window.Clear();
                    this->HandleSystemEvents();

                    if(gfx::FadeIn(p_Fader, 0.008f))
                        break;

                    m_Window.Update();
                    m_Timer.DelayFPS();
                }
                while(m_state == game::e_SPLASH)
                {
                    m_Timer.Start();
                    m_Window.Clear();
                    this->HandleSystemEvents();

                    if(gfx::FadeOut(p_Fader, 0.006f))
                    {
                        m_state = game::e_MAINMENU;
                        m_MusicPlayer.Play();
                    }

                    m_Window.Update();
                    m_Timer.DelayFPS();
                }

                m_Window.Clear();
                continue;
#else
                m_state = game::e_MAINMENU;
                m_MusicPlayer.Play();
#endif // _DEBUG
            }

            break;

        case game::e_INTRO:
#ifndef _DEBUG
            // I don't want to watch the intro every single time I compile
            this->Intro();
            continue;   // We've been inside the loop this whole time,
                        // so I don't want to call CL_Timer::DelayFPS()
                        // because it'd be extremely off.
#else
            SDL_ShowCursor(0);
            m_state = game::e_GAME;
            break;
#endif // _DEBUG

        case game::e_GAME:
            glColor4f(1, 1, 1, alpha);
            m_World.Update();
            mp_Cursor->Move(game::GetMousePosition());
            mp_Cursor->Move_Rate(-16, -16);
            mp_Cursor->Update();

            g_AudioAssets.Update();
            alpha += 0.01f;
            break;

        default:
            g_Log.Flush();
            g_Log << "[ERROR] Invalid game state: " << m_state << ".\n";
            break;
        }

        g_AudioAssets.Update();
        m_Window.Update();

#if REGULATE_FPS
        // No timer in debug builds
        m_Timer.DelayFPS();
#else
        // Show frame-rate in debug builds
        Uint32 elapsed = SDL_GetTicks() - start_time;
        double fps = frame / (elapsed / 1000.0);
        printf("Frame Rate: %0.2f\r", fps);
#endif // REGULATE_FPS
    }
}

/**
 * Shows an intro screen that sets the theme and provides a storyline.
 */
void CL_Engine::Intro()
{
    g_Log.Flush();
    g_Log << "[INFO] Playing intro sequence.\n";

    m_MusicPlayer.PurgeQueue();
    m_MusicPlayer.Stop();

    // Current line, height
    u_int index = 0;
    float h = 78;

    // Rate to fade at
    float rate = 0.0028f;

    // Font
    asset::FL_Font* p_IntroFont = g_FontAssets.GetFontByID(m_introfont_id);

    // Song
    asset::AL_Sound2D* p_IntroSong = 
        g_AudioAssets.GetAudioByID(m_introsong_id);
    
    // The actual object representing each line
    std::vector<asset::GL_Entity*> linepEntities, faderpLine;

    // The current line
    char* current_line = (char*)INTRO_STR[index];

    // Add the first line to fade
    faderpLine.push_back(p_IntroFont->RenderText(current_line, m_OffBlue));
    faderpLine[0]->Move(100.0f, h);
    
    // Fade out the main menu
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();
        this->HandleSystemEvents();
        if(m_Menus.FadeOut(0.02f))
            break;
        m_Window.Update();
        m_Timer.DelayFPS();
    }

    p_IntroSong->Play();

    // Fade in one line at a time
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();
        if(game::IsDown(SDLK_ESCAPE))
            break;

        // Rendering
        m_Window.Clear();

        for(size_t i = 0; i < linepEntities.size(); ++i)
            linepEntities[i]->Update();

        if(gfx::FadeIn(faderpLine, rate))
        {
            index++;
            if(index >= INTRO_STR_SIZE)
                break;

            if(index > 2)
                rate = 0.004f;

            // Casting away const-ness is pretty bad...
            current_line = (char*)INTRO_STR[index];
            
            linepEntities.push_back(faderpLine[0]);
            h += p_IntroFont->GetTextHeight(current_line);
            faderpLine[0] = p_IntroFont->RenderText(current_line, m_OffBlue);
            faderpLine[0]->Move(100.0f, h);
        }
        
        m_Window.Update();
        m_Timer.DelayFPS();
    }

    // Fade out all lines
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_Window.Clear();
        if(gfx::FadeOut(linepEntities, 0.01f))
            break;
        m_Window.Update();
        m_Timer.DelayFPS();
    }

    p_IntroFont->Resize(64);
    faderpLine[0] = p_IntroFont->RenderText(
        "Year 2200 - 6 Days Post-Collapse", m_OffBlue);
    faderpLine[0]->Move(
        m_Window.GetWidth() / 2.0f - faderpLine[0]->GetW() / 2.0f,
        200.0f);

    // Fade in newest line
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_Window.Clear();
        if(gfx::FadeIn(faderpLine, 0.01f))
            break;
        m_Window.Update();
        m_Timer.DelayFPS();
    }

    // Fade out slowly
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_Window.Clear();
        if(gfx::FadeOut(faderpLine, 0.008f))
            break;
        m_Window.Update();
        m_Timer.DelayFPS();
    }

    // Memory clean up
    for(size_t i = 0; i < linepEntities.size(); ++i)
        delete linepEntities[i];
    delete faderpLine[0];
    linepEntities.clear();
    faderpLine.clear();
    
    // Only change state if we are exiting loops properly
    if(m_state == game::e_INTRO)
    {
        SDL_ShowCursor(0);
        m_state = game::e_GAME;
    }
}

/**
 * Handles all keyboard, mouse, and window events.
 */
void CL_Engine::HandleSystemEvents()
{
    SDL_Event Evt;

    if(game::IsDown(SDLK_ESCAPE) && m_state != e_INTRO)
    {
        g_Log.Flush();
        g_Log << "[INFO] Quitting.\n";
        m_state = game::e_QUIT;
    }

    while(SDL_PollEvent(&Evt))
    {
        switch(Evt.type)
        {
        case SDL_QUIT:
            g_Log.Flush();
            g_Log << "[INFO] Quitting.\n";

            m_state = game::e_QUIT;
            break;

        default:
            if(m_state == game::e_GAME)
                m_World.HandleEvent(Evt);
            break;
        }
    }

    m_World.HandleEvent(Evt);
}