#include "GenericEvents.h"

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