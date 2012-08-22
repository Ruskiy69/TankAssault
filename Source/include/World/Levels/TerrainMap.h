/**
 * @file
 *  Declarations for the TerrainMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.1
 *
 * @addtogroup Game
 * @bug Sometimes tiles aren't replaced properly.
 */
/// @{

#ifndef TERRAIN_MAP_H
#define TERRAIN_MAP_H

#include <vector>
#include <string>

#include "Errors.h"
#include "Helpers.h"

#include "World/Levels/Map.h"

namespace game
{
    /// Extension for terrain maps (@a Collapse Terrain Map)
    static const char TERRAIN_MAP_EXT[] = {".ctm"};

    /**
     * The terrain tile map.
     */
    class CL_TerrainMap : public CL_Map
    {
    public:
        CL_TerrainMap(bool edit_mode = false);
        ~CL_TerrainMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void NextTile();
        void PlaceTile(int x, int y);
        void Update(bool show_active);
        
    private:
        bool IsValidTextureName(const char* ptexture_name);
        asset::GL_Entity* GetEntityFromFilename(const char* p_filename);

        std::vector<std::string> m_textureNames;
        std::vector<asset::GL_Entity*> m_textures;
    };
}

#endif // TERRAIN_MAP_H
/// @}