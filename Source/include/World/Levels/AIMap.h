/**
 * @file
 *  Declarations of the CL_ObjectiveMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.2.0
 *
 * @addtogroup Game
 */
/// @{

#ifndef OBJECTIVE_MAP_H
#define OBJECTIVE_MAP_H

#include "World/Levels/Map.h"

namespace game
{
    /// The extension for objective maps.
    static const char OBJ_MAP_EXT[] = {".cjm"};

    /**
     * Contains path information for AI movement and navigation.
     */
    class CL_ObjectiveMap : public CL_Map
    {
    public:
        CL_ObjectiveMap(bool can_edit = false);
        ~CL_ObjectiveMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void NextTile();
        void PlaceTile(int x, int y);
        void Update(bool show_active);

        asset::GL_Entity* GetNearestPOI(const math::ML_Vector2& Position) const;
        asset::GL_Entity* GetAvailableEnemySpawn(
            const std::vector<asset::GL_Entity*> p_allEnemies) const;
        asset::GL_Entity* GetPlayerSpawn() const;

    private:
        enum TileAttributes {e_POI, e_ENEMY_SPAWN, e_PLAYER_SPAWN};
        std::vector<TileAttributes> m_allTileAttributes;
        SDL_Surface* mp_Overlay;
    };
}

#endif // OBJECTIVE_MAP_H

/// @}