/**
 * @file
 *  Main program file.
 *  All libraries are linked here, and all subsystem initialization is
 *  performed here as well, excluding the GLEW library, because that must
 *  be initialized after an OpenGL context exists.
 *
 * @author George Kudrayvtsev
 * @version 0.7.4
 *
 * @mainpage Collapse Documentation
 * Version 0.7.4a pre-release
 *
 * @section collapse_install Installation
 *  @a Collapse is an easy install. Simply extract Collapse.zip and run
 *  Collapse.exe. An actual installer may be included with the final
 *  release, provided it's deemed necessary.
 *
 * @section collapse_descr About
 *  @a Collapse is a tank game set in a post-apocalyptic world in which
 *  human-created AI, known as the Mechs, have taken over the world. You
 *  play as Sgt. Ivanov, last survivor of the Eurasian Geurilla Coalition,
 *  and possibly the last survivor of mankind. @a Collapse was written by
 *  me, George Kudrayvtsev, in my spare time during much of 2012. I wrote a
 *  2-D engine from scratch using OpenGL and SDL together. A portion of the
 *  artwork is mine and another is my friend's, who contributed to the
 *  project. All 6500 some-odd (as of 8.28.12, version 0.7.4a) lines of
 *  code are written completely by me, consulting outside sources for some
 *  aspects, of course, but absolutely no copy-paste, which I'm pretty
 *  proud of. \n
 *  It is licensed under the LGPL (Liberated GNU Public License), which is
 *  included with the game.
 *
 * @section collapse_design Engine Design
 *  I decided to implement a state-driven engine system because it allows
 *  the most versatility, in my opinion. The engine is broken into three
 *  primary subsystems: World, HUD, and Menus. Each primary subsystem has
 *  access to a CL_GameState& reference, so it can modify it as it wishes,
 *  changing how the base engine interacts with its subsystems.
 *  @see Game/Definitions.h
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
 * @section code_style Code Style
 *  The majority of the source has adopted a uniform style so, divided into
 *  "libraries" with custom prefixes so that, throughout development, I
 *  automatically know what type of object I'm dealing with and what other
 *  libraries it interacts with.
 *      @subsection audio Audio Code
 *          All classes and structures that interface directly with OpenAL,
 *          or are exclusively dedicated to audio management, are prefixed
 *          with AL ([A]udio [L]ibrary). \n
 *          For example, AL_Sound2D is a class that deals with
 *          sound effects in a 2-dimensional environment.
 *      @subsection graphics Graphics Code
 *          All classes and structures that interface directly with OpenGL,
 *          or are exclusively dedicated to managing textures, pixels,
 *          or other graphical elements are prefixed with GL ([G]raphics
 *          [L]ibrary).
 *          For example, GL_Entity is a base class for all on-screen
 *          objects.
 *      @subsection math Math Code
 *          A large portion of the code base deals with math, so there are 
 *          more than a few classes dedicated to linear algerba and
 *          physics. These classes are prefixed with ML ([M]ath [L]ibrary).
 *          For example, ML_Vector2 is a C++ representation of a
 *          2-dimensional vector.
 *      @subsection game Game Code
 *          Though all of the above "libraries" deal with the game, code
 *          that falls into this category is very game-related and cannot
 *          fall into the other categories. It is prefixed with
 *          CL ([C]ollapse [L]ibrary).
 *          For example, CL_Menu is the menu class used in the game.
 */

#include "Engine.h"

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
 *
 * @param int Argument count
 * @param char* Arguments
 * 
 * @return Zero, no matter what.
 */
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
    */

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

    game::CL_Engine Collapse;
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
 *
 * @return TRUE on successful initialization of ALL systems, FALSE otherwise.
 */
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1)
        return false;
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        return false;
    if(TTF_Init() == -1)
        return false;

    asset::AL_Sound2D::InitializeOpenAL();

    return true;
}

/**
 * Quit all SDL subsystems.
 */
void quit()
{
    alutExit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
