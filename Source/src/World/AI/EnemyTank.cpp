/**
 * Implementation of the CEnemyTank class.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 **/

#include "World/AI/EnemyTank.hpp"

using ai::CEnemyTank;
using asset::CAssetManager;

/**
 * Loads the tank entities and initializes the base class.
 *
 * @param game::CLevel* The current level
 * @param obj::CPlayer& Player used in AI tactics
 **/       
CEnemyTank::CEnemyTank(game::CLevel* pCurrentLevel,
    const obj::CPlayer& Player) : 
    CEnemy(pCurrentLevel, Player) {}

bool CEnemyTank::Init(game::CSettings& Settings)
{
    m_Tank.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        Settings.GetValueAt("Enemy1IMG1").c_str()));

    m_Tower.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        Settings.GetValueAt("Enemy1IMG2").c_str()));

    m_Weapon1.Init("Data/Data Files/hrl.cwp");
    m_Weapon2.Init("Data/Data Files/lpmg.cwp");
    m_Weapon1.SetClipCount(5);
    m_Weapon2.SetClipCount(10);

    return true;
}

void CEnemyTank::Spawn(const math::CVector2& Pos)
{
    // Move the tank to the specified location.
    m_Tank.Move(Pos);
    m_Tower.Move(Pos);
    m_Position = Pos;

    // Don't actually rotate the tower, but set up the barrel position.
    this->RotateTower(0.0f);

    // Set up initial tactics.
    this->ResetState();
    this->SetTactic(e_PATROLLING);
    this->AddState(e_PATHFINDING);
}

void CEnemyTank::Adjust(const math::CVector2& Rate)
{
    CTank::Adjust(Rate);
}

/**
 * Updates the enemy.
 *  All rendering of tank and tower entities is done here, as well
 *  as AI processing and, in case of a debug build, paths and
 *  states are rendered or printed.
 *
 * @return The current enemy state.
 **/       
int CEnemyTank::Update()
{
    // Remove firing states, because without this, the enemy fires
    // continuously regardless of reloading status.
    /// @todo Do something like ResetState() to make it look nicer.
    this->RemoveState(e_FIRING_PRIMARY);
    this->RemoveState(e_FIRING_SECONDARY);

    // Process AI based on player location and other factors.
    this->ProcessAI();
    this->UpdateLOS();

    // Show the A* path (debugging only)
#ifdef _DEBUG
    m_Pathfinder.ShowPath();

    // Print the current state and tactic (debugging only)
    //PrintState(m_state);
    //printf("[DEBUG] AI Tactic: %s\n", m_tactic == e_PATROLLING ? 
    //    "patrol" : m_tactic == e_ATTACKING ? "attack" : "search");
#endif // _DEBUG

    // Update tank and weapons.
    CTank::Update();

    return m_state;
}

/// Processes AI actions based on tactics.
void CEnemyTank::ProcessAI()
{
    switch(m_tactic)
    {
    case e_PATROLLING:
        this->OnPatrolling();
        break;
        
    case e_SEARCHING:
        this->OnSearching();
        break;

    case e_ATTACKING:
        this->OnAttacking();
        break;

    default:
#ifdef _DEBUG
        printf("Invalid AI tactic.\n");
#endif // _DEBUG
        break;
    }

    if(m_state & e_PATHFINDING)
        this->FollowPath();

    // Update the current line-of-sight to reflect current location.
    this->UpdateLOS();
}

/**
 * Patrols the current path, scanning the area from time to time
 * for the player. If the player is spotted, the location is
 * calculated and the tactic is set to an aggressive attacking
 * method.
 * 
 * @param obj::CPlayer& The player
 * 
 * @see ai::CEnemyPathfinder::FindPath()
 * @see ai::CEnemyTank::FollowPath()
 **/        
