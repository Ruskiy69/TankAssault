/**
 * @file
 *	Declarations for the CWorld class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.1
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
 **/
/// @{

#ifndef WORLD__WORLD_HPP
#define WORLD__WORLD_HPP

#include "CollapseDef.hpp"
#include "SystemEvents.hpp"
#include "GameEvents.hpp"

#include "Math/Math.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Light.hpp"

#include "World/Levels/Level.hpp"
#include "World/Objects/Bullet.hpp"
#include "World/Objects/Player.hpp"
#include "World/AI/EnemyTank.hpp"

namespace game
{
    /**
     * Handles a majority of the game logic and mechanics.
     * This is a primary subsystem of @a Collapse, one of several.
     * Here, collision detection is performed; maps are loaded,
     * rendered, and updated; AI logic is performed; events are
     * analyzed and acted upon. The primary engine state can be
     * changed from this subsystem.
     **/
    class CWorld
    {
    public:
        CWorld(GameState& engine_state);
        ~CWorld();

        void Init();
        void HandleEvent(SDL_Event& Evt);
        void Update();

        void HandleSystemEvent(const SDL_Event& Evt);
        void HandleGameEvent(const game::GameEvent* pEvt);

        obj::CPlayer& GetPlayer();

    private:
        void HandleCollisions();
        void HandleWorldEvents();
        bool SpawnEnemy();

        math::CVector2  m_PlayerRate;
        game::CLevel*   mp_ActiveLevel;
        obj::CEntity    m_Background;
        obj::CPlayer    m_Player;
        gfx::CShader    m_Lighting;

        std::vector<game::CLevel*>  mp_Levels;
        //std::vector<gfx::CLight*>   mp_Lights;
        std::list<ai::CEnemyTank*>  mp_Enemies;
        obj::pBulletCollection      mp_enemyBullets;
        obj::pBulletCollection      mp_playerBullets;

        game::GameState&    m_engine_state;

        float*          mp_enemy_light_poss;
        float*          mp_enemy_light_cols;
        float*          mp_enemy_light_atts;
    };
}

#endif // WORLD__WORLD_HPP

/// @}