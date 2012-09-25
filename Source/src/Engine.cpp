#include "Engine.hpp"

using game::CEngine;
using asset::CAssetManager;

CEngine::CEngine() : m_GameWindow(800, 600, "Collapse", 
    "Data/Textures/tank.ico"),
    m_Menus(m_GameWindow, m_state),
    m_state(game::e_SPLASH), m_World(m_state),
    m_Inventory(m_World.GetPlayer()) {}

bool CEngine::Init()
{
    // Initialize GLEW after the OpenGL context has been created.
    if(glewInit() != GLEW_OK) return false;

    // Set the frame rate.
    m_Timer.SetFrameRate(60);

    // Start loading intro, splash, and menu assets.
    g_Log.Flush();
    g_Log << "[INFO] Loading primary assets.\n";

    mp_IntroFont = CAssetManager::Create<asset::CFont>(
        "Data/Fonts/IntroFont.ttf");
    mp_IntroFont->Resize(72);

    // Create the off-blue color used for text in the intro and
    // many other places.
    m_OffBlue = gfx::create_color(55, 170, 250);

    // Loading screen so the user knows what's going on.
    obj::CEntity* pLoading = mp_IntroFont->RenderText(
        "Loading...", m_OffBlue);
    pLoading->Move(m_GameWindow.GetWidth() / 2 - pLoading->GetW() / 2,
        m_GameWindow.GetHeight() / 2 - pLoading->GetH() / 2);
    mp_IntroFont->Resize(32);

    mp_Version = mp_IntroFont->RenderText(GAME_VERSION, m_OffBlue);
    mp_Version->Move(m_GameWindow.GetWidth() - mp_Version->GetW(),
        m_GameWindow.GetHeight() - mp_Version->GetH());

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_GameWindow.Clear();
    pLoading->Update();
    m_GameWindow.Update();

    delete pLoading;

    // Load settings file
    if(!g_Settings.Load("Data/Settings.ini"))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load Data/Settings.ini.\n";
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    // Load the intro song.
    mp_IntroSong = CAssetManager::Create<asset::CSound2D>(
        "Data/Audio/Music/Intro.ogg");

    // Add a song to the main menu.
    m_MusicPlayer.AddSongToQueue("Data/Audio/Music/MenuMusic1.ogg");
    
    // In-game cross hairs
    m_IngameCursor.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        "Data/Textures/Crosshairs.png"));

    m_Splash.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        "Data/Textures/Splash.png"));

    // Initialize menu assets
    m_Menus.Init();

    // Initialize the world assets.
    m_World.Init();

    // Initialize inventory assets.
    m_Inventory.Init();

    g_Log.Flush();
    g_Log << "[INFO] Initialization complete.\n";
    g_Log.ShowLastLog();

    return true;
}

bool CEngine::GameLoop()
{
    // Alpha fading for various sequences.
    float alpha = 0.0f;

    // First time loop entering tracking
    bool first  = true;

    // Disable vsync if specified, for checking real frame rates.
#if !REGULATE_FPS
#ifdef _WIN32
    typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)
        wglGetProcAddress("wglSwapIntervalEXT");
    wglSwapIntervalEXT(0);

    long frame = 0;
    Uint32 start_time = SDL_GetTicks();
