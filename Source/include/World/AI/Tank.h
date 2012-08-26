/**
 * @file
 *  Declarations for the AI_Tank class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup AI
 */
/// @{

#include "SystemEvents.h"

#include "Math/Math.h"

#include "Assets/TextureAssetManager.h"

#include "World/Objects/BaseTank.h"
#include "World/Objects/Player.h"
#include "World/AI/BaseEnemy.h"

namespace ai
{
    /**
     * An enemy tank object, to be controlled by a global AI master.
     */
    class AI_Tank : public obj::Obj_Tank, public ai::AI_Enemy
    {
    public:
        /**
         * Loads the tank entities and initializes the base class.
         *
         * @param game::CL_TerrainMap&    Terrain map used for pathfinding
         * @param game::CL_CollisionMap&  Collision map used for pathfinding
         * @param game::CL_ObjectiveMap&         AI map used to determine points of interest
         * @param obj::Obj_Player&          Player used in AI tactics
         */
        AI_Tank(const game::CL_TerrainMap& Terrain_Map,
            const game::CL_CollisionMap& Collision_Map,
            const game::CL_ObjectiveMap& AI_Map,
            const obj::Obj_Player& Player);

        void Spawn(const math::ML_Vector2& Position);
        void Adjust(const math::ML_Vector2& Rate);

        /**
         * Updates the enemy.
         *  All rendering of tank and tower entities is done here, as well
         *  as AI processing and, in case of a debug build, paths and
         *  states are rendered or printed.
         *
         * @return The current enemy state.
         */
        int Update();

    private:

        /**
         * Processes AI actions based on tactics.
         */
        void ProcessAI();
        
        /**
         * Patrols the current path, scanning the area from time to time
         * for the player. If the player is spotted, the location is
         * calculated and the tactic is set to an aggressive attacking
         * method.
         * 
         * @param obj::Obj_Player& The player
         * 
         * @see ai::AI_Pathfinder::FindPath()
         * @see ai::AI_Tank::FollowPath()
         */
        void OnPatrolling();
        
        /**
         * Searches for the player.
         *  This method is called after the player is lost from the current
         *  line-of-sight. It uses a similar scanning mechanism to the one 
         *  found in OnPatrolling(), and moves towards the player's last
         *  known position.
         *
         * @pre m_tactic must be set to e_SEARCHING
         * 
         * @see ai::AI_Pathfinder::FindPath()
         * @see ai::AI_Tank::FollowPath()
         * @see ai::AI_Tank::OnPatrolling()
         * @see ai::AI_Enemy::AITactic
         */
        void OnSearching();

        /**
         * Attacks the player.
         *  Here, the enemy will fire all available weaponry at the player
         *  continuously until it either runs out of ammo, or loses sight
         *  of the player. Upon losing sight of the player, the last known
         *  player position is recorded and the tactic is changed to
         *  e_SEARCHING. Also, throughout the attacking stage, the enemy
         *  will move towards the player.
         *  
         * @pre m_tactic must be set to e_ATTACKING
         * 
         * @see ai::AI_Pathfinder::FindPath()
         * @see ai::AI_Tank::FollowPath()
         * @see ai::AI_Tank::OnSearching()
         * @see ai::AI_Enemy::AITactic
         */
        void OnAttacking();

        /**
         * Follows the current path.
         *  If we aren't in pathfinding mode, or there is no path to
         *  follow, or the path is done, this method simply does nothing.
         *  Otherwise, it moves towards the current destination tile in the
         *  path by calculating the angle towards it and moving in that
         *  direction.
         *
         * @see ai::AI_Pathfinder
         * @see ai::AIState
         * @see ai::AI_Enemy::m_state
         */
        void FollowPath();

        /**
         * Updates the tank's line-of-sight vector.
         *  Since the tank is constantly moving, it needs to constantly be 
         *  updating it's line-of-sight to find the player properly.
         */
        void UpdateLOS();

        /**
         * Retrieves the tank's position.
         *  Defining the virtual method found in ai::AI_Enemy, this returns
         *  the (x, y) vector of the tank, rather than the tower or
         *  anything else.
         *
         * @return The tank's coordinates.
         */
        const math::ML_Vector2& GetPosition() const;

        /**
         * Retrieves the main (tank body) entity.
         *
         * @return A pointer to the tank body entity.
         */
        const asset::GL_Entity* const GetMainEntity() const;
    };
}

/// @}