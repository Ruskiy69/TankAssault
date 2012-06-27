/**************** EVENTHANDLER.H ********************
 *                                                  *
 * In contrast with GenericEvents.h, this file is   *
 * usually very game-specific, providing dedicated, *
 * unique event handling and functionality.         *
 *                                                  *
 ***************************************************/

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "SDL/SDL.h"

#include "World.h"
#include "Graphics/HUD.h"

namespace Game
{
    class Game_Events
    {
    public:
        Game_Events(Game_World& World, Game_HUD& HUD) : World(World), HUD(HUD), State(Main_Menu) {}

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
        Game_World& World;
        Game_HUD& HUD;
        SDL_Event Evt;
        Game_State State;
    };
}

#endif // EVENT_HANDLER_H