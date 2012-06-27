#ifndef EVENTS_H
#define EVENTS_H

#include "SDL/SDL.h"

#include "World.h"

namespace Game
{
    class Game_Events
    {
    public:
        Game_Events(Game::Game_World& World) : World(World), State(Main_Menu) {}

        enum Game_State
        {
            Main_Menu,
            Options_Menu,
            Intro,
            Play,
            Pause_Menu,
            Quit
        };

        void HandleGameEvents();

        Game_State& GetState();

    private:
        Game::Game_World& World;
        SDL_Event Evt;
        Game_State State;
    };

    const int QUIT_KEY = SDLK_ESCAPE;

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

#endif // EVENTS_H