void CEnemyTank::OnPatrolling()
{
    // Whelp, this shouldn't be happening...
    if(m_tactic != e_PATROLLING)
        return;

    // If we're done with our path, just go back to where we started from.
    if(m_state & e_DONE)
    {
        this->RemoveState(e_DONE);
        obj::CEntity* p_Dest = mp_Level->GetObjectiveMap().GetNearestPOI(
            m_Tank.GetPosition());
        this->SetDestination(p_Dest->GetPosition());
        if(mp_DestinationTile == NULL)
            this->AddState(e_DONE);
        else
            this->AddState(e_PATHFINDING);
    }

    // Scan the area for hostiles.
    // This is done by repeatedly rotating the tank tower 90 degrees in
    // both directions from the tank front-center position, with a slight
    // pause upon reaching the end-points.

    // This is the direction in which to rotate the tower.
    static float rotation_speed = 3.0f;

    // Check that the rotation angle is within the closed interval
    // [Tower Rotation - 90, Tower Rotation + 90]
    if((m_Tower.GetRotationAngle() >= 90.0f + m_Tank.GetRotationAngle() &&
        rotation_speed == 3.0f) ||

        (m_Tower.GetRotationAngle() <= m_Tank.GetRotationAngle() - 90.0f &&
        rotation_speed == -3.0f))
    {
        rotation_speed = -rotation_speed;
    }
    else
    {
        this->RotateTower(rotation_speed);
    }

    // Check if player is in line-of-sight. If he is, calculate the angle
    // toward him (for the pathfinding), change the tactic to
    // the hostile e_ATTACKING tactic, reset the state, add pathfinding, 
    // and set the new path as the player location.
    if(m_LineOfSight.CheckCollision(m_Player.GetCollisionBox()))
    {
#ifdef _DEBUG
        printf("[DEBUG] Player spotted!\n");
#endif // _DEBUG

        // Calculate axis toward player
        m_axis_to_player = math::deg(
            atan2(m_Tank.GetX() - m_Player.GetPosition().x, 
            m_Tank.GetY() - m_Player.GetPosition().y));

        // Attack the newly found enemy!
        this->SetTactic(e_ATTACKING);

        // Find a path to the enemy
        this->SetDestination(m_Player.GetPosition());

        // Just do pathfinding now
        this->ResetState();
        this->AddState(e_PATHFINDING);
    }
}

/**
 * Searches for the player.
 *  This method is called after the player is lost from the current
 *  line-of-sight. It uses a similar scanning mechanism to the one 
 *  found in OnPatrolling(), and moves towards the player's last
 *  known position.
 *
 * @pre m_tactic must be set to e_SEARCHING
 * 
 * @see ai::CEnemyPathfinder::FindPath()
 * @see ai::CEnemyTank::FollowPath()
 * @see ai::CEnemyTank::OnPatrolling()
 * @see ai::CEnemy::AITactic
 **/        
void CEnemyTank::OnAttacking()
{
    // You shouldn't be here...
    if(!(m_tactic & e_ATTACKING))
        return;

    // Have we lost our target?
    if(!m_LineOfSight.CheckCollision(m_Player.GetCollisionBox()))
    {
#ifdef _DEBUG
        printf("[DEBUG] Player lost!\n");
#endif // _DEBUG

        // Find a path to the player's last known position.
        m_PlayerLastSeen = m_Player.GetPosition();
        this->SetDestination(m_PlayerLastSeen);

        // Commence search
        this->ResetState();
        this->AddState(e_PATHFINDING);
        this->SetTactic(e_SEARCHING);
        return;
    }

    // We have reached our previous destination, now move towards the player
    // again by locating his position.
    /// @todo Make attacking-pathfinding more effective.
    if(m_state & e_DONE)
    {
#ifdef _DEBUG
        printf("[DEBUG] Attacking path done!\n");
#endif // _DEBUG

        // Make sure player isn't in the same spot. If he is, nothing should
        // be done, but if he's not, search for him the same way it's done
        // in CEnemyTank::OnPatrolling().
        if(m_PlayerLastSeen != m_Player.GetPosition())
        {

        }
    }

    // Try and fire main gun.
    if(m_Weapon1.Fire())
        this->AddState(e_FIRING_PRIMARY);

    // If unable, try and fire the secondary.
    else
    {
        this->AddState(e_RELOADING_PRIMARY);
        if(m_Weapon2.Fire())
            this->AddState(e_FIRING_SECONDARY);
        else
            this->AddState(e_RELOADING_SECONDARY);
    }
}

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
 * @see ai::CEnemyPathfinder::FindPath()
 * @see ai::CEnemyTank::FollowPath()
 * @see ai::CEnemyTank::OnSearching()
 * @see ai::CEnemy::AITactic
 **/        
