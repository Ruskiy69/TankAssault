/**
 * @file
 *  Declarations of the CMap class.
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
 * @addtogroup Game
 **/
/// @{

#ifndef WORLD__LEVELS__MAP_HPP
#define WORLD__LEVELS__MAP_HPP

#include <vector>
#include <string>

#include "Errors.hpp"
#include "Helpers.hpp"
#include "CollapseDef.hpp"

#include "Math/Math.hpp"
#include "Graphics/Graphics.hpp"
#include "Assets/AssetManager.hpp"
#include "World/Objects/GameObject.hpp"

namespace game
{
    /**
     * The base class for the map layers.
     *  Throughout @a Collapse, there are three layers to every map.
     *  The lowest layer is the terrain map. It describes terrain tiles,
     *  each which have unique properties such as impassibility or slower
     *  movement speed. The next highest layer is the collision map.
     *  This one describes collision data for all on-screen tiles.
     *  Then finally, at the highest level, is the objective map, which
     *  specifies spawn points, player objectives, AI points-of-interest,
     *  etc.
     **/
    class CMap
    {
    public:
        CMap(bool edit_mode = false);
        virtual ~CMap();

        virtual bool Load(const char* pfilename) = 0;
        virtual bool Save(const char* pfilename) = 0;

        obj::CGameObject* FindTile(const int x, const int y) const;
        obj::CGameObject* FindTile(const math::CVector2& Position) const;
        obj::CGameObject* FindTile(const math::CRect& Area) const;

        virtual void PlaceTile(int x, int y) = 0;
        void RemoveTile(int x, int y);
        void RemoveTile(const math::CVector2& Position);
        void RemoveTile(const obj::CGameObject* p_Tile);

        bool Pan(const math::CVector2& Position);

        virtual void Update(bool show_active) = 0;

        void SetPanRate(const int rate);
        const math::CVector2& GetPanRate() const;
        const std::vector<obj::CGameObject*>& GetTiles() const;
        
    protected:
        obj::CGameObject*   mp_CurrentTile;
        math::CVector2      m_PanRate;

        std::vector<obj::CGameObject*> mp_allTiles;
        bool m_can_edit;
        int m_pan_adjustment_rate;
    };
}

#endif // WORLD__LEVELS__MAP_HPP

/// @}