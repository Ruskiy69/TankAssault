#include "Engine.h"

using Game::Game_Engine;

Game_Engine::Game_Engine() : Window(800, 600, "Collapse", "Data/Images/tank.ico"), 
    World(Timer, Window), Events(World), Game_State(Events.GetState())
{
    GLenum err = glewInit();
    if(err != GLEW_OK)
        gk::handle_error((char*)glewGetErrorString(err));

    /* Load the menu music files */
    if(!this->MusicPlayer.AddSongToQueue("Data/Sounds/Menu_Music1.ogg"))
        gk::handle_error(Mix_GetError());

    if(!this->MenuFont.Load("Data/Fonts/MenuFont.ttf", 60))
        gk::handle_error(TTF_GetError());

    if(!this->MainFont.Load("Data/Fonts/GameFont.ttf", 36))
        gk::handle_error(TTF_GetError());

    /* Set up the HUD */
    MathHelper::Vector2 Pos(0, 0);

    int old_size = this->MenuFont.GetSize();
    this->MenuFont.Resize(18);
    this->HUD.Init(this->MenuFont, 12);
    this->MenuFont.Resize(old_size);
    
    this->HUD.AddHUDElement(Game::Bar, Pos, "Health", GFX::create_color(GREEN), 100); Pos.Move(0, 25);
    this->HUD.AddHUDElement(Game::Bar, Pos, "Ammo", GFX::create_color(BLUE), 100); Pos.Move(700, 0);
    this->HUD.AddHUDElement(Game::Menu, Pos, "Pause", GFX::create_color(55, 170, 250), 0);
    
    /* Set the game state to the main menu */
    this->Game_State = Game_Events::Main_Menu;
        
    this->Background.LoadEntity("Data/Images/Background.png");
    this->Light.LoadEntity("Data/Images/Light.png");
    this->Timer.SetFrameRate(60);
    if(!this->World.LoadMap("Data/Map.txt"))
        gk::handle_error("Failed to load level data.");

    /* Setup menus */
    this->Setup();
}

Game_Engine::~Game_Engine()
{
    printf("Done!\n");
}

