/**
 * @file
 *	Declarations for the CProjectile class.
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
 * @addtogroup Objects
 **/
/// @{

#ifndef WORLD__OBJECTS__PROJECTILE_HPP
#define WORLD__OBJECTS__PROJECTILE_HPP

#include "Math/Math.hpp"
#include "World/Objects/GameObject.hpp"

namespace obj
{
    class CProjectile : public CGameObject
    {
    public:
        CProjectile(const float lifetime);

        void Launch(const float start_x, const float start_y,
            const float end_x, const float end_y);
        void Launch(const math::CVector2& Start,
            const math::CVector2& Target);

        virtual void Update();

        void  SetLifetime(const float lifetime);
        void  SetDamage(const u_int dmg);
        u_int GetDamage()   const;
        float GetLifetime() const;

    protected:
        u_int m_damage;
        float m_lifetime;

    private:
        static const int DEFAULT_PROJ_SPD = 16;
        math::CVector2 m_Rate;
    };
}

#endif // WORLD__OBJECTS__PROJECTILE_HPP

/// @}