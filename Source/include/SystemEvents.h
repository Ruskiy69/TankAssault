/**
 * @file
 *  Various methods of event checking, such as keyboard and mouse states.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 *
 * @addtogroup Game
 */
/// @{

#ifndef GENERIC_EVENTS_H
#define GENERIC_EVENTS_H

#include "SDL/SDL.h"
#include "Math/Math.h"

namespace game
{
    Uint8*  GetKeyState();

    bool    IsDown(const SDLKey key);
    bool    IsDown_Event(const SDLKey key);
    bool    IsPressed(const int mouse_button);

    void    GetMousePosition(int& x, int& y);
    void    GetMousePosition(math::ML_Vector2& Pos);
    math::ML_Vector2 GetMousePosition();

    bool    CheckQuit_Event();
}

#endif // GENERIC_EVENTS_H

/// @}