void Game_Engine::Setup()
{
    MediaManager::SoundEffect Hover;
    MathHelper::Vector2 Pos(150, 70);

    GFX::Color Title_Color = {55, 170, 250};    // Sort of a light blue

    this->MenuFont.Resize(60);

    this->MainMenu.SetHoverSound("Data/Sounds/Menu_Hover.wav");
    this->MainMenu.SetTitle(this->MenuFont, Title_Color, Pos, "Collapse");
    this->MainMenu.SetBackground("Data/Images/Menu/Menu_BG.png");

    Pos.Move(40, 160);
    this->Main_Menu_ids.push_back(this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Play.png", "Data/Images/Menu/Menu_Play_High.png")); Pos.SetY(Pos.GetY() + 100);
    this->Main_Menu_ids.push_back(this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Options.png", "Data/Images/Menu/Menu_Options_High.png")); Pos.SetY(Pos.GetY() + 100);
    //this->Main_Menu_ids.push_back(this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Credits.png", "Data/Images/Menu/Menu_Credits_High.png")); Pos.SetY(Pos.GetY() + 100);
    this->Main_Menu_ids.push_back(this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Exit.png", "Data/Images/Menu/Menu_Exit_High.png")); Pos.SetY(Pos.GetY() + 100);

    Pos.Move(40, 160);
    this->Opt_Menu_ids.push_back(this->OptionsMenu.AddMenuItem(Pos, "Data/Images/Menu/Options_Music.png", "Data/Images/Menu/Options_Music_High.png")); Pos.SetY(Pos.GetY() + 100);
    this->Opt_Menu_ids.push_back(this->OptionsMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Return.png", "Data/Images/Menu/Menu_Return_High.png")); Pos.SetY(Pos.GetY() + 100);

    Pos.Move(150, 70);
    this->OptionsMenu.SetTitle(this->MenuFont, Title_Color, Pos, "Collapse");
    this->OptionsMenu.SetBackground("Data/Images/Menu/Menu_BG.png");
    this->OptionsMenu.SetHoverSound("Data/Sounds/Menu_Hover.wav");

    Pos.Move(40, 160);
    this->Pause_Menu_ids.push_back(this->PauseMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Return.png", "Data/Images/Menu/Menu_Return_High.png")); Pos.SetY(Pos.GetY() + 100);
    this->Pause_Menu_ids.push_back(this->PauseMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Exit.png", "Data/Images/Menu/Menu_Exit_High.png")); Pos.SetY(Pos.GetY() + 100);

    Pos.Move(150, 70);
    this->PauseMenu.SetTitle(this->MenuFont, Title_Color, Pos, "Paused");
    this->PauseMenu.SetBackground(this->Background);
    this->PauseMenu.SetHoverSound("Data/Sounds/Menu_Hover.wav");
}

void Game_Engine::Run()
{
    this->MusicPlayer.SetVolume(100);
    this->MusicPlayer.PlayMusic();

    while(this->Game_State != Game_Events::Quit)
    {
        this->Timer.Start();
        this->GameLoop();
        this->Timer.DelayFPS();
    }
}

void Game_Engine::GameLoop()
{
    bool first = true;
    static float alpha = 0.0f;
    static int x, y;
    int status = -1;

    const element_id hud_pause_id = this->HUD.FindHUDElement(Game::Menu, "Pause");

    Game::GetMousePosition(x, y);

    this->Window.Clear();

    switch(this->Game_State)
    {
    case Game_Events::Main_Menu:
        glColor4f(1, 1, 1, 1);
        status = this->MainMenu.Update();

        if(status == this->Main_Menu_ids[Game_Engine::main_play_id])
        {
            this->MusicPlayer.StopMusic();
            this->MusicPlayer.PurgeQueue();
            SDL_Delay(100);
            this->Game_State = Game_Events::Intro;
        }
        else if(status == this->Main_Menu_ids[Game_Engine::main_options_id])
        {
            SDL_Delay(200);
            this->Game_State = Game_Events::Options_Menu;
        }
        else if(status == this->Main_Menu_ids[Game_Engine::main_quit_id])
            this->Game_State = Game_Events::Quit;

        break;

    case Game_Events::Options_Menu:
        glColor4f(1, 1, 1, 1);
        status = this->OptionsMenu.Update();
        
        if(status == this->Opt_Menu_ids[Game_Engine::opt_music_toggle_id])
        {
            SDL_Delay(200);
            this->MusicPlayer.PauseMusic();
        }
        else if(status == this->Opt_Menu_ids[Game_Engine::opt_ret_id])
        {
            SDL_Delay(200);
            this->Game_State = Game_Events::Main_Menu;
        }
        
        break;

    case Game_Events::Pause_Menu:
        glColor4f(1, 1, 1, 1);
        status = this->PauseMenu.Update();

        if(status == this->Pause_Menu_ids[Game_Engine::pause_ret_id])
        {
            SDL_Delay(100);
            this->Game_State = Game_Events::Play;
        }
        else if(status == this->Pause_Menu_ids[Game_Engine::pause_quit_id])
            this->Game_State = Game_Events::Quit;
        
        break;

    case Game_Events::Intro:
        this->GameIntro();
        this->MusicPlayer.PlayMusic();
        this->Game_State = Game_Events::Play;
        break;

    case Game_Events::Play:
        /* Update everything on-screen */
        if(first)
        {
            /* Fade-in effect after intro scene */
            glColor4f(1, 1, 1, alpha);
            alpha += 0.005f;
            if(alpha >= 1.0f)
                first = false;
        }

        this->Background.Update();
        this->World.Update();
        if(this->HUD.Update() == hud_pause_id)
            this->Game_State = Game_Events::Pause_Menu;

        break;
    }

    printf("State: %d\n", this->Game_State);

    this->Events.HandleGameEvents();
    this->MusicPlayer.Update();
    this->Window.Update();
}

void Game_Engine::GameIntro()
{
    static bool first = true;
    static GFX::Color Color = {55, 170, 250};    // Same off-blue as the titles

    /* We only run the intro once heh. */
    if(!first)
        return;

    /* Variable */
    float alpha = 0.01f, rate = 0.003f;        // For fade in/out effects
    int h = 100;                                // y position of each line
    size_t index = 0;                           // which line?
    std::vector<std::string> lines = gk::split(INTRO_STR, '\n');    // Individual lines
    std::vector<GFX::GL_Entity*> LineEntities;  // All lines
    std::string line = lines[index];            // Line to render

    MediaManager::Music IntroSong;              // Sinister music
    if(!IntroSong.Load("Data/Sounds/Intro.ogg"))// Load it
        gk::handle_error(Mix_GetError());
    IntroSong.SetVolume(69);                    // Not too loud, plus it's 69

    /* First fade out the menu */
    alpha = 1.0f, rate = 0.05f;
    while(this->Game_State != Game_Events::Quit && alpha >= 0.0f)
    {
        this->Timer.Start();
        this->Window.Clear();
        
        this->Events.HandleGameEvents();
        if(IsDown(SDLK_ESCAPE)) break;

        this->MainMenu.Update();
        glColor4f(1, 1, 1, alpha);
        this->Window.Update();
        this->Timer.DelayFPS();

        alpha -= rate;
    }

    alpha = 0.01f; rate = 0.0028f;
    IntroSong.Play();                           // Play it duh

    /* Render the first line of text */
    LineEntities.push_back(this->MainFont.RenderText(line, Color));
    LineEntities.back()->Move(100.0f, h - 32.0f);

    while(this->Game_State != Game_Events::Quit)
    {
        this->Timer.Start();
        this->Window.Clear();

        this->Events.HandleGameEvents();
        if(IsDown(SDLK_ESCAPE)) break;

        /* If we are done fading in the current
         * line, fade-in the next line.
         */
        if(alpha >= 1.0f)
        {
            index++;
            if(index >= lines.size())   // Last line faded
            {
                first = false;
                break;
            }

            /* After the second line, the music
             * is a little different, so it's okay
             * to speed up the fading because it 
             * doesn't need to match anymore.
             */
            if(index > 2)
                rate = 0.004f;

            /* Reset the alpha, load the next line. */
            alpha = 0.0f;
            line = lines[index];
            LineEntities.push_back(this->MainFont.RenderText(line, Color));
            LineEntities.back()->Move(100.0f, h*1.0f);
            h += LineEntities.back()->GetH();
        }

        /* Render all lines */
        for(size_t i = 0; i < LineEntities.size(); i++)
        {
            /* Fading-in on the last line only */
            if(i + 1 >= LineEntities.size())
                glColor4f(1, 1, 1, alpha);

            LineEntities[i]->Update();
            glColor4f(1, 1, 1, 1);
        }

        alpha += rate;
        this->Window.Update();
        this->Timer.DelayFPS();
    }

    /* Fade to black */
    rate = 0.01f;   // Fairly quickly
    while(this->Game_State != Game_Events::Quit && alpha >= 0.0f)
    {
        this->Timer.Start();
        this->Window.Clear();
        
        this->Events.HandleGameEvents();
        if(IsDown(SDLK_ESCAPE)) break;

        for(size_t i = 0; i < LineEntities.size(); i++)
        {
            LineEntities[i]->Update();
        }

        glColor4f(1, 1, 1, alpha);
        alpha -= rate;

        this->Window.Update();
        this->Timer.DelayFPS();
    }

    /* Add the last line */
    this->MainFont.Resize(64);  // Make it big
    LineEntities.push_back(this->MainFont.RenderText(INTRO_STR2, Color));
    LineEntities.back()->Move(Window.GetWidth() / 2.0f - LineEntities.back()->GetW() / 2.0f, 200.0f);    // Center it
    alpha = 0.0f;   // Start with no alpha
    rate = 0.01f;   // Pretty fast fade-in

    while(this->Game_State != Game_Events::Quit && alpha <= 1.0f)
    {
        this->Timer.Start();
        this->Window.Clear();

        this->Events.HandleGameEvents();
        if(IsDown(SDLK_ESCAPE)) break;

        LineEntities.back()->Update();

        glColor4f(1, 1, 1, alpha);
        alpha += rate;

        this->Window.Update();
        this->Timer.DelayFPS();
    }
    
    rate = 0.008f;  // Really slow fade-out
    while(this->Game_State != Game_Events::Quit && alpha >= 0.0f)
    {
        this->Timer.Start();
        this->Window.Clear();
    
        this->Events.HandleGameEvents();
        if(IsDown(SDLK_ESCAPE)) break;

        LineEntities.back()->Update();

        glColor4f(1, 1, 1, alpha);
        alpha -= rate;

        this->Window.Update();
        this->Timer.DelayFPS();
    }

    first = false;

    /* Memory clean up */
    for(size_t i = 0; i < LineEntities.size(); i++)
        delete LineEntities[i];
    LineEntities.clear();
    IntroSong.FadeOut(3000);
}