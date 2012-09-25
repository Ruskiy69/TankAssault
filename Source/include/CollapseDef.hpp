/**
 * @file
 *	Global variable declarations and definitions for @a Collapse.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
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

#ifndef COLLAPSE_DEF_HPP
#define COLLAPSE_DEF_HPP

#include "Logging.hpp"
#include "Settings.hpp"

/**
 * A collection of stuff that is unique to each individual game,
 * or is a class that uses graphical elements but does not interface
 * with OpenGL directly.
 **/
namespace game
{
    /// Various game states
    enum GameState
    {
        e_SPLASH = 0,
        e_INTRO,
        e_MAINMENU,
        e_OPTIONSMENU,
        e_PAUSEMENU,
        e_GAME,
        e_INVENTORY,
        e_QUIT,
        e_STATE_COUNT
    };
    
    /// Globally accessible logging class singleton.
    static gk::CLogging& g_Log = gk::CLogging::GetInstance();

    /// Globally accessible settings class.
    static game::CSettings& g_Settings = game::CSettings::GetInstance();
}

#endif // COLLAPSE_DEF_HPP

/// @}