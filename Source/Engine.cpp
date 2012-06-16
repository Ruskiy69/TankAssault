#include "Engine.h"

using gk::CEngine;
using namespace gk_gl; 
using namespace gk_sdl;

CEngine::CEngine(): Display(800, 600, "Tank Assault", "Data/Images/tank.ico"),
    Settings("Settings.ini"), Player(Timer)
{
    srand((unsigned int)time(NULL));

    this->State = CEngine::Menu;
    this->quit  = false;

    int retval = this->Map.Load("Data/Map.txt");
    if(retval == -1)
    {
        gk::HandleError("Unable to find 'Map.txt'");
    }

    SDL_Surface* Bg = LoadImage("Data/Images/Background.png");
    this->Background.SetEntity(Bg);
    SDL_FreeSurface(Bg);

    this->Enemies.push_back(new CGL_Enemy(Timer, Player, Map));
}

CEngine::~CEngine()
{
    TTF_CloseFont(this->Menu_Font);
}

void CEngine::Run()
{
    CGL_Line Line(GL_Vertex2f(0, 0), GL_Vertex2f(700, 400));

    GL_Vertex2f Pos(40, 160);
    GL_Vertex2f Title_Pos(120, 70);
    int status  = -1;

    int play_id = this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Play.png", "Data/Images/Menu/Menu_Play_High.png"); Pos.y += 100;
    int opt_id  = this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Options.png", "Data/Images/Menu/Menu_Options_High.png"); Pos.y += 100;
    int quit_id = this->MainMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Exit.png", "Data/Images/Menu/Menu_Exit_High.png"); Pos.y += 100;
    
    Pos.x = 40;
    Pos.y = 160;
    int mus_toggle  = this->OptionsMenu.AddMenuItem(Pos, "Data/Images/Menu/Options_Music.png", "Data/Images/Menu/Options_Music_High.png"); Pos.y += 100;
    int ret_id      = this->OptionsMenu.AddMenuItem(Pos, "Data/Images/Menu/Menu_Return.png", "Data/Images/Menu/Menu_Return_High.png");

    this->MainMenu.SetTitle(Settings.GetValueAt("MenuFont").c_str(), atoi(Settings.GetValueAt("MenuFont_Size").c_str()), create_color(55, 170, 250), Title_Pos, "Tank Assault");
    this->MainMenu.SetBackground("Data/Images/Menu/Menu_BG.png");
    this->MainMenu.SetMusic(Settings.ChooseValueAt("MenuMusic").c_str());
    this->MainMenu.SetHoverSound("Data/Sounds/Menu_Hover.wav");
    this->MainMenu.PlayMusic();

    this->OptionsMenu.SetTitle(Settings.GetValueAt("MenuFont").c_str(), atoi(Settings.GetValueAt("MenuFont_Size").c_str()), create_color(55, 170, 250), Title_Pos, "Tank Assault");
    this->OptionsMenu.SetBackground("Data/Images/Menu/Menu_BG.png");
    this->OptionsMenu.SetHoverSound("Data/Sounds/Menu_Hover.wav");

    int mx = 0, my = 0, dx = 0, dy = 0;

    while(!this->quit)
    {
        this->Timer.Start();
        
        /* Make sure corner 'X' isn't pressed to quit. */
        CheckQuit_Event() ? this->State = CEngine::Quit : NULL;

        /* Find mouse location */
        GetMousePosition(mx, my);

        /* Black background */
        this->Display.Clear();
        
        switch(this->State)
        {
        case CEngine::Menu:
            {
                /* Run the menu, doing what's necessary
                 * if the user clicked.
                 */
                status = this->MainMenu.Update();

                if(status == play_id)
                {
                    SDL_Delay(100);
                    this->State = CEngine::Play;
                }
                else if(status == opt_id)
                {
                    SDL_Delay(100);
                    this->State = CEngine::Options;
                }
                else if(status == quit_id)
                    this->quit = true;
            }
            break;

        case CEngine::Options:
            status = this->OptionsMenu.Update();

            if(status == mus_toggle)
            {
                if(this->MainMenu.IsPlaying())
                    this->MainMenu.PauseMusic();
                else
                    this->MainMenu.ResumeMusic();
                SDL_Delay(100); // So the button isn't pressed
                                // again immediately after play/pause
            }
            else if(status == ret_id)
            {
                SDL_Delay(100);
                this->State = CEngine::Menu;
            }
            break;

        case CEngine::Play:
            {
                /* Play in-game music rather than
                 * the menu music or any other kind.
                 */
                this->MainMenu.PauseMusic();
                
                /* Handle keyboard events */
                this->HandleEvents(dx, dy);

                if(this->Player.CheckCollision(Line.GetCollisionBox()))
                    this->Player.Move(400, 400);
                this->Logic(dx, dy);
                this->RenderAll();
                Line.Update();
            }
            break;

        case CEngine::Quit:
            this->quit = true;
        }

        /* Swap buffers and regulate frame rate */
        this->Display.Update();
        this->Timer.DelayFPS();
    }
}

void CEngine::HandleEvents(int& dx, int& dy)
{
    static SDL_Event evt;

    if(IsPressed(SDL_BUTTON_LEFT))
    {
        this->Player.Shoot(this->Bullets);
    }

    if(IsDown(SDLK_w) || IsDown(SDLK_UP))
        dy = 3;
    else if(IsDown(SDLK_s) || IsDown(SDLK_DOWN))
        dy = -3;

    if(IsDown(SDLK_d) || IsDown(SDLK_RIGHT))
        dx = -3;
    else if(IsDown(SDLK_a) || IsDown(SDLK_LEFT))
        dx = 3;

    CheckQuit(SDLK_q) ? this->State = CEngine::Quit : NULL;
}

void CEngine::Logic(int& dx, int& dy)
{
    if(dy != 0)
    {
        this->Player.Move(dy * 1.0f);
        if(!this->Map.CanPass(this->Player.GetCollisionBox()))
            this->Player.Move(-dy * 1.0f);

        dy = 0;
    }
    if(dx != 0)
    {
        this->Player.RotateBody(dx * 1.0f);
        dx = 0;
    }
}

void CEngine::RenderAll()
{
    static GL_Rect Tile_Rect(0, 0, 32, 32);

    /* Update everything on-screen */
    this->Background.Update();
    this->Map.Pan(this->Display, this->Player);
    this->Map.Update(this->Player);
    this->Player.Update();
    this->Enemies[0]->Update();

    /* Render ALL the bullets.
     * Delete any that're off-screen.
     * If they collide with any walls, 
     * show an "impact spark."
     */
    for(CGL_Bullets::iterator i = this->Bullets.begin();
        i != this->Bullets.end(); /* No third */)
    {
        gk::Tile* Tile = this->Map.FindTile((*i)->GetPosition());

        if(!Tile)
        {
            (*i)->Update();
            i++;
            continue;
        }

        Tile_Rect.Position.x = Tile->x;
        Tile_Rect.Position.y = Tile->y;

        if((*i)->IsOffscreen(this->Display) || 
            (*i)->CheckCollision(Tile_Rect) && Tile->type != gk::Tile::Floor)
        {
            (*i)->LoadEntity("Data/Images/Spark.png");
            (*i)->Update();
            delete (*i);
            i = this->Bullets.erase(i);
        }
        else
        {
            (*i)->Update();
            i++;
        }
    }
}