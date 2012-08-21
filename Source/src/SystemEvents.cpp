/**
 * @file
 *  Implementation of basic event handling functions.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 */

#include "SystemEvents.h"

/**
 * Retrieves the current state of the keyboard.
 *
 * @return An array of 8-bit integers representing the keyboard state.
 */
Uint8* game::GetKeyState()
{
    return SDL_GetKeyState(NULL);
}

/**
 * Checks if a key is being held down.
 *
 * @param SDLKey The key to check
 *
 * @return TRUE if down, FALSE otherwise.
 */
bool game::IsDown(const SDLKey KEY)
{
    return game::GetKeyState()[KEY];
}

/**
 * Checks the event queue if a certain key was pressed. It's not recommended
 * to use this method, because it renders the rest of the events unchecked
 * and useless.
 *
 * @param SDLKey Key to check
 *
 * @return TRUE if found pressed, FALSE otherwise.
 */
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

/**
 * Checks if a mouse button is being held down.
 *
 * @param int Button to check (1 = Left)
 *
 * @return TRUE if down, FALSE otherwise.
 *
 * @see SDL.h
 */
bool game::IsPressed(const int button)
{
    return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}

/**
 * Retrieves the current mouse position.
 *
 * @param int The x-coordinate to receive data into
 * @param int The y-coordinate to receive data into
 */
void game::GetMousePosition(int& x, int& y)
{
    SDL_GetMouseState(&x, &y);
}

/**
 * Retrieves the current mouse position in a vector.
 *
 * @return The vector representing mouse position.
 */
math::ML_Vector2 game::GetMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return math::ML_Vector2(x, y);
}

/**
 * Modifies a given vector with the current mouse position.
 *
 * @param The vector representing mouse position.
 */
void game::GetMousePosition(math::ML_Vector2& Pos)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    Pos.Move(x, y);
}

/**
 * Checks the event queue for an SDL_QUIT event.
 *
 * @return TRUE if there is one, FALSE otherwise.
 */
bool game::CheckQuit_Event()
{
    static SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
            return true;
    }

    return false;
}