void CEnemyTank::OnSearching()
{
    // If we've reached the last known player position,
    // go back to patrolling.
    /// @todo Find the nearest point-of-interest rather than just choosing
    /// a random spot on the map.
    if(m_state == e_DONE)
    {
#ifdef _DEBUG
        printf("[DEBUG] Search path done!\n");
#endif // _DEBUG
        this->RemoveState(e_DONE);
        obj::CEntity* p_Dest = mp_Level->GetObjectiveMap().GetNearestPOI(
            m_Tank.GetPosition());
        this->SetDestination(p_Dest->GetPosition());
        if(mp_DestinationTile == NULL)
            this->AddState(e_DONE);
        else
            this->AddState(e_PATHFINDING);

        this->SetTactic(e_PATROLLING);
        this->ResetState();
    }

    // Just like in OnPatrolling(), scan the area for hostiles.
    // But rather than rotating just 90 degrees from the tank front, 
    // rotate 90 degrees from the axis at which the player was spotted.

    // This is the direction in which to rotate the tower.
    static float rotation_speed = 3.0f;

    // Calculate the angle towards the player.
    float to_player = math::deg(atan2(
        this->GetPosition().x - m_Player.GetPosition().x,
        this->GetPosition().y - m_Player.GetPosition().y)) + 180;

    // Check that the rotation angle is within the closed interval
    // [Last spotted - 90, Last spotted + 90]
    if((m_Tower.GetRotationAngle() >= 90.0f + to_player &&
        rotation_speed == 3.0f) ||
        (m_Tower.GetRotationAngle() <= to_player - 90.0f &&
        rotation_speed == -3.0f))
    {
        rotation_speed = -rotation_speed;
    }

    this->RotateTower(rotation_speed);

    // Similarly to patrolling, check if player is in line-of-sight.
    // If he is, calculate the angle toward him (for the pathfinding),
    // change the tactic to the hostile e_ATTACKING tactic, reset the
    // state, add pathfinding, and set the new path as the player location.
    if(m_LineOfSight.CheckCollision(m_Player.GetCollisionBox()))
    {
#ifdef _DEBUG
        printf("[DEBUG] Player spotted!\n");
#endif // _DEBUG

        // Calculate axis toward player
        m_axis_to_player = math::deg(
            atan2(m_Tank.GetX() - m_Player.GetPosition().x, 
            m_Tank.GetY() - m_Player.GetPosition().y));

        // Attack the newly found enemy!
        this->SetTactic(e_ATTACKING);

        // Find a path to the enemy
        this->SetDestination(m_Player.GetPosition());

        // Just do pathfinding now
        this->ResetState();
    }

    this->AddState(e_PATHFINDING);
}

/**
 * Follows the current path.
 *  If we aren't in pathfinding mode, or there is no path to
 *  follow, or the path is done, this method simply does nothing.
 *  Otherwise, it moves towards the current destination tile in the
 *  path by calculating the angle towards it and moving in that
 *  direction.
 *
 * @see ai::CEnemyPathfinder
 * @see ai::AIState
 * @see ai::CEnemy::m_state
 * 
 * @todo Add actions for done and no path states.
 **/
