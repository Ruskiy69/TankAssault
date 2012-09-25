/**
 * @file
 *  Declarations for the CCollisionMap class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.2.1
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

#ifndef WORLD__LEVELS__COLLISION_MAP_HPP
#define WORLD__LEVELS__COLLISION_MAP_HPP

#include <vector>
#include <string>

#include "Errors.hpp"
#include "Helpers.hpp"

#include "World/Levels/Map.hpp"

namespace game
{
    /// Extension for terrain maps (@a Collapse Collision Map)
    static const char COLLISION_MAP_EXT[] = {".ccm"};

    /// The collision tile map.
    class CCollisionMap : public CMap
    {
    public:
        CCollisionMap(bool edit_mode = false);
        ~CCollisionMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void PlaceTile(int x, int y);
        void Update(bool show_active);
        
    private:
        SDL_Surface* mp_Overlay;
    };
}

#endif // WORLD__LEVELS__COLLISION_MAP_HPP
/// @}