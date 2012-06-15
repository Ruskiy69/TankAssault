#ifndef __CGL_ENTITY_H__
#define __CGL_ENTITY_H__

#include <vector>

#include "Timer.h"
#include "Map.h"

#include "GL_Entity.h"
#include "GL_Player.h"

namespace gk_gl
{
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
        void InitAI();
        void AI();
        void RenewLists();

        struct Node
        {
            gk::Tile* Tile;
            int f, g, h;
        };

        const gk::CMap& Map;
        const CGL_Player& Player;

        gk_gl::GL_Vertex2f Destination;

        const std::vector<gk::Tile*>& Tiles;

        std::vector<Node*>  Open_List;      // Open nodes
        std::vector<Node*>  Closed_List;    // Closed nodes
    };

    typedef std::vector<CGL_Enemy*> CGL_Enemies;
}

#endif // __CGL_ENTITY_H__