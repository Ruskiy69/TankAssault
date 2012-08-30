/**
 * @file
 *  Implementation of the CL_World class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 */
 
#include "World/World.h"

using game::CL_World;
using game::g_Log;

using asset::g_TextureAssets;
using asset::g_AudioAssets;
using asset::g_FontAssets;

/**
 * Initialize all of the internal components.
 * @param CL_State& The current engine state
 */
CL_World::CL_World(game::CL_GameState& Engine_State) : 
    m_engine_state(Engine_State)
#ifdef _DEBUG
        , m_CollisionMap(true), m_TerrainMap(true),
        m_ObjectiveMap(true)
#endif // _DEBUG
    {
    }

void CL_World::Init()
{
    g_Log.Flush();
    g_Log << "[INFO] Loading first level.\n";

    // Load first level
    if(!m_TerrainMap.Load("Data/Levels/Level1.ctm") ||
        !m_CollisionMap.Load("Data/Levels/Level1.ccm") ||
        !m_ObjectiveMap.Load("Data/Levels/Level1.cam"))
    {
        gk::handle_error("Failed to load level 1!");
    }

    g_Log.Flush();
    g_Log << "[INFO] Loaded first level.\n";

    m_TerrainMap.SetPanRate(3);
    m_CollisionMap.SetPanRate(3);
    m_ObjectiveMap.SetPanRate(3);

    // Load game background
    mp_Background = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
        "Data/Images/Background.png"));

    // Spawn enemies at all available spawns.
    while(this->SpawnEnemy());

    // Set player spawn location.
    m_Player.Init();
    m_Player.SetSpawn(m_ObjectiveMap.GetPlayerSpawn()->GetPosition());
}

CL_World::~CL_World()
{
    g_Log.Flush();
    g_Log << "[DEBUG] Destroying CL_World instance.\n";

    for(obj::Obj_pBullets::iterator i = mp_playerBullets.begin(); 
        i != mp_playerBullets.end(); /* no third */)
    {
        delete (*i);
        i = mp_playerBullets.erase(i);
    }

    mp_playerBullets.clear();

    for(size_t i = 0; i < mp_enemyBullets.size(); i++)
    {
        for(obj::Obj_pBullets::iterator j = mp_enemyBullets[i].begin(); 
            j != mp_enemyBullets[i].end(); /* no third */)
        {
            delete (*j);
            j = mp_enemyBullets[i].erase(j);
        }

        mp_enemyBullets[i].clear();
    }

    for(ai::AI_Enemies::iterator i = ai::AI_Enemy::p_allEnemies.begin(); 
        i != ai::AI_Enemy::p_allEnemies.end(); /* no third */)
    {
        i = ai::AI_Enemy::p_allEnemies.erase(i);
    }

    ai::AI_Enemy::p_allEnemies.clear();
    mp_enemyBullets.clear();
    m_engine_state = game::e_QUIT;
}

/**
 * Handles a specific event given by game::Game_Engine.
 *
 * @param SDL_Event The event
 *
 * @pre The current engine state must be e_GAME.
 * @see Game/Definitions.h
 * @see game::Game_State
 */
void CL_World::HandleEvent(SDL_Event& Evt)
{
    // For player movement
    static float speed = 0, angle = 0;

    // Wrong state?
    if(m_engine_state != game::e_GAME)
        return;

    switch(Evt.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        break;
        
    case SDL_KEYDOWN:
        g_Log.Flush();
        g_Log << "[DEBUG] CL_World received a key-down event.\n";
        g_Log << "[DEBUG] Event ID: " << Evt.key.keysym.sym << ".\n";

        switch(Evt.key.keysym.sym)
        {
        case SDLK_w:
        case SDLK_UP:
            speed = m_Player.DEFAULT_TANK_SPEED;
            break;

        case SDLK_s:
        case SDLK_DOWN:
            speed = -m_Player.DEFAULT_TANK_SPEED;
            break;

        case SDLK_d:
        case SDLK_RIGHT:
            angle = -m_Player.DEFAULT_TANK_SPEED;
            break;

        case SDLK_a:
        case SDLK_LEFT:
            angle = m_Player.DEFAULT_TANK_SPEED;
            break;

#ifdef _DEBUG
        case SDLK_e:
            this->SpawnEnemy();
            break;
#endif // _DEBUG
        }
        break;

    case SDL_KEYUP:
        switch(Evt.key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_w:
        case SDLK_s:
            speed = 0;
            break;

        case SDLK_RIGHT:
        case SDLK_LEFT:
        case SDLK_d:
        case SDLK_a:
            angle = 0;
            break;
        }
        break;
    }
    m_PlayerRate.Move(speed, angle);
}

