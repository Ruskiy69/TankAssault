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

#include "Math/Math.h"

#include "CollapseDef.h"
#include "SystemEvents.h"

#include "Assets/AssetManagers.h"

#include "World/Levels/CollisionMap.h"
#include "World/Levels/TerrainMap.h"
#include "World/Levels/AIMap.h"

#include "World/Objects/Bullet.h"
#include "World/Objects/Player.h"
#include "World/AI/Tank.h"

namespace game
{
    /**
     * Handles a majority of the game logic and mechanics.
     * This is a primary subsystem of @a Collapse, one of several.
     * Here, collision detection is performed; maps are loaded,
     * rendered, and updated; AI logic is performed; events are
     * analyzed and acted upon. The primary engine state can be
     * changed from this subsystem.
     */
    class CL_World
    {
    public:
        CL_World(CL_GameState& Engine_State);
        ~CL_World();

        void Init();
        void HandleEvent(SDL_Event& Evt);
        void Update();

    private:
        void HandleCollisions();
        void HandleEvents();
        bool SpawnEnemy();

        math::ML_Vector2        m_PlayerRate;

        asset::GL_Entity*       mp_Background;
        
        game::CL_TerrainMap     m_TerrainMap;
        game::CL_CollisionMap   m_CollisionMap;
        game::CL_ObjectiveMap   m_ObjectiveMap;

        std::vector<obj::Obj_pBullets> mp_enemyBullets;
        obj::Obj_pBullets       mp_playerBullets;
        obj::Obj_Player         m_Player;

        game::CL_GameState&     m_engine_state;
    };
}

/// @}