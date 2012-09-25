/**
 * @file
 *  Various methods of event checking, such as keyboard and mouse states.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Game
 **/
/// @{

#ifndef SYSTEM_EVENTS_HPP
#define SYSTEM_EVENTS_HPP

#include "SDL/SDL.h"
#include "Math/Math.hpp"

namespace game
{
    /**
     * Retrieves the current state of the keyboard.
     * @return An array of 8-bit integers representing the keyboard state.
     **/
    Uint8*  GetKeyState();

    /**
     * Checks if a key is being held down.
     * @param SDLKey The key to check
     * @return TRUE if down, FALSE otherwise.
     **/
    bool    IsDown(const SDLKey key);

    /**
     * Checks the event queue if a certain key was pressed. It's not recommended
     * to use this method, because it renders the rest of the events unchecked
     * and useless.
     *
     * @param SDLKey Key to check
     * @return TRUE if found pressed, FALSE otherwise.
     **/
    bool    IsDown_Event(const SDLKey key);

    /**
     * Checks if a mouse button is being held down.
     *
     * @param int Button to check (1 = Left)
     * @return TRUE if down, FALSE otherwise.
     * @see SDL.h
     **/
    bool    IsPressed(const int mouse_button);

    /**
     * Retrieves the current mouse position.
     *
     * @param int The x-coordinate to receive data into
     * @param int The y-coordinate to receive data into
     **/
    void    GetMousePosition(int& x, int& y);
    void    GetMousePosition(math::CVector2& Pos);
    math::CVector2 GetMousePosition();
}

#endif // SYSTEM_EVENTS_HPP

/// @}