/**
 * Updates all World elements.
 */
void CL_World::Update()
{
    // Player logic
    m_Player.Drive(m_PlayerRate.x);
    m_Player.Turn(m_PlayerRate.y);

    // Pan maps based on player position
    m_TerrainMap.Pan(m_Player.GetTankPosition());
    m_CollisionMap.Pan(m_Player.GetTankPosition());
    m_ObjectiveMap.Pan(m_Player.GetTankPosition());

    // Listener is player position.
    alListener3f(AL_POSITION,
        m_Player.GetTankPosition().x,
        m_Player.GetTankPosition().y, 
        0.0f);

    
    // Adjust objects to go with map panning
    m_Player.Adjust(m_TerrainMap.GetPanRate());
    for(ai::AI_Enemies::iterator i = ai::AI_Enemy::p_allEnemies.begin();
        i != ai::AI_Enemy::p_allEnemies.end(); ++i)
    {
        (*i)->Adjust(m_TerrainMap.GetPanRate());
    }

    // Render everything
    mp_Background->Update();
    m_TerrainMap.Update(false);
    m_CollisionMap.Update(false);
    m_ObjectiveMap.Update(false);
    m_Player.Update();
    
    // Update all of the enemies, rendering them and shooting if
    // the returned state specifies it.
    for(ai::AI_Enemies::iterator i = ai::AI_Enemy::p_allEnemies.begin();
        i != ai::AI_Enemy::p_allEnemies.end(); ++i)
    {
        int state = (*i)->Update();

        /// @todo Add a GetBarrelPosition() to the enemy base class.
        if(state & ai::e_FIRING_SECONDARY)
        {
            math::ML_Vector2 Aim_Vec = (*i)->GetPosition() - 
                m_Player.GetTankPosition();
            obj::Obj_Bullet* pBullet = (obj::Obj_Bullet*)
                g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                "Data/Images/Player_Shot2.png"));
            pBullet->SetStartPosition((*i)->GetPosition());
            pBullet->SetDamage(10);
            pBullet->Fire(m_Player.GetTankPosition());
            pBullet->Rotate(
                math::deg(atan2(Aim_Vec.x, Aim_Vec.y)) + 90);
            mp_enemyBullets[(*i)->GetID()].push_back(pBullet);
        }
        else if(state & ai::e_FIRING_PRIMARY)
        {
            math::ML_Vector2 Aim_Vec = (*i)->GetPosition() - 
                m_Player.GetTankPosition();
            obj::Obj_Bullet* pBullet = (obj::Obj_Bullet*)
                g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                "Data/Images/Player_Shot.png"));
            pBullet->SetStartPosition((*i)->GetPosition());
            pBullet->SetDamage(45);
            pBullet->Fire(m_Player.GetTankPosition());
            pBullet->Rotate(
                math::deg(atan2(Aim_Vec.x, Aim_Vec.y)) + 90);
            mp_enemyBullets[(*i)->GetID()].push_back(pBullet);
        }
    }

    // World logic
    this->HandleEvents();
    this->HandleCollisions();
}

/**
 * Handles all collisions with elements such as the player and map.
 */
