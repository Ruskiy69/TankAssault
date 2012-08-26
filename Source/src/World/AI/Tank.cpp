/**
 * Implementation of the AI_Tank class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 */

#include "World/AI/Tank.h"

using ai::AI_Tank;
using asset::g_TextureAssets;

AI_Tank::AI_Tank(const game::CL_TerrainMap& Terrain_Map,
    const game::CL_CollisionMap& Collision_Map,
    const game::CL_ObjectiveMap& AI_Map,
    const obj::Obj_Player& Player) : 
    AI_Enemy(Terrain_Map, Collision_Map, AI_Map, Player)
{
    mp_Tank = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
        "Data/Images/Enemy_Tank.png"));

    mp_Tower = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
        "Data/Images/Enemy_Tank_Barrel1.png"));
}

void AI_Tank::Spawn(const math::ML_Vector2& Pos)
{
    // Move the tank to the specified location.
    mp_Tank->Move(Pos);
    mp_Tower->Move(Pos);
    m_Position = Pos;

    // Don't actually rotate the tower, but set up the barrel position.
    this->RotateTower(0.0f);

    // Set up initial tactics.
    this->ResetState();
    this->SetTactic(e_PATROLLING);
    this->AddState(e_PATHFINDING);
}

void AI_Tank::Adjust(const math::ML_Vector2& Rate)
{
    Obj_Tank::Adjust(Rate);
}

int AI_Tank::Update()
{
    // Remove firing states, because without this, the enemy fires
    // continuously regardless of reloading status.
    /// @todo Do something like ResetState() to make it look nicer.
    this->RemoveState(e_FIRING_PRIMARY);
    this->RemoveState(e_FIRING_SECONDARY);

    // Process AI based on player location and other factors.
    this->ProcessAI();

    // Show the A* path (debugging only)
#ifdef _DEBUG
    m_Pathfinder.ShowPath();

    // Print the current state and tactic (debugging only)
    //PrintState(m_state);
    printf("[DEBUG] AI Tactic: %s\n", m_tactic == e_PATROLLING ? 
        "patrol" : m_tactic == e_ATTACKING ? "attack" : "search");
#endif // _DEBUG

    // Render the tank
    mp_Tank->Update();
    mp_Tower->Update();

    // Reduce shot delays
    if(m_secondary_shot_delay > 0)
        m_secondary_shot_delay--;
    if(m_primary_shot_delay > 0)
        m_primary_shot_delay--;

    // Render the line of sight for easy debugging.
#ifdef _DEBUG
    SDL_Surface* p_Pixel = gfx::create_surface_alpha(5, 5, gfx::YELLOW);
    SDL_Surface* p_Pixel2 = gfx::create_surface_alpha(5, 5, gfx::GREEN);

    std::vector<asset::GL_Entity*> p_allPointEntities;

    asset::GL_Entity* p_Tmp = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromSurface(p_Pixel));
    p_Tmp->Move(m_LineOfSight.Start);
    p_allPointEntities.push_back(p_Tmp);
    p_Tmp = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromSurface(p_Pixel2));
    p_Tmp->Move(m_LineOfSight.End);
    p_allPointEntities.push_back(p_Tmp);
    
    SDL_FreeSurface(p_Pixel);
    SDL_FreeSurface(p_Pixel2);

    /// @bug Memory leak in asset::GL_TextureManager
    for(size_t i = 0; i < p_allPointEntities.size(); i++)
    {
        p_allPointEntities[i]->Update();
        //delete p_allPointEntities[i];
    }
    p_allPointEntities.clear();
#endif // _DEBUG

    return m_state;
}

void AI_Tank::ProcessAI()
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

