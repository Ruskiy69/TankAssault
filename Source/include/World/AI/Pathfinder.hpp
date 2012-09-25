/**
 * @file
 *  Declarations for the CPathfinder class.
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
 * @addtogroup AI
 *  AI functionality and controlling.
 **/
/// @{

#ifndef WORLD__AI__PATHFINDER_HPP
#define WORLD__AI__PATHFINDER_HPP

#include "Math/Math.hpp"
#include "World/Objects/GameObject.hpp"
#include "World/Levels/Level.hpp"

/**
 * Dynamic decision making, enemies, objects,
 * pathfinding routines, and any other AI functionality.
 **/
namespace ai
{
    /// Implements a custom pathfinding algorithm that's based on A*.
    class CPathfinder
    {
    public:
        CPathfinder(game::CLevel* pCurrentLevel) : 
            mp_Level(pCurrentLevel), m_current_node(0) {}
        ~CPathfinder(){}

        bool FindPath(obj::CGameObject* pStart_Tile,
            obj::CGameObject* pEnd_Tile);
        void ShowPath();
        void ReversePath();

        obj::CGameObject* NextTile();
        const math::CVector2& GetNextDestination() const;
        const math::CVector2& GetPrevDestination() const;
        const math::CVector2& GetCurrentDestination() const;

    private:
        struct Node
        {
            Node* pParent;
            obj::CGameObject* pTile;
            int cost;
            int move_count;
            int heuristic;
        };

        std::vector<obj::CGameObject*>  mp_Path;
        game::CLevel*                   mp_Level;

        int m_current_node;
    };
}

#endif // WORLD__AI__PATHFINDER_HPP
/// @}