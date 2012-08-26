/**
 * @file
 *  Definitions for the Enemy class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 */

#include "World/AI/BaseEnemy.h"

using ai::AI_Enemy;
using namespace ai;

// Declare the list in AI_Enemy
std::list<AI_Enemy*> AI_Enemy::p_allEnemies;

#ifdef _DEBUG
void ai::PrintState(int state)
{
    printf("\n\n==========================\ne_NONE: %s\ne_PATHFINDING: %s\ne_DONE: %s\ne_FIRING PRIMARY: %s\ne_FIRING_SECONDARY: %s\ne_RELOADING_PRIMARY: %s\ne_RELOADING_SECONDARY: %s\n",
        state & e_NONE ? "true" : "false", state & e_PATHFINDING ? "true" : "false", state & e_DONE ? "true" : "false", state & e_FIRING_PRIMARY ? "true" : "false", 
        state & e_FIRING_SECONDARY ? "true" : "false", state & e_RELOADING_PRIMARY ? "true" : "false", state & e_RELOADING_SECONDARY ? "true" : "false");
}
#endif // _DEBUG

AI_Enemy::AI_Enemy(const game::CL_TerrainMap& Terrain_Map,
    const game::CL_CollisionMap& Collision_Map,
    const game::CL_ObjectiveMap& AI_Map,
    const obj::Obj_Player& Player) : m_TerrainMap(Terrain_Map),
    m_CollisionMap(Collision_Map), m_ObjectiveMap(AI_Map),
    m_Player(Player),
    m_Pathfinder(m_TerrainMap, m_CollisionMap),
    mp_DestinationTile(NULL),
    m_LineOfSight(0, 0, 0, 0),  // Should be fixed in inheriting class
    m_tactic(e_PATROLLING),
    m_state(e_NONE),
    m_axis_to_player(0.0f),
    m_axis_to_path(0.0f)
{
    m_id = p_allEnemies.size();
    p_allEnemies.push_back(this);
}

AI_Enemy::~AI_Enemy()
{
    for(AI_Enemies::iterator i = p_allEnemies.begin();
        i != p_allEnemies.end(); /* no third */)
    {
        if((*i) == this)
            i = p_allEnemies.erase(i);
        else
            ++i;
    }
}

void AI_Enemy::SetDestination(const math::ML_Vector2& Position)
{
    // Find the necessary tiles from the map.
    asset::GL_Entity* p_CurrentTile = m_TerrainMap.FindTile(Position);
    asset::GL_Entity* p_Destination = m_TerrainMap.FindTile(this->GetPosition());

#ifdef _DEBUG
    assert(p_CurrentTile && p_Destination);
#else
    if(p_CurrentTile == NULL || p_Destination == NULL)
        this->AddState(e_NO_PATH);
#endif // _DEBUG

    // A* for the path.
    /// @todo If there's no available path, find another point of interest.
    if(!m_Pathfinder.FindPath(p_CurrentTile, p_Destination))
    {
#ifdef _DEBUG
        printf("[DEBUG] No path found.\n");
#endif // _DEBUG

        this->AddState(e_NO_PATH);
    }

    else
    {
#ifdef _DEBUG
        printf("[DEBUG] Next tile called due to SetDestination()\n");
#endif // _DEBUG

        mp_DestinationTile = m_Pathfinder.NextTile();

        // Make sure we haven't reached the end
        if(mp_DestinationTile != NULL)
        {
            // Calculate angle toward destination
            m_axis_to_path = math::deg(atan2(
                this->GetPosition().x - mp_DestinationTile->GetX(), 
                this->GetPosition().y - mp_DestinationTile->GetY()));
        }
        else
        {
            this->AddState(e_DONE);
        }
    }
}

void AI_Enemy::SetTactic(AI_Enemy::AITactic tactic)
{
    m_tactic = tactic;
}

void AI_Enemy::SetState(AIState state)
{
    m_state = state;
}

void AI_Enemy::RemoveState(AIState state)
{
    if(m_state & state)
        m_state &= ~(int)state;
}

void AI_Enemy::AddState(AIState state)
{
    if(!(m_state & state))
    {
        this->RemoveState(e_NONE);
        m_state |= (int)state;
    }
}

void AI_Enemy::ResetState()
{
    m_state = e_NONE;
}

AI_Enemy::AITactic AI_Enemy::GetTactic()
{
    return m_tactic;
}

int AI_Enemy::GetID()
{
    return m_id;
}
