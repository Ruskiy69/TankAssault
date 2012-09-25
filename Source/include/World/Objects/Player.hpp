/**
 * @file
 *  Declarations for the CPlayer class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.5
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
 * @addtogroup Objects
 **/
/// @{

#ifndef WORLD__OBJECTS__PLAYER_HPP
#define WORLD__OBJECTS__PLAYER_HPP

#include "World/Objects/Tank.hpp"

namespace obj
{
    /// The user-controlled entity.
    class CPlayer : public CTank
    {
    public:
        CPlayer();

        bool Init(game::CSettings&);
        void IncreaseKillCount();
        void IncreaseSurvivorCount();
        void IncreaseDayCount();
        int  Update();

        void SetSpawn(const math::CVector2& Pos);
        u_int GetKills() const;
        u_int GetSurvivorsSaved() const;
        u_int GetDaysSurvived() const;
        u_int GetTowerHealth() const;
        u_int GetTankHealth() const;

        static const int DEFAULT_TANK_SPEED = 3;

    private:
        std::string m_playername;
        u_int m_days_survived, m_kills;
        u_int m_survivors_saved;
    };
}

#endif // WORLD__OBJECTS__PLAYER_HPP
/// @}