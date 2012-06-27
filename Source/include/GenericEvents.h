#ifndef GENERIC_EVENTS_H
#define GENERIC_EVENTS_H

#include "SDL/SDL.h"

namespace Game
{
    static const int QUIT_KEY = SDLK_ESCAPE;

    /* Get the current state of the keyboard */
    Uint8*  GetKeyState();

    /* Check if a button on the keyboard is down */
    bool    IsDown(const SDLKey key);
    bool    IsDown_Event(const SDLKey key);

    /* Check if a mouse button is down */
    bool    IsPressed(const int mouse_button);

    /* Get the current mouse position */
    void    GetMousePosition(int& x, int& y);

    /* Check if the key for quitting (declared above)
     * is pressed down, or check if another key used
     * to quit is pressed down, or check for an SDL_QUIT
     * event.
     */
    bool    CheckQuit();
    bool    CheckQuit(const SDLKey key);
    bool    CheckQuit_Event();
}

#endif // GENERIC_EVENTS_H