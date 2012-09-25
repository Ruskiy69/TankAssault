/**
 * @file
 *  Declarations for the CTank class.
 *  
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1.3
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

#ifndef WORLD_OBJECTS_TANK_HPP
#define WORLD_OBJECTS_TANK_HPP

#include "Settings.hpp"
#include "CollapseDef.hpp"
#include "GameEvents.hpp"

#include "Math/Math.hpp"
#include "Assets/AssetManager.hpp"
#include "World/Objects/Weapon.hpp"
#include "World/Objects/GameObject.hpp"

namespace obj
{
    class CTank
    {
    public:
        CTank();
        virtual ~CTank();
    
        virtual bool Init(game::CSettings&) = 0;

        void Turn(const float deg);
        void Drive(const float speed);
        void Adjust(const math::CVector2& Rate);
        void Aim(const float x, const float y);
        void Aim(const math::CVector2& Target);
        void RotateTower(const float degrees);
        void Damage(const u_int dmg);

        bool AddWeapon(const std::string& data_file);
        bool FirePrimary();
        bool FireSecondary();
        bool IsAlive()  const;
        bool IsMoving() const;

        virtual int Update();

        const math::CVector2& GetBarrelPosition() const;
        const math::CVector2& GetPosition() const;
        const math::CRect& GetCollisionBox() const;
        obj::CWeapon& GetPrimary();
        obj::CWeapon& GetSecondary();
        obj::CGameObject* GetTankEntity();
        obj::CGameObject* GetTowerEntity();

    protected:
        obj::CGameObject    m_Tank;
        obj::CGameObject    m_Tower;

        math::CVector2      m_MovementRate;
        math::CVector2      m_BarrelPosition;

        obj::CWeapon        m_Weapon1;
        obj::CWeapon        m_Weapon2;

        bool m_moving;
    };
}

#endif // WORLD_OBJECTS_TANK_HPP

/// @}
