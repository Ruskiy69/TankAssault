/**
 * @file
 *  Definitions for the CBullet class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.3.1
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

#ifndef WORLD__OBJECTS__BULLET_HPP
#define WORLD__OBJECTS__BULLET_HPP

#include <list>

#include "World/Objects/Projectile.hpp"

namespace obj
{
    /**
     * A base class for any type of bullet. Handles motion,
     * target location, and updating. The only thing necessary
     * for the inheriting classes is to load the bullet image
     * and set the damage.
     **/
    class CBullet : public CProjectile
    {
    public:
        CBullet() : CProjectile(60.0f) {}
    };

    /// Easily create a container of bullets
    typedef std::list<obj::CBullet*> pBulletCollection;
}

#endif // WORLD__OBJECTS__BULLET_HPP

/// @}