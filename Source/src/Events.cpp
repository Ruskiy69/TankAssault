#include "Events.h"

using Game::Game_Events;

void Game_Events::HandleGameEvents()
{
    static int speed = 0, angle = 0;

    while(SDL_PollEvent(&Evt))
    {
        switch(Evt.type)
        {
        case SDL_QUIT:
            this->State = Game_Events::Quit;
            break;

        case SDL_MOUSEBUTTONDOWN:
            printf("MB Down\n");
            if(Evt.button.button == SDL_BUTTON_LEFT)
            {
                if(this->State == Game_Events::Play)
                {
                    this->World.Shoot();
                }
            }
            break;

        case SDL_KEYDOWN:
            if(this->State == Game_Events::Play)
            {
                switch(Evt.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_UP:
                    speed = this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_s:
                case SDLK_DOWN:
                    speed = -this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_d:
                case SDLK_RIGHT:
                    angle = -this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_a:
                case SDLK_LEFT:
                    angle = this->World.GetPlayer().PLAYER_SPEED;
                    break;
                }
            }

            break;

        case SDL_KEYUP:
            if(this->State == Game_Events::Play)
                {
                switch(Evt.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_s:
                case SDLK_UP:
                case SDLK_DOWN:
                    speed = 0;
                    break;

                case SDLK_d:
                case SDLK_a:
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    angle = 0;
                    break;
                }
            }
            break;
        }
    }

    if(this->State == Game_Events::Play)
    {
        this->World.MovePlayer(speed*1.0f);
        this->World.RotatePlayer(angle*1.0f);
    }
}

Game::Game_Events::Game_State& Game_Events::GetState()
{
    return this->State;
}

Uint8* Game::GetKeyState()
{
    return SDL_GetKeyState(NULL);
}

bool Game::IsDown(const SDLKey KEY)
{
    return Game::GetKeyState()[KEY];
}

bool Game::IsPressed(const int button)
{
    return (bool)(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}

void Game::GetMousePosition(int& x, int& y)
{
    SDL_GetMouseState(&x, &y);
}

bool Game::CheckQuit()
{
    return Game::GetKeyState()[Game::QUIT_KEY];
}

bool Game::CheckQuit(SDLKey key)
{
    return Game::IsDown(key);
}

bool Game::CheckQuit_Event()
{
    static SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
            return true;
    }

    return false;
}

bool Game::IsDown_Event(const SDLKey key)
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_KEYDOWN)
            if(evt.key.keysym.sym == key)
                return true;
    }

    return false;
}