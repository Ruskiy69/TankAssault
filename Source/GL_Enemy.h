#ifndef __CGL_ENTITY_H__
#define __CGL_ENTITY_H__

#include <vector>
#include <queue>

#include "Timer.h"
#include "Map.h"

#include "GL_Entity.h"
#include "GL_Player.h"

namespace gk_gl
{
    class CNode
    {
    public:
        CNode() : x(0), y(0), cost(0), Tile(NULL){}
        CNode(int x, int y, int cost, gk::Tile* Tile) : x(x), y(y), cost(cost), Tile(Tile){}

        inline CNode& operator=(const CNode& target)
        {
            if (*this != target)
            {
                x = target.x;
                y = target.y;
                cost = target.cost;
            }

            return *this;
        }

        inline bool operator==(const CNode& target) const
        {
            return x == target.x && y == target.y;
        }

        inline bool operator!=(const CNode& target) const
        {
            return !(*this == target);
        }

        inline bool operator<(const CNode& target) const
        {
            return target.cost < cost;
        }

        int x, y, cost;
        gk::Tile* Tile;
    };

    class CGL_Enemy : public gk_gl::CGL_Entity
    {
    public:
        CGL_Enemy(const gk::CTimer& Timer,
            const gk_gl::CGL_Player& Player,
            const gk::CMap& Map);
        ~CGL_Enemy();

        void Update();
        void Render();

    private:
        void SetupAI();
        void ResetLists();
        void FindPath();
        void UpdateAI();

        typedef std::vector<CNode> Node_List;

        const gk::CMap& Map;
        const CGL_Player& Player;

        const std::vector<gk::Tile*>& Tiles;

        Node_List   Open_List;      // Open nodes
        Node_List   Closed_List;    // Closed nodes
        Node_List   Path;           // Final path
        
        CNode* Destination;
        CNode* Start;
    };

    typedef std::vector<CGL_Enemy*> CGL_Enemies;
}

#endif // __CGL_ENTITY_H__