void CL_World::HandleCollisions()
{
    // Check to see if the player can move to where they 
    // want to move to.
    if(m_CollisionMap.FindTile(m_Player.GetCollisionBox()) != NULL)
    {
        if(m_PlayerRate.x != 0.0f)
            m_Player.Drive(-m_PlayerRate.x);
        else if(m_PlayerRate.y != 0.0f)
            m_Player.Turn(-m_PlayerRate.y);
    }
    
    // Update player bullets on-screen and check if they are
    // off-screen. If they are, delete them. If they are currently
    // colliding with something, change the entity to a spark for one
    // frame, and delete it.
    for(obj::Obj_pBullets::iterator i = mp_playerBullets.begin();
        i != mp_playerBullets.end(); /* no third */)
    {
        asset::GL_Entity* pCurrent_Tile = m_CollisionMap.FindTile(
            (*i)->GetPosition());

        if((*i)->IsOffscreen(800, 600))
            i = mp_playerBullets.erase(i);

        else if(pCurrent_Tile != NULL)
        {
            math::ML_Vector2 Pos = (*i)->GetPosition();
            (*i) = (obj::Obj_Bullet*)g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                "Data/Images/Spark.png"));
            (*i)->Move(Pos);
            (*i)->Update();
            m_CollisionMap.RemoveTile(pCurrent_Tile);
            i = mp_playerBullets.erase(i);
        }
        else
        {
            for(ai::AI_Enemies::iterator j = ai::AI_Enemy::p_allEnemies.begin();
                j != ai::AI_Enemy::p_allEnemies.end(); /* no third */)
            {
                if((*i)->CheckCollision(*(*j)->GetMainEntity()))
                    j = ai::AI_Enemy::p_allEnemies.erase(j);
                else
                    ++j;
            }

            (*i)->Update();
            ++i;
        }
    }

    // Update all enemy bullets on-screen and check if they are
    // off-screen. If they are, delete them. If they are currently
    // colliding with something, change the entity to a spark for one
    // frame, and delete it.
    for(size_t i = 0; i < mp_enemyBullets.size(); i++)
    {
        for(obj::Obj_pBullets::iterator j = mp_enemyBullets[i].begin();
            j != mp_enemyBullets[i].end(); /* no third */)
        {
            asset::GL_Entity* pCurrent_Tile = m_CollisionMap.FindTile(
                (*j)->GetPosition());

            if((*j)->IsOffscreen(800, 600))
                j = mp_enemyBullets[i].erase(j);

            else if(pCurrent_Tile != NULL)
            {
                math::ML_Vector2 Pos = (*j)->GetPosition();
                (*j) = (obj::Obj_Bullet*)g_TextureAssets.GetEntityByID(
                    g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                    "Data/Images/Spark.png"));
                (*j)->Move(Pos);
                (*j)->Update();
                m_CollisionMap.RemoveTile(pCurrent_Tile);
                j = mp_enemyBullets[i].erase(j);
            }
            else
            {
                (*j)->Update();
                ++j;
            }
        }
    }
}

/**
 * Handles CL_World-specific events.
 * Leaves the SDL event queue untouched.
 */
void CL_World::HandleEvents()
{
    const math::ML_Vector2 Mouse = game::GetMousePosition();
    const math::ML_Vector2 Aim_Vec = m_Player.GetTankPosition() - Mouse;

    m_Player.Aim(Mouse);

    if(game::IsPressed(SDL_BUTTON_LEFT))
    {
        if(m_Player.FirePrimary())
        {
            obj::Obj_Bullet* pBullet = (obj::Obj_Bullet*)
                g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                "Data/Images/Player_Shot2.png"));
            pBullet->SetStartPosition(m_Player.GetBarrelPosition());

            pBullet->SetDamage(75);
            pBullet->Fire(Mouse);
            pBullet->Rotate(math::deg(
                atan2(Aim_Vec.x, Aim_Vec.y)) + 90);
            mp_playerBullets.push_back(pBullet);
        }
    }
    if(game::IsPressed(SDL_BUTTON_RIGHT))
    {
        if(m_Player.FireSecondary())
        {
            /// @bug Fat memory leak
            obj::Obj_Bullet* pBullet = (obj::Obj_Bullet*)
                g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromFile<obj::Obj_Bullet>(
                "Data/Images/Player_Shot2.png"));
            pBullet->SetStartPosition(m_Player.GetBarrelPosition());

            pBullet->SetDamage(10);
            pBullet->Fire(Mouse);
            pBullet->Rotate(math::deg(
                atan2(Aim_Vec.x, Aim_Vec.y)) + 90);
            mp_playerBullets.push_back(pBullet);
        }
    }
}

/**
 * Spawns an enemy in the game world.
 * 
 * @todo Find a point of interest for the enemy.
 */
bool CL_World::SpawnEnemy()
{
    std::vector<asset::GL_Entity*> p_allObjs;

    for(ai::AI_Enemies::iterator i = ai::AI_Enemy::p_allEnemies.begin();
        i != ai::AI_Enemy::p_allEnemies.end(); ++i)
    {
        p_allObjs.push_back((asset::GL_Entity*)(*i)->GetMainEntity());
    }

    asset::GL_Entity* p_Spawn = m_ObjectiveMap.GetAvailableEnemySpawn(p_allObjs);
    if(p_Spawn == NULL)
        return false;

    asset::GL_Entity* p_Dest = m_ObjectiveMap.GetNearestPOI(p_Spawn->GetPosition());
    if(p_Dest == NULL)
        return false;

    ai::AI_Enemy* p_Enemy = new ai::AI_Tank(m_TerrainMap, 
        m_CollisionMap, m_ObjectiveMap, m_Player);

    p_Enemy->Spawn(p_Spawn->GetPosition());
    p_Enemy->SetDestination(p_Dest->GetPosition() + 
        math::ML_Vector2(1.0f, 1.0f));

    obj::Obj_pBullets p_enemyBullets;
    p_enemyBullets.clear();
    p_allObjs.clear();
    mp_enemyBullets.push_back(p_enemyBullets);

    return true;
}
