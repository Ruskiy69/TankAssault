/**
 * @file
 *  Declarations of the CL_Map class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.1
 *
 * @addtogroup Game
 */
/// @{

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#include "Errors.h"
#include "Helpers.h"
#include "CollapseDef.h"

#include "Math/Math.h"
#include "Graphics.h"
#include "Assets/TextureAssetManager.h"

namespace game
{
    /**
     * Throughout @a Collapse, there are three layers to every map. The lowest
     * layer is the terrain map. It describes terrain tiles, each
     * which have unique properties such as impassibility or slower movement
     * speed. The next highest layer is the collision map. This one describes 
     * collision data for all on-screen tiles. Then finally, at the highest
     * level, is the enemy path map, which specifies possible paths for
     * enemies to follow. 
     */
    class CL_Map
    {
    public:
        CL_Map(bool edit_mode = false);
        virtual ~CL_Map();

        virtual bool Load(const char* pfilename) = 0;
        virtual bool Save(const char* pfilename) = 0;

        asset::GL_Entity* FindTile(const int x, const int y) const;
        asset::GL_Entity* FindTile(const math::ML_Vector2& Position) const;
        asset::GL_Entity* FindTile(const math::ML_Rect& Area) const;

        virtual void PlaceTile(int x, int y) = 0;
        void RemoveTile(int x, int y);
        void RemoveTile(const math::ML_Vector2& Position);
        void RemoveTile(const asset::GL_Entity* p_Tile);

        bool Pan(const math::ML_Vector2& Position);

        virtual void Update(bool show_active) = 0;

        void SetPanRate(const int rate);
        const math::ML_Vector2& GetPanRate() const;
        const std::vector<asset::GL_Entity*>& GetTiles() const;
        
    protected:
        asset::GL_Entity* mp_CurrentTile;
        
        std::vector<asset::GL_Entity*> mp_allTiles;
        math::ML_Vector2 m_PanRate;
        bool m_can_edit;
        int m_pan_adjustment_rate;
    };
}

#endif // MAP_H
/// @}
