/**
 * @file
 *  Declarations for the CTerrainMap class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1.1
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
 * @bug Sometimes tiles aren't replaced properly.
 **/
/// @{

#ifndef WORLD__LEVELS__TERRAIN_MAP_H
#define WORLD__LEVELS__TERRAIN_MAP_H

#include <vector>
#include <string>

#include "Errors.hpp"
#include "Helpers.hpp"

#include "World/Levels/Map.hpp"

namespace game
{
    /// Extension for terrain maps (@a Collapse Terrain Map)
    static const char TERRAIN_MAP_EXT[] = {".ctm"};

    /// The terrain tile map.
    class CTerrainMap : public CMap
    {
    public:
        CTerrainMap(bool edit_mode = false);
        ~CTerrainMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void NextTile();
        void PlaceTile(int x, int y);
        void Update(bool show_active);
        
    private:
        bool IsValidTextureName(const char* ptexture_name);

        std::vector<std::string> m_textureNames;
    };
}

#endif // WORLD__LEVELS__TERRAIN_MAP_H
/// @}