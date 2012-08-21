/**
 * @file
 *	Global variable declarations and definitions for @a Collapse.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Game
 */
/// @{

#ifndef COLLAPSE_DEF_H
#define COLLAPSE_DEF_H

#include "Logging.h"

/**
 * A collection of stuff that is unique to each individual game,
 * or is a class that uses graphical elements but does not interface
 * with OpenGL directly.
 */
namespace game
{
    /// Various game states
    enum CL_GameState
    {
        e_SPLASH = 0,
        e_INTRO,
        e_MAINMENU,
        e_OPTIONSMENU,
        e_PAUSEMENU,
        e_GAME,
        e_QUIT
    };
    
    /// Globally accessible logging class singleton.
    static gk::Logging& g_Log = gk::Logging::GetInstance();
}

#endif // COLLAPSE_DEF_H

/// @}