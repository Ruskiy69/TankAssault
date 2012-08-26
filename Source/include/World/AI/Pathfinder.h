/**
 * @file
 *  Declarations for the Game_Pathfinder class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup AI
 *  AI functionality and controlling.
 */
/// @{

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Assets/TextureAssetManager.h"

#include "Math/Math.h"

#include "World/Levels/TerrainMap.h"
#include "World/Levels/CollisionMap.h"

/**
 * Dynamic decision making, enemies, objects,
 * pathfinding routines, and any other AI functionality.
 */
namespace ai
{
    /**
     * Implements a custom pathfinding algorithm that's based on A*.
     */
    class AI_Pathfinder
    {
    public:
        AI_Pathfinder(const game::CL_TerrainMap& Terrain,
            const game::CL_CollisionMap& Collision) : m_Terrain(Terrain),
            m_Collision(Collision), m_current_node(0) {}
        ~AI_Pathfinder(){}

        bool FindPath(asset::GL_Entity* pStart_Tile, asset::GL_Entity* pEnd_Tile);
        void ShowPath();
        void ReversePath();

        asset::GL_Entity* NextTile();
        const math::ML_Vector2& GetNextDestination() const;
        const math::ML_Vector2& GetPrevDestination() const;
        const math::ML_Vector2& GetCurrentDestination() const;

    private:
        struct Node
        {
            Node* pParent;
            asset::GL_Entity* pTile;
            int cost;
            int move_count;
            int heuristic;
        };

        std::vector<asset::GL_Entity*> mp_Path;
        const game::CL_TerrainMap& m_Terrain;
        const game::CL_CollisionMap& m_Collision;

        int m_current_node;
    };
}

#endif // PATHFINDER_H
/// @}