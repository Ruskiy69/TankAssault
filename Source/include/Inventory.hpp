/**
 * @file
 *	Declarations for the CInventory class
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.1
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
 * @addtogroup Inventory
 */
/// @{

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "CollapseDef.hpp"
#include "SystemEvents.hpp"
#include "Assets/Font.hpp"
#include "Assets/AssetManager.hpp"
#include "World/Objects/Entity.hpp"
#include "World/Objects/Player.hpp"

namespace game
{
    class CInventory
    {
    public:
        CInventory(obj::CPlayer& Player) : m_Player(Player) {}
        ~CInventory(){}

        bool Init();
        void Update();
        void HandleInventoryEvents();
        void HandleGameEvent();
        void HandleSystemEvent(const SDL_Event& Evt);

    private:
        obj::CEntity    m_Background;
        obj::CEntity    m_LargePlayerIMG;
        obj::CEntity*   mp_Weapon1Stats;
        obj::CEntity*   mp_Weapon2Stats;
        obj::CEntity*   mp_PlayerStats;
        obj::CEntity*   mp_PlayerHealth;
        obj::CEntity*   mp_TowerHealth;
        obj::CEntity*   mp_TankHealth;
        obj::CPlayer&   m_Player;
        asset::CFont*   mp_Font;
    };

    gfx::Color color_from_health(const u_int health);
}

#endif // INVENTORY_HPP

/// @}