/**
 * @file
 *  Declarations of the CObjectiveMap class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.3.0
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

#ifndef WORLD__LEVELS__OBJECTIVE_MAP_H
#define WORLD__LEVELS__OBJECTIVE_MAP_H

#include <vector>

#include "Graphics/Light.hpp"
#include "World/Levels/Map.hpp"

namespace game
{
    /// The extension for objective maps.
    static const char OBJ_MAP_EXT[] = {".cjm"};

    /// Contains path information for AI movement and navigation.
    class CObjectiveMap : public CMap
    {
    public:
        CObjectiveMap(bool can_edit = false);
        ~CObjectiveMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void NextTile();
        void PlaceTile(int x, int y);
        bool Pan(const math::CVector2& Position);
        void Update(bool show_active);

        obj::CGameObject* GetNearestPOI(const math::CVector2& Position) const;
        obj::CGameObject* GetAvailableEnemySpawn(
            const std::vector<const obj::CGameObject*>& p_allEnemies) const;
        obj::CGameObject* GetPlayerSpawn() const;
        std::vector<gfx::CLight*>& GetLights();

    private:
        enum TileAttributes {e_POI, e_ENEMY_SPAWN, e_PLAYER_SPAWN, e_LIGHT};
        std::vector<TileAttributes> m_allTileAttributes;
        std::vector<gfx::CLight*> mp_allLights;
        SDL_Surface* mp_Overlay;
    };
}

#endif // WORLD__LEVELS__OBJECTIVE_MAP_H

/// @}