#endif // _WIN32
#endif // REGULATE_FPS

    // Some logging before the loop starts, showing assets.
    g_Log.Flush();
    g_Log << "[INFO] Asset count: " << CAssetManager::GetAssetCount() << "\n";
    g_Log.ShowLastLog();

    while(m_state != game::e_QUIT)
    {
        // Events
        this->HandleGameEvents();
        this->HandleSystemEvents();

        // Rendering
        m_GameWindow.Clear();
        switch(m_state)
        {
        case game::e_MAINMENU:
#ifndef _DEBUG
            if(first)
            {
                first = !(m_Menus.FadeIn(0.005f));
            }
#endif // _DEBUG
        case game::e_OPTIONSMENU:
        case game::e_PAUSEMENU:
            m_Menus.Update(m_MusicPlayer);
            mp_Version->Update();
            break;

        case game::e_SPLASH:
            {
                // Show the splash logo but not in debug builds.
#ifndef _DEBUG
                float alpha = 0.0f;

                while(m_state == game::e_SPLASH && alpha <= 1.0f)
                {
                    m_Timer.Start();
                    this->HandleSystemEvents();
                    alpha += 0.01f;
                    m_GameWindow.Clear();
                    glColor4f(1, 1, 1, alpha);
                    m_Splash.Update();
                    m_GameWindow.Update();
                    m_Timer.DelayFPS();
                }

                alpha = 1.0f;

                while(m_state == game::e_SPLASH)
                {
                    m_Timer.Start();
                    this->HandleGameEvents();
                    this->HandleSystemEvents();
                    alpha -= 0.008f;
                    if(alpha <= 0.0f)
                    {
                        game::GameEvent* pLatest = new game::GameEvent;
                        pLatest->evt_type = game::e_STATE_CHANGE;
                        pLatest->new_state= game::e_MAINMENU;
                        g_GameEventQueue.PushEvent(pLatest);
                        m_MusicPlayer.Play();
                    }

                    m_GameWindow.Clear();
                    glColor4f(1, 1, 1, alpha);
                    m_Splash.Update();
                    m_GameWindow.Update();
                    m_Timer.DelayFPS();
                }

                continue;
#else
                game::GameEvent* pMenu = new game::GameEvent;
                pMenu->evt_type = game::e_STATE_CHANGE;
                pMenu->new_state= game::e_MAINMENU;
                g_GameEventQueue.PushEvent(pMenu);
                m_MusicPlayer.Play();
#endif // _DEBUG
            }

            break;

        case game::e_INTRO:
            {
#ifndef _DEBUG
                // I don't want to watch the intro every single time I compile
                this->Intro();
                continue;   // We've been inside the loop this whole time,
                // so I don't want to call CTimer::DelayFPS()
                // because it'd be extremely off.
#else
                game::GameEvent* pGame = new game::GameEvent;
                pGame->evt_type = game::e_STATE_CHANGE;
                pGame->new_state= game::e_GAME;
                g_GameEventQueue.PushEvent(pGame);
                break;
#endif // _DEBUG
            }

        case game::e_GAME:
            {
                // Logic
                m_IngameCursor.Move(game::GetMousePosition());
                m_IngameCursor.Move_Rate(-16, -16);

                // Rendering
                m_World.Update();
                m_IngameCursor.Update();
                break;
            }

        case game::e_INVENTORY:
            m_Inventory.Update();
            break;

        case game::e_QUIT:
            break;

        default:
            g_Log.Flush();
            g_Log << "[ERROR] Invalid game state: " << m_state << ".\n";
            break;            
        }

        std::vector<asset::CSound2D*>::iterator i;
        for(i = asset::CSound2D::mp_allSounds.begin();
            i != asset::CSound2D::mp_allSounds.end();
            ++i)
        {
            if((*i)->GetAudioState() == AL_STOPPED)
            {
                g_Log.Flush();
                g_Log << "[INFO] Unloading audio file: ";
                g_Log << (*i)->GetFilename() << ".\n";
                g_Log.ShowLastLog();
                (*i)->UnloadSource();
            }
        }
        
        m_GameWindow.Update();

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

    return (m_state == game::e_QUIT);
}

void CEngine::HandleSystemEvents()
{
    SDL_Event Evt;
    while(SDL_PollEvent(&Evt))
    {
        switch(Evt.type)
        {
        case SDL_QUIT:
            m_state = game::e_QUIT;
            break;

        case SDL_KEYDOWN:
            if(m_state == game::e_GAME && Evt.key.keysym.sym == SDLK_i)
                m_state = game::e_INVENTORY;
            else if(m_state == game::e_INVENTORY)
                m_state = game::e_GAME;
            break;
        }

        if(m_state == game::e_GAME)
            m_World.HandleSystemEvent(Evt);
    }
}

