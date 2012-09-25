/**
 * @file
 *	Declarations for the CGameObject class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1.2
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

#ifndef WORLD_OBJECTS_COLLIDABLE_HPP
#define WORLD_OBJECTS_COLLIDABLE_HPP

#include "World/Objects/Entity.hpp"

namespace obj
{
    /**
     * The basic game object.
     *  It has a health value and collision boxes, and is meant to be
     *  inherited, except for various cases where the basic functionality
     *  provides everything necessary.
     **/
    class CGameObject : public obj::CEntity
    {
    public:
        // Ctor
        CGameObject() : m_CollisionBox(0, 0, 0, 0), m_health(100) {}

        // Loading methods
        bool LoadFromFile(const char* pfilename);
        bool LoadFromTexture(const asset::CTexture* pTexture);
        bool LoadFromSurface(SDL_Surface* pSurface);

        // Collision methods
        bool CheckCollision(const CGameObject* pOther) const;
        bool CheckCollision(const obj::CEntity* pOther) const;
        bool CheckCollision(const math::CRect& Box) const;
        bool CheckCollision(const float x, const float y) const;

        // Resizing
        void ResizeCollisionBox(const u_int w, const u_int h);
        void ResizeCollisionBox(const math::CRect& New_Size);
        void AdjustCollisionBox(const u_int dx, const u_int dy);

        // Hurting
        void Damage(const u_int dmg);

        // Updating
        virtual void Update();

        // Modifiers
        void SetCollisionBox(const math::CRect& Collision_Box);

        // Accessors
        const math::CRect& GetCollisionBox() const;
        u_int GetHealth() const;
        bool  IsAlive()   const;

    private:
        // Member variables
        math::CRect m_CollisionBox;

        u_int m_health;
    };

    /// Temporary backward compatibility
    typedef CGameObject CGameObject;
}

#endif // WORLD_OBJECTS_COLLIDABLE_HPP

/// @}