void AI_Tank::OnPatrolling()
{
    // Whelp, this shouldn't be happening...
    if(m_tactic != e_PATROLLING)
        return;

    // If we're done with our path, just go back to where we started from.
    if(m_state & e_DONE)
    {
        this->RemoveState(e_DONE);
        asset::GL_Entity* p_Dest = m_ObjectiveMap.GetNearestPOI(mp_Tank->GetPosition());
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
    if((mp_Tower->GetRotation() >= 90.0f + mp_Tank->GetRotation() &&
        rotation_speed == 3.0f) ||

        (mp_Tower->GetRotation() <= mp_Tank->GetRotation() - 90.0f &&
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
            atan2(mp_Tank->GetX() - m_Player.GetTankPosition().x, 
            mp_Tank->GetY() - m_Player.GetTankPosition().y));

        // Attack the newly found enemy!
        this->SetTactic(e_ATTACKING);

        // Find a path to the enemy
        this->SetDestination(m_Player.GetTankPosition());

        // Just do pathfinding now
        this->ResetState();
        this->AddState(e_PATHFINDING);
    }
}

void AI_Tank::OnAttacking()
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
        m_PlayerLastSeen = m_Player.GetTankPosition();
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
        // in AI_Tank::OnPatrolling().
        if(m_PlayerLastSeen != m_Player.GetTankPosition())
        {

        }
    }

    // Try and fire main gun.
    if(this->FirePrimary())
        this->AddState(e_FIRING_PRIMARY);

    // If unable, try and fire the secondary.
    else
    {
        this->AddState(e_RELOADING_PRIMARY);
        if(this->FireSecondary())
            this->AddState(e_FIRING_SECONDARY);
        else
            this->AddState(e_RELOADING_SECONDARY);
    }
}

void AI_Tank::OnSearching()
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
        asset::GL_Entity* p_Dest = m_ObjectiveMap.GetNearestPOI(mp_Tank->GetPosition());
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
        this->GetTankPosition().x - m_Player.GetTankPosition().x,
        this->GetTankPosition().y - m_Player.GetTankPosition().y)) + 180;

    // Check that the rotation angle is within the closed interval
    // [Last spotted - 90, Last spotted + 90]
    if((mp_Tower->GetRotation() >= 90.0f + to_player &&
        rotation_speed == 3.0f) ||
        (mp_Tower->GetRotation() <= to_player - 90.0f &&
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
            atan2(mp_Tank->GetX() - m_Player.GetTankPosition().x, 
            mp_Tank->GetY() - m_Player.GetTankPosition().y));

        // Attack the newly found enemy!
        this->SetTactic(e_ATTACKING);

        // Find a path to the enemy
        this->SetDestination(m_Player.GetTankPosition());

        // Just do pathfinding now
        this->ResetState();
    }

    this->AddState(e_PATHFINDING);
}

/// @todo Add actions for done and no path states.
void AI_Tank::FollowPath()
{
    if(!(m_state & e_PATHFINDING))
        return;

    else if(m_state & e_NO_PATH)
    {
        /// @todo Find another point-of-interest from the AI map.
        //this->SetDestination(m_ObjectiveMap.FindNearestPOI(this->GetPosition()));
        //this->ResetState();
        //this->SetState(e_PATHFINDING);
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
        math::ML_Rect(mp_Tank->GetX() + 16, mp_Tank->GetY(), 32, 8)))
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
                atan2(mp_Tank->GetX() - mp_DestinationTile->GetX(), 
                mp_Tank->GetY() - mp_DestinationTile->GetY()));

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
        float current_angle = mp_Tank->GetRotation() + 180.0f;

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
            this->Drive(3.0f);
            if(m_CollisionMap.FindTile(this->GetPosition()) != NULL)
                this->Drive(-3.0f);
        }
    }
}

void AI_Tank::UpdateLOS()
{
    // Locate tank center
    math::ML_Vector2 Tank_Center(this->GetPosition().x + 32, this->GetPosition().y + 32);

    // Reset LOS to center-top, 8 tiles out.
    m_LineOfSight.Start = this->GetBarrelPosition();
    m_LineOfSight.End.Move(this->GetPosition().x - 2 + 32, this->GetPosition().y + 256);

    // Rotate around center
    m_LineOfSight = m_LineOfSight - Tank_Center;
    m_LineOfSight.End.Rotate(math::rad(mp_Tower->GetRotation()));
    m_LineOfSight = m_LineOfSight + Tank_Center;

    // Cut off at walls
    const std::vector<asset::GL_Entity*>& p_allTiles = m_CollisionMap.GetTiles();
    math::ML_Vector2 Intersection;
    for(size_t i = 0; i < p_allTiles.size(); ++i)
    {
        if(m_LineOfSight.CheckCollision(p_allTiles[i]->GetCollisionBox(), &Intersection))
            m_LineOfSight.End = Intersection;
    }
}

const math::ML_Vector2& AI_Tank::GetPosition() const
{
    return mp_Tank->GetPosition();
}

const asset::GL_Entity* const ai::AI_Tank::GetMainEntity() const
{
    return mp_Tank;
}