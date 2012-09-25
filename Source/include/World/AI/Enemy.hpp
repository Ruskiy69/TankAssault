/**
 * @file
 *  Declarations for the CEnemy class.
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
 **/
/// @{

#ifndef WORLD__AI__ENEMY_H
#define WORLD__AI__ENEMY_H

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

#include <list>

#include "CollapseDef.hpp"
#include "World/Levels/Level.hpp"
#include "World/Objects/Player.hpp"
#include "World/AI/Pathfinder.hpp"

namespace ai
{
    /// Various phases of AI action which can be combined together as flags.
    enum AIState
    {
        e_NONE                  = (1 << 0),     /// Doing nothing
        e_PATHFINDING           = (1 << 1),     /// Following the current path
        e_DONE                  = (1 << 2),     /// Finished following the current path
        e_FIRING_PRIMARY        = (1 << 3),     /// These next four are self-explanatory
        e_FIRING_SECONDARY      = (1 << 4),
        e_RELOADING_PRIMARY     = (1 << 5),
        e_RELOADING_SECONDARY   = (1 << 6),
        e_NO_PATH               = (1 << 7)      /// @see ai::CEnemyPathfinder::FindPath()
    };

    void PrintState(int state);

    /// Abstract base class for all AI-controlled enemies.
    class CEnemy
    {
    protected:
        enum AITactic
        {
            e_PATROLLING,   // Passive patrolling
            e_SEARCHING,    // Active searching for player
            e_ATTACKING     // Aggressive attacking and advancing toward player
        } m_tactic;

        // The state is declared as an int because otherwise I cannot do
        // bitwise operations like &= and |= on the state due to
        // AIState being incompatible.
        int m_state;

        void SetTactic(AITactic tactic);
        void RemoveState(AIState state);
        void AddState(AIState state);
        void SetState(AIState state);
        void ResetState();

        virtual void ProcessAI()    = 0;
        virtual void OnPatrolling() = 0;
        virtual void OnSearching()  = 0;
        virtual void OnAttacking()  = 0;
        
        game::CLevel*           mp_Level;
        const obj::CPlayer&     m_Player;

        ai::CPathfinder         m_Pathfinder;

        math::CVector2          m_PlayerLastSeen;
        math::CVector2          m_Position;
        math::CRay2             m_LineOfSight;

        obj::CGameObject*       mp_DestinationTile;

        float m_axis_to_path, m_axis_to_player;
        int m_id;

    public:
        CEnemy(game::CLevel* pCurrentLevel,
            const obj::CPlayer& Player);

        virtual ~CEnemy();

        virtual bool Init(game::CSettings& Settings) = 0;

        /**
         * Spawns an enemy.
         * @param math::CVector2& Position to spawn at
         **/
        virtual void Spawn(const math::CVector2& Position) = 0;

        /**
         * Adjusts the entity by a certain rate. Since there is no default entity
         * for the enemy, it should be over-written by the inheriting class.
         * 
         * @param math::CVector2& The rate to adjust at
         * @see game::CMap::GetPanRate()
         **/
        virtual void Adjust(const math::CVector2& Rate) = 0;

        /**
         * Updates the enemy.
         *  This method is left purely virtual because there are many
         *  different types of enemies in @a Collapse, and each one will
         *  have a personal Update() method. Most of the time, rendering
         *  of the entity and AI processing are done here.
         *
         * @return The current state of the enemy.
         **/
        virtual int Update() = 0;

        void SetDestination(const math::CVector2& Position);

        /**
         * Retrieves the current enemy position.
         *  It may seem strange that this be left purely virtual, but for
         *  inheriting classes such as ai::CEnemyTank, there is more than one
         *  entity being handled (the tower and the tank base), so it is
         *  impossible to determine from the base class which entity's
         *  position should be returned.
         *
         * @return The current position.
         **/
        virtual const math::CVector2& GetPosition() const = 0;

        /**
         * Retrieves the main (usually largest) entity of the enemy.
         *  Similarly to GetPosition(), this is virtual due
         *  to the large variation in primary entities from enemy to enemy.
         *
         * @return A pointer to the main entity.
         * @see ai::CEnemy::GetPosition()
         **/
        virtual const obj::CGameObject* GetMainEntity() const = 0;

        AITactic GetTactic();

        int GetID();

        /// Contains all of the currently created enemies.
        static std::list<CEnemy*> p_allEnemies;
    };

    /**
     * For easy creation of a list of enemies, though this shouldn't
     * be necessary with the static member inside of CEnemy.
     *
     * @see ai::CEnemy::p_allEnemies;
     **/
    typedef std::list<CEnemy*> CEnemies;
}

#endif // BASE_ENEMY_H
/// @}