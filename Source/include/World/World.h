/**
 * @file
 *  Declaration of the CL_World class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Game
 */
/// @{
/*
#include "Math/MathHelper.h"

#include "CollapseDef.h"
#include "SystemEvents.h"

#include "World/CollisionMap.h"
#include "World/TerrainMap.h"
#include "World/AIMap.h"

#include "Game/World/Objects/Bullet.h"
#include "Game/World/Objects/Player.h"
#include "Game/World/Objects/AI/Tank.h"

namespace game
{
    /**
     * Handles a majority of the game logic and mechanics.
     * This is a primary subsystem of @a Collapse, one of several.
     * Here, collision detection is performed; maps are loaded,
     * rendered, and updated; AI logic is performed; events are
     * analyzed and acted upon. The primary engine state can be
     * changed from this subsystem.
     *
    class CL_World
    {
    public:
        CL_World(CL_GameState& Engine_State);
        ~CL_World();

        void HandleEvent(SDL_Event& Evt);
        void Update();

    private:
        void HandleCollisions();
        void HandleEvents();
        bool SpawnEnemy();

        math::Vector2           m_PlayerRate;

        gfx::GL_Entity          m_Background;
        
        game::Game_TerrainMap   m_TerrainMap;
        game::Game_CollisionMap m_CollisionMap;
        game::Game_AIMap        m_AIMap;

        std::vector<obj::Obj_pBullets> mp_enemyBullets;
        obj::Obj_pBullets       mp_playerBullets;
        obj::Obj_Player         m_Player;

        game::Game_State&       m_engine_state;
    };
}*/

/// @}