void CEngine::HandleGameEvents()
{
    game::GameEvent* pLatest = NULL;
    while((pLatest = g_GameEventQueue.PopEvent()) != NULL)
    {
        switch(pLatest->evt_type)
        {
        case game::e_STATE_CHANGE:
            m_state = pLatest->new_state;
            
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);

            if(m_state == game::e_GAME)
            {
                SDL_ShowCursor(0);
                m_MusicPlayer.Stop();
                //m_Renderer.SetAlphaMask(1.0f);
                //m_Renderer.PurgeObjects();
                m_World.HandleGameEvent(pLatest);
            }
            break;
        }

        delete pLatest;
    }
}

/**
 * Shows an intro screen that sets the theme and provides a storyline.
 */
void CEngine::Intro()
{
    g_Log.Flush();
    g_Log << "[INFO] Playing intro sequence.\n";

    m_MusicPlayer.Stop();
    m_MusicPlayer.PurgeQueue();

    // Current line, height
    u_int index = 0;
    float h     = 78;

    // Rate to fade at
    float rate  = 0.0028f;

    // The actual object representing each line
    std::vector<obj::CEntity*> p_lineEntities;
    obj::CEntity* pFadingLine = NULL;

    // The current line
    char* current_line = (char*)INTRO_STR[index];

    // Add the first line to fade
    pFadingLine = mp_IntroFont->RenderText(current_line, m_OffBlue);
    pFadingLine->Move(100.0f, h);
    pFadingLine->Update();

    // Fade out the main menu
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();
        this->HandleSystemEvents();
        if(m_Menus.FadeOut(0.02f))
            break;
        m_GameWindow.Update();
        m_Timer.DelayFPS();
    }

    mp_IntroSong->Play();

    // Fade in one line at a time
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();
        if(game::IsDown(SDLK_ESCAPE))
            break;

        // Rendering
        m_GameWindow.Clear();

        for(size_t i = 0; i < p_lineEntities.size(); ++i)
            p_lineEntities[i]->Update();

        if(gfx::FadeIn(pFadingLine, rate))
        {
            index++;
            if(index >= INTRO_STR_SIZE)
                break;

            if(index > 2)
                rate = 0.004f;

            // Casting away const-ness is pretty bad...
            current_line = (char*)INTRO_STR[index];

            p_lineEntities.push_back(pFadingLine);
            h += mp_IntroFont->GetTextHeight(current_line);
            pFadingLine = mp_IntroFont->RenderText(current_line, m_OffBlue);
            pFadingLine->Move(100.0f, h);
        }

        m_GameWindow.Update();
        m_Timer.DelayFPS();
    }

    // Fade out all lines
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_GameWindow.Clear();
        if(gfx::FadeOut(p_lineEntities, 0.01f)) break;
        m_GameWindow.Update();
        m_Timer.DelayFPS();
    }

    mp_IntroFont->Resize(64);
    delete pFadingLine;
    pFadingLine = mp_IntroFont->RenderText(
        "Year 2200 - 6 Days Post-Collapse", m_OffBlue);
    pFadingLine->Move(
        m_GameWindow.GetWidth() / 2.0f - pFadingLine->GetW() / 2.0f,
        200.0f);

    // Fade in newest line
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_GameWindow.Clear();
        if(gfx::FadeIn(pFadingLine, 0.01f)) break;
        m_GameWindow.Update();
        m_Timer.DelayFPS();
    }

    // Fade out slowly
    while(m_state == game::e_INTRO)
    {
        m_Timer.Start();

        // Events
        this->HandleSystemEvents();

        // Rendering
        m_GameWindow.Clear();
        if(gfx::FadeOut(pFadingLine, 0.008f)) break;
        m_GameWindow.Update();
        m_Timer.DelayFPS();
    }

    // Memory clean up
    for(size_t i = 0; i < p_lineEntities.size(); ++i)
        delete p_lineEntities[i];
    delete pFadingLine;
    p_lineEntities.clear();

    // Only change state if we are exiting loops properly
    if(m_state == game::e_INTRO)
    {
        game::GameEvent* pChange = new game::GameEvent;
        pChange->evt_type = game::e_STATE_CHANGE;
        pChange->new_state= game::e_GAME;
        g_GameEventQueue.PushEvent(pChange);
    }

    mp_IntroSong->Stop();
}