/**
 * @file
 *  Declarations for the CEnemyTank class.
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
 * @addtogroup AI
 **/
/// @{

#ifndef WORLD__AI__ENEMY_TANK_HPP
#define WORLD__AI__ENEMY_TANK_HPP

#include "SystemEvents.hpp"

#include "Math/Math.hpp"
#include "Assets/AssetManager.hpp"
#include "World/Objects/Tank.hpp"
#include "World/Objects/Player.hpp"
#include "World/AI/Enemy.hpp"

namespace ai
{
    /**
     * An enemy tank object, to be controlled by a global AI master.
     **/
    class CEnemyTank : public obj::CTank, public ai::CEnemy
    {
    public:
        CEnemyTank(game::CLevel* pCurrentLevel,
            const obj::CPlayer& Player);

        bool Init(game::CSettings&);
        void Spawn(const math::CVector2& Position);
        void Adjust(const math::CVector2& Rate);

        int Update();

        const math::CVector2&   GetPosition()   const;
        const obj::CGameObject* GetMainEntity() const;

    private:
        void ProcessAI();
        
        void OnPatrolling();
        void OnSearching();
        void OnAttacking();

        void FollowPath();
        void UpdateLOS();
    };
}

#endif // WORLD__AI__ENEMY_TANK_HPP
/// @}
