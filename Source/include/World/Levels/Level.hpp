/**
 * @file
 *	Declarations of the CLevel class.
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
 * @addtogroup Game
 **/
/// @{

#ifndef WORLD__LEVELS__LEVEL_HPP
#define WORLD__LEVELS__LEVEL_HPP

#include "World/Levels/TerrainMap.hpp"
#include "World/Levels/CollisionMap.hpp"
#include "World/Levels/ObjectiveMap.hpp"

namespace game
{
    class CLevel
    {
    public:
        CLevel();

    	bool LoadLevel(const int level_no);
        void PanMaps(const math::CVector2& Pos);
        void Update();

        void SetPanRate(const float rate);

        game::CTerrainMap&   GetTerrainMap();
        game::CCollisionMap& GetCollisionMap();
        game::CObjectiveMap& GetObjectiveMap();
        const std::string&   GetLevelName() const;

    private:
        game::CTerrainMap   m_TerrainMap;
        game::CCollisionMap m_CollisionMap;
        game::CObjectiveMap m_ObjectiveMap;
        std::string         m_levelname;
    };
}

#endif // WORLD__LEVELS__LEVEL_HPP

/// @}