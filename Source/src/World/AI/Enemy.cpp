/**
 * @file
 *  Definitions for the CEnemy class.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 **/

#include "World/AI/Enemy.hpp"

using ai::CEnemy;
using namespace ai;

// Declare the list in CEnemy
std::list<CEnemy*> CEnemy::p_allEnemies;

/**
 * Constructor for the CEnemy class.
 *  Since this is a base class, nothing is done in the constructor,
 *  except for initializing member variables. Also, the class will
 *  add itself to the list of CEnemy's.
 *
 * @param game::CLevel*     The current level
 * @param obj::CPlayer&  Player used in AI tactics
 * 
 * @see ai::CEnemy::p_allEnemies
 **/        
CEnemy::CEnemy(game::CLevel* pCurrentLevel,
    const obj::CPlayer& Player) : mp_Level(pCurrentLevel),
    m_Player(Player),
    m_Pathfinder(mp_Level),
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

/**
 * Cleans up internal data.
 *  Here, the destructor iterates through the list of all existing
 *  enemies and finds itself. Upon finding itself, it will erase
 *  itself from the list.
 **/        
CEnemy::~CEnemy()
{
    for(CEnemies::iterator i = p_allEnemies.begin();
        i != p_allEnemies.end(); /* no third **/)
    {
        if((*i) == this)
            i = p_allEnemies.erase(i);
        else
            ++i;
    }
}

/**
 * Gives the AI a destination to move towards.
 *  Uses the A* algorithm to find the path, if it exists.
 *
 * @param math::CVector2& Destination
 **/        
void CEnemy::SetDestination(const math::CVector2& Position)
{
    // Find the necessary tiles from the map.
    obj::CGameObject* p_CurrentTile = mp_Level->GetTerrainMap().FindTile(Position);
    obj::CGameObject* p_Destination = mp_Level->GetTerrainMap().FindTile(this->GetPosition());

#ifdef _DEBUG
    assert(p_CurrentTile && p_Destination);
#else
    if(p_CurrentTile == NULL || p_Destination == NULL)
    {
        this->AddState(e_NO_PATH);
        return;
    }
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
                this->GetPosition().y - mp_DestinationTile->GetY(),
                this->GetPosition().x - mp_DestinationTile->GetX()));
            m_axis_to_path += 90.0f;
        }
        else
        {
            this->AddState(e_DONE);
        }
    }
}

void CEnemy::SetTactic(CEnemy::AITactic tactic)
{
    m_tactic = tactic;
}

void CEnemy::SetState(AIState state)
{
    m_state = state;
}

void CEnemy::RemoveState(AIState state)
{
    if(m_state & state)
        m_state &= ~(int)state;
}

void CEnemy::AddState(AIState state)
{
    if(!(m_state & state))
    {
        this->RemoveState(e_NONE);
        m_state |= (int)state;
    }
}

void CEnemy::ResetState()
{
    m_state = e_NONE;
}

CEnemy::AITactic CEnemy::GetTactic()
{
    return m_tactic;
}

/**
 * Retrieves the unique ID for this enemy.
 *  The ID is generated in the constructor based on the current
 *  total enemy count.
 *
 * @see CEnemy::CEnemy()
 * @see game::CWorld::SpawnEnemy()
 * @return Unique ID.
 **/       
int CEnemy::GetID()
{
    return m_id;
}
