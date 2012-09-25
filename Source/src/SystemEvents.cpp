/**
 * @file
 *  Implementation of basic event handling functions.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 **/

#include "SystemEvents.hpp"

Uint8* game::GetKeyState()
{
    return SDL_GetKeyState(NULL);
}

bool game::IsDown(const SDLKey KEY)
{
    return game::GetKeyState()[KEY];
}

bool game::IsDown_Event(const SDLKey key)
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

bool game::IsPressed(const int button)
{
    return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}


void game::GetMousePosition(int& x, int& y)
{
    SDL_GetMouseState(&x, &y);
}

math::CVector2 game::GetMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return math::CVector2(x, y);
}

void game::GetMousePosition(math::CVector2& Pos)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    Pos.Move(x, y);
}
