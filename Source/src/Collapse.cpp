/**
 * @file
 *  Main program file.
 *  All libraries are linked here, and all subsystem initialization is
 *  performed here as well, excluding the GLEW library, because that must
 *  be initialized after an OpenGL context exists.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.8.0
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
 * @mainpage Collapse Documentation
 *
 * @section collapse_install Installation
 *  @a Collapse is an easy install. Simply extract Collapse.zip and run
 *  Collapse.exe. An actual installer may be included with the final
 *  release, provided it's deemed necessary.
 *
 * @section collapse_descr About
 *  @a Collapse is a tank game set in a post-apocalyptic world in which
 *  human-created AI, known as the Mechs, have taken over the world. You
 *  play as Sgt. Ivanov, last survivor of the Eurasian Guerrilla Coalition,
 *  and possibly the last survivor of mankind. @a Collapse was written by
 *  me, George Kudrayvtsev, in my spare time during much of 2012. I wrote a
 *  2-D engine from scratch using OpenGL and SDL together. A portion of the
 *  artwork is mine and another is my friend's, who contributed to the
 *  project. All 6600 some-odd (as of 9.24.12, version 0.8pr) lines of
 *  code are written completely by me, consulting outside sources for some
 *  aspects, of course, but absolutely no copy-paste, which I'm pretty
 *  proud of. \n
 *
 * @section collapse_design Engine Design
 *  I decided to implement a state-driven engine system because it allows
 *  the most versatility, in my opinion. The engine is broken into three
 *  primary subsystems: World, HUD, and Menus. Each primary subsystem has
 *  access to a CEntityGameState& reference, so it can modify it as it wishes,
 *  changing how the base engine interacts with its subsystems.
 *  @see CollapseDef.hpp
 *      @subsection collapse_world World
 *          The World subsystem focuses on game-mechanics like player and
 *          enemy logic, collision detection, level loading/updating, and
 *          other things. It interacts with the engine in only one way,
 *          changing the engine state.
 *      @subsection collapse_hud HUD
 *          The HUD subsystem takes care of the UI, such as health/ammo
 *          bars, power-up display, as opposed to the @ref collapse_world's
 *          responsibility of actually using and applying the power-ups, as
 *          well as interacting with the inventory layout. Both the World
 *          and HUD subsystems depend on the inventory, interacting with it
 *          in different ways.
 *      @subsection collapse_menus Menus
 *          The Menus subsystem does what it sounds like it does: takes
 *          care of any menu operations. It takes care of loading, event
 *          handling, button clicks, music, and rendering. Absolutely no
 *          interaction with the other sections, nor the engine, other than
 *          a state change when the player wants to play, or switch menus,
 *          for example.
 **/

#include "Engine.hpp"

// Link OpenGL and GLEW libraries.
// These are located in the system path
/// @todo Add to lib sub folder.
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "GLEW32.lib")

// Link audio (OpenAL, ALUT, lib-vorbis) libraries.
#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")
#pragma comment(lib, "libvorbisfile.lib")

// Link SDL libraries.
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")

using game::g_Log;

// Function headers
bool init();
void quit();

/**
 * Executes the program.
 * @param int Argument count
 * @param char* Arguments
 * @return Zero, no matter what.
 **/
int main(int argc, char* argv[])
{
    // Testing how to load resources (for shaders later)
    /*
    HRSRC hres = FindResource(NULL, "#102", "TEXT");
    if(hres == 0)
    {
        printf("Failed.\n");
        return 1;
    }

    HGLOBAL hbytes = LoadResource(NULL, hres);
    LPVOID data = LockResource(hbytes);

    printf("Data: %s\n", (char*)data);
    UnlockResource(hbytes);
    return 0;
    **/

    // Initialize all libraries.
    // If initialization fails, log the error and shut down.
    if(!init())
    {
        char* error = SDL_GetError();
        g_Log.Flush();
        g_Log << "[ERROR] Initialization error: " << error;
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    g_Log.Flush();
    g_Log << "[INFO] Debug build: ";
#ifdef _DEBUG
    g_Log << "true.\n";
#else
    g_Log << "false.\n";
#endif _DEBUG

    g_Log.Flush();
    g_Log << "[INFO] Initializing game engine.\n";

    game::CEngine Collapse;
    Collapse.Init();
    Collapse.GameLoop();
    
    // Log data and shut down libraries.
    g_Log.Flush();
    g_Log << "[INFO] Quitting library sub-systems.\n";
    g_Log.Close();

    quit();

    return 0;
}

/**
 * Initialize SDL and all SDL dependent libraries.
 * @return TRUE on successful initialization of ALL systems, FALSE otherwise.
 **/
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1)
        return false;
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        return false;
    if(TTF_Init() == -1)
        return false;

    asset::CSound2D::InitializeOpenAL();

    return true;
}

/// Quit all SDL subsystems.
void quit()
{
    alutExit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
