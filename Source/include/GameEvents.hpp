/**
 * @file
 *	Declarations for structs and classes related to the GameEvent system
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0.3
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

#ifndef GAME_EVENTS_HPP
#define GAME_EVENTS_HPP

#include <deque>
#include "CollapseDef.hpp"

namespace obj   { class CWeapon; class CEntity; }

namespace game
{
    /// Various event types
    enum GameEventType
    {
        e_NONE = -1,
        e_MOVEMENT_REQUEST,
        e_STATE_CHANGE,
        e_EVENT_COUNT
    };

    /// A game event.
    struct GameEvent
    {
        GameEventType   evt_type;
        GameState       new_state;
        obj::CWeapon*   pWeapon;
        obj::CEntity*   pEntity;
    };

    /// Controls events pushed by in-game objects.
    class CGameEventQueue
    {
    public:
        ~CGameEventQueue();

        void PushEvent(GameEvent* pEvt);
        GameEvent* PopEvent();
        static CGameEventQueue& GetInstance();

    private:
        CGameEventQueue(){}
        CGameEventQueue(CGameEventQueue&);
        CGameEventQueue& operator=(CGameEventQueue&);

        std::deque<GameEvent*> mp_eventQueue;
    };

    /// Globally accessible event queue.
    static CGameEventQueue& g_GameEventQueue = CGameEventQueue::GetInstance();
}

#endif // GAME_EVENTS_HPP


/// @}