void CEnemyTank::FollowPath()
{
    if(!(m_state & e_PATHFINDING))
        return;

    else if(m_state & e_NO_PATH)
    {
        /// @todo Find another point-of-interest from the AI map.
        this->SetDestination(mp_Level->GetObjectiveMap().
            GetNearestPOI(this->GetPosition())->GetPosition());
        this->ResetState();
        this->SetState(e_PATHFINDING);
        printf("[DEBUG] No path found!\n");
        return;
    }

    else if(m_state & e_DONE)
    {
        printf("[DEBUG] Path following complete.\n");
        return;
    }

    else if(mp_DestinationTile == NULL)
    {
        printf("[DEBUG] mp_DestinationTile is NULL!\n");
        return;
    }
    
    // If we have reached our current tile destination.
    else if(mp_DestinationTile->CheckCollision(
        math::CRect(m_Tank.GetX() + 16, m_Tank.GetY(), 32, 8)))
    {
#ifdef _DEBUG
        printf("[DEBUG] Next tile called due to collision.\n");
#endif // _DEBUG

        // Find the next one
        mp_DestinationTile = m_Pathfinder.NextTile();

        if(mp_DestinationTile == NULL)
        {
            this->RemoveState(e_PATHFINDING);
            this->AddState(e_DONE);
            return;
        }
        else
        {
            // Calculate angle toward destination
            m_axis_to_path = math::deg(
                atan2(m_Tank.GetY() - mp_DestinationTile->GetY(),
                      m_Tank.GetX() - mp_DestinationTile->GetX()));
            m_axis_to_path += 90.0f;

            // Keep the angle in the interval [0, 360]
            if(m_axis_to_path < 0.0f)
                m_axis_to_path = 180.0f + 180.0f + m_axis_to_path;
            while(m_axis_to_path >= 360.0f)
                m_axis_to_path -= 360.0f;

#ifdef _DEBUG
            printf("[DEBUG] Axis toward next tile: %0.1f\n", m_axis_to_path);
#endif // _DEBUG
        }
    }

    // Move toward the destination
    else
    {
        // Our angle
        float current_angle = m_Tank.GetRotationAngle();

        // Adjust so it's in a 3 degree range
        while((int)m_axis_to_path % 3 != 0) m_axis_to_path--;

        // If we're not axis-aligned
        if(!(current_angle + 1.0f >= m_axis_to_path &&
            current_angle - 1.0f <= m_axis_to_path))
        {
            if(current_angle < m_axis_to_path)
                this->Turn(3.0f);
            else
                this->Turn(-3.0f);
        }
        else
        {
            this->Drive(-3.0f);
            if(mp_Level->GetCollisionMap().FindTile(this->GetPosition()) != NULL)
                this->Drive(3.0f);
        }
    }
}

/**
 * Updates the tank's line-of-sight vector.
 *   Since the tank is constantly moving, it needs to constantly be 
 *   updating it's line-of-sight to find the player properly.
 **/        
void CEnemyTank::UpdateLOS()
{
    // Refreshes barrel pos.
    this->RotateTower(0);

    // Locate tank center
    math::CVector2 Tank_Center(this->GetPosition().x + 32, this->GetPosition().y + 32);

    // Reset LOS to center-top, 8 tiles out.
    m_LineOfSight.Start = this->GetBarrelPosition();
    m_LineOfSight.End.Move(this->GetPosition().x - 2 + 32, this->GetPosition().y + 256);

    // Rotate around center
    m_LineOfSight = m_LineOfSight - Tank_Center;
    m_LineOfSight.End.Rotate(math::rad(m_Tower.GetRotationAngle()));
    m_LineOfSight = m_LineOfSight + Tank_Center;

    // Cut off at walls
    const std::vector<obj::CGameObject*>& p_allTiles = 
        mp_Level->GetCollisionMap().GetTiles();
    math::CVector2 Intersection;
    for(size_t i = 0; i < p_allTiles.size(); ++i)
    {
        if(m_LineOfSight.CheckCollision(p_allTiles[i]->GetCollisionBox(), &Intersection))
            m_LineOfSight.End = Intersection;
    }
}

/**
 * Retrieves the tank's position.
 *  Defining the virtual method found in ai::CEnemy, this returns
 *  the (x, y) vector of the tank, rather than the tower or
 *  anything else.
 *
 * @return The tank's coordinates.
 **/        
const math::CVector2& CEnemyTank::GetPosition() const
{
    return m_Tank.GetPosition();
}

/**
 * Retrieves the main (tank body) entity.
 * @return A pointer to the tank body entity.
 **/        
const obj::CGameObject* ai::CEnemyTank::GetMainEntity() const
{
    return &m_Tank;
}