/**
 * @file
 *  Declarations for the Enemy class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup AI
 */
/// @{

#ifndef BASE_ENEMY_H
#define BASE_ENEMY_H

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

#include <list>

#include "World/Levels/AIMap.h"
#include "World/Levels/CollisionMap.h"
#include "World/Levels/TerrainMap.h"

#include "World/Objects/Player.h"
#include "World/AI/Pathfinder.h"

namespace ai
{
    /**
     * Various phases of AI action which can be combined together as flags.
     */
    enum AIState
    {
        e_NONE                  = (1 << 0),     /// Doing nothing
        e_PATHFINDING           = (1 << 1),     /// Following the current path
        e_DONE                  = (1 << 2),     /// Finished following the current path
        e_FIRING_PRIMARY        = (1 << 3),     /// These next four are self-explanatory
        e_FIRING_SECONDARY      = (1 << 4),
        e_RELOADING_PRIMARY     = (1 << 5),
        e_RELOADING_SECONDARY   = (1 << 6),
        e_NO_PATH               = (1 << 7)      /// @see ai::AI_Pathfinder::FindPath()
    };

    void PrintState(int state);

    /**
     * Abstract base class for all AI-controlled enemies.
     */
    class AI_Enemy
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
        
        const game::CL_TerrainMap& m_TerrainMap;
        const game::CL_CollisionMap& m_CollisionMap;
        const game::CL_ObjectiveMap& m_ObjectiveMap;
        const obj::Obj_Player& m_Player;

        ai::AI_Pathfinder m_Pathfinder;

        math::ML_Vector2 m_PlayerLastSeen;
        math::ML_Vector2 m_Position;
        math::ML_Ray2    m_LineOfSight;

        asset::GL_Entity* mp_DestinationTile;

        float m_axis_to_path, m_axis_to_player;
        int m_id;

    public:
        /**
         * Constructor for the AI_Enemy class.
         *  Since this is a base class, nothing is done in the constructor,
         *  except for initializing member variables. Also, the class will
         *  add itself to the list of AI_Enemy's.
         *
         * @param game::CL_TerrainMap&      Terrain map used for pathfinding
         * @param game::CL_CollisionMap&    Collision map used for pathfinding
         * @param game::CL_ObjectiveMap&           AI map used to determine points of interest
         * @param obj::Obj_Player&          Player used in AI tactics
         * 
         * @see ai::AI_Enemy::p_allEnemies
         */
        AI_Enemy(const game::CL_TerrainMap& Terrain_Map,
            const game::CL_CollisionMap& Collision_Map,
            const game::CL_ObjectiveMap& Obj_Map,
            const obj::Obj_Player& Player);

        /**
         * Cleans up internal data.
         *  Here, the destructor iterates through the list of all existing
         *  enemies and finds itself. Upon finding itself, it will erase
         *  itself from the list.
         */
        virtual ~AI_Enemy();

        /**
         * Spawns an enemy.
         *
         * @param math::ML_Vector2& Position to spawn at
         */
         virtual void Spawn(const math::ML_Vector2& Position) = 0;

        /**
         * Adjusts the entity by a certain rate. Since there is no default entity
         * for the enemy, it should be over-written by the inheriting class.
         * 
         * @param math::ML_Vector2& The rate to adjust at
         * 
         * @see game::CL_Map::GetPanRate()
         */
        virtual void Adjust(const math::ML_Vector2& Rate) = 0;

        /**
         * Updates the enemy.
         *  This method is left purely virtual because there are many
         *  different types of enemies in @a Collapse, and each one will
         *  have a personal Update() method. Most of the time, rendering
         *  of the entity and AI processing are done here.
         *
         * @return The current state of the enemy.
         */
        virtual int Update() = 0;

        /**
         * Gives the AI a destination to move towards.
         * Uses the A* algorithm to find the path, if it exists.
         *
         * @param math::ML_Vector2& Destination
         */
        void SetDestination(const math::ML_Vector2& Position);

        /**
         * Retrieves the current enemy position.
         *  It may seem strange that this be left purely virtual, but for
         *  inheriting classes such as ai::AI_Tank, there is more than one
         *  entity being handled (the tower and the tank base), so it is
         *  impossible to determine from the base class which entity's
         *  position should be returned.
         *
         * @return The current position.
         */
        virtual const math::ML_Vector2& GetPosition() const = 0;

        /**
         * Retrieves the main (usually largest) entity of the enemy.
         *  Similarly to GetPosition(), this is virtual due
         *  to the large variation in primary entities from enemy to enemy.
         *
         * @return A pointer to the main entity.
         * 
         * @see ai::AI_Enemy::GetPosition()
         */
        virtual const asset::GL_Entity* const GetMainEntity() const = 0;

        /**
         * Retrieves the current tactic.
         *
         * @see AI_Enemy::Update()
         * @return The tactic.
         */
        AITactic GetTactic();

        /**
         * Retrieves the unique ID for this enemy.
         *  The ID is generated in the constructor based on the current
         *  total enemy count.
         *
         * @see AI_Enemy::AI_Enemy()
         * @see game::CL_World::SpawnEnemy()
         * 
         * @return Unique ID.
         */
        int GetID();

        /// Contains all of the currently created enemies.
        static std::list<AI_Enemy*> p_allEnemies;
    };

    /**
     * For easy creation of a list of enemies, though this shouldn't
     * be necessary with the static member inside of AI_Enemy.
     *
     * @see ai::AI_Enemy::p_allEnemies;
     */
    typedef std::list<AI_Enemy*> AI_Enemies;
}

#endif // BASE_ENEMY_H
/// @}