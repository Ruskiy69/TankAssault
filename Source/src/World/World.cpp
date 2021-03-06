/**
 * @file
 *  Implementation of the CWorld class.
 *
 * @author  George Kudrayvtsev
 * @version 0.1
 */
 
#include "World/World.hpp"

using game::CWorld;
using game::g_Log;
using game::g_Settings;
using asset::CAssetManager;

/**
 * Initialize all of the internal components.
 * @param GameState& The current engine state
 */
CWorld::CWorld(game::GameState& engine_state) :  m_engine_state(engine_state) {}

void CWorld::Init()
{
    g_Log.Flush();
    g_Log << "[INFO] Initializing world.\n";
    g_Log.ShowLastLog();

    // Load first level
    game::CLevel* pLevelOne = new game::CLevel;
    if(!pLevelOne->LoadLevel(1))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load level.\n";
        gk::handle_error(g_Log.GetLastLog().c_str());
    }
    mp_Levels.clear();
    mp_Levels.push_back(pLevelOne);

    mp_Levels[0]->SetPanRate(3.0f);
    mp_Levels[0]->Update();
    mp_ActiveLevel = mp_Levels[0];

    // Load game background
    m_Background.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        g_Settings.GetValueAt("GameBackground").c_str()));

    // Load lighting shader
    if(!m_Lighting.LoadFromFile("Data/Shaders/Lighting.vs",
        "Data/Shaders/Lighting.fs"))
    {
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    // Set up shader values
    // {0.05, 0.00145, 0}{0.005} gives a soft, almost ambient light.
    // {0.05, 0.01, 0.00001}{0.1} gives a small light that sufficiently lights around it
    // {0.05, 0.01, 0}{0.1} is what I will use in-game
    int screen[1]     = {600};

    m_Lighting.PassVariableiv("scr_height", screen, 1, 1);

    // Set player spawn location.
    if(!m_Player.Init(g_Settings))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to initialize player data.\n";
        gk::handle_error(g_Log.GetLastLog().c_str());
    }
    m_Player.SetSpawn(
        mp_ActiveLevel->GetObjectiveMap().GetPlayerSpawn()->GetPosition());
    m_Player.Update();

    while(!mp_ActiveLevel->PanMaps(m_Player.GetPosition()));

    // Spawn enemies at all available spawns.
    while(this->SpawnEnemy());

    // Add map lights to shader.
    std::vector<gfx::CLight*>& allLights = mp_ActiveLevel->GetObjectiveMap().GetLights();

    if(!m_Lighting.SetMacro("MAX_LIGHTS", allLights.size()))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to set shader macro!\n";
        g_Log << "[ERROR] " << m_Lighting.GetError() << "\n";
        g_Log.ShowLastLog();
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    float* positions    = new float[allLights.size() * 2];
    float* attns        = new float[allLights.size() * 3];
    float* cols         = new float[allLights.size() * 3];
    float* brightnesses = new float[allLights.size()];

    for(size_t i = 0; i < allLights.size(); ++i)
    {
        positions[i * 2]    = allLights[i]->GetPosition()[0];
        positions[i * 2 + 1]= allLights[i]->GetPosition()[1];

        attns[i * 3]        = allLights[i]->GetAttenuation()[0];
        attns[i * 3 + 1]    = allLights[i]->GetAttenuation()[1];
        attns[i * 3 + 2]    = allLights[i]->GetAttenuation()[2];

        cols[i * 3]         = allLights[i]->GetColor()[0];
        cols[i * 3 + 1]     = allLights[i]->GetColor()[1];
        cols[i * 3 + 2]     = allLights[i]->GetColor()[2];

        brightnesses[i]     = allLights[i]->GetBrightness();
    }

    m_Lighting.PassVariablefv("light_pos", positions,    2, allLights.size());
    m_Lighting.PassVariablefv("light_att", attns,        3, allLights.size());
    m_Lighting.PassVariablefv("light_col", cols,         3, allLights.size());
    m_Lighting.PassVariablefv("light_brt", brightnesses, 1, allLights.size());

    delete[] positions;
    delete[] attns;
    delete[] cols;
    delete[] brightnesses;

    //mp_enemy_light_poss = new float[mp_Enemies.size() * 2];
    //mp_enemy_light_atts = new float[mp_Enemies.size() * 3];
    //mp_enemy_light_cols = new float[mp_Enemies.size() * 3];

    //for(size_t i = 0; i < mp_Enemies.size() * 3; ++i)
    //    mp_enemy_light_cols[i] = 255.0f / (rand() % 255);
    //for(size_t i = 0; i < mp_Enemies.size() * 3; i += 3)
    //    mp_enemy_light_atts[i] = 0.05f;
    //for(size_t i = 1; i < mp_Enemies.size() * 3; i += 3)
    //    mp_enemy_light_atts[i] = 0.01f;
    //for(size_t i = 2; i < mp_Enemies.size() * 3; i += 3)
    //    mp_enemy_light_atts[i] = 0.0f;
    //for(size_t i = 0; i < mp_Enemies.size() * 2; ++i)
    //    mp_enemy_light_poss[i] = rand() % 600;

    //m_Lighting.PassVariablefv("light_col", mp_enemy_light_cols, 3, mp_Enemies.size());
    //m_Lighting.PassVariablefv("light_att", mp_enemy_light_atts, 3, mp_Enemies.size());
    //m_Lighting.PassVariablefv("light_pos", mp_enemy_light_poss, 2, mp_Enemies.size());
}

CWorld::~CWorld()
{
    g_Log.Flush();
    g_Log << "[DEBUG] Destroying CWorld instance.\n";
    
    for(obj::pBulletCollection::iterator i = mp_playerBullets.begin(); 
        i != mp_playerBullets.end(); /* no third */)
    {
        delete (*i);
        i = mp_playerBullets.erase(i);
    }
    mp_playerBullets.clear();

    for(obj::pBulletCollection::iterator i = mp_enemyBullets.begin();
        i != mp_enemyBullets.end(); )
    {
        delete (*i);
        i = mp_enemyBullets.erase(i);
    }        
    mp_enemyBullets.clear();

    for(ai::CEnemies::iterator i = ai::CEnemy::p_allEnemies.begin(); 
        i != ai::CEnemy::p_allEnemies.end(); /* no third */)
    {
        i = ai::CEnemy::p_allEnemies.erase(i);
    }

    ai::CEnemy::p_allEnemies.clear();
    m_engine_state = game::e_QUIT;
}

/**
 * Handles a specific event given by game::Game_Engine.
 *
 * @param SDL_Event& The event
 *
 * @pre The current engine state must be e_GAME.
 * @see CollapseDef.hpp
 * @see game::Game_State
 */
void CWorld::HandleSystemEvent(const SDL_Event& Evt)
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
        g_Log << "[DEBUG] CWorld received a key-down event.\n";
        g_Log << "[DEBUG] Event ID: " << Evt.key.keysym.sym << ".\n";

        switch(Evt.key.keysym.sym)
        {
        case SDLK_w:
        case SDLK_UP:
            speed = -m_Player.DEFAULT_TANK_SPEED;
            break;

        case SDLK_s:
        case SDLK_DOWN:
            speed = m_Player.DEFAULT_TANK_SPEED;
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
    m_PlayerRate.Move(speed, -angle);
}

/// Updates all World elements.
void CWorld::Update()
{
    // Player logic
    m_Player.Drive(m_PlayerRate.x);
    m_Player.Turn(m_PlayerRate.y);

    // Pan maps based on player position
    mp_ActiveLevel->PanMaps(m_Player.GetPosition());

    // Listener is player position.
    //alListener3f(AL_POSITION,
    //    m_Player.GetPosition().x,
    //    m_Player.GetPosition().y, 
    //    0.0f);
    
    std::vector<gfx::CLight*>& allLights = mp_ActiveLevel->GetObjectiveMap().GetLights();
    float* positions = new float[allLights.size() * 2];
    for(size_t i = 0; i < allLights.size(); ++i)
    {
        positions[i * 2]    = allLights[i]->GetPosition()[0];
        positions[i * 2 + 1]= allLights[i]->GetPosition()[1];
    }

    m_Lighting.PassVariablefv("light_pos", positions, 2, allLights.size());
    delete[] positions;

    // Adjust objects to go with map panning
    m_Player.Adjust(mp_Levels[0]->GetTerrainMap().GetPanRate());
    for(std::list<ai::CEnemyTank*>::iterator i = mp_Enemies.begin();
        i != mp_Enemies.end(); ++i)
    {

        (*i)->Adjust(mp_ActiveLevel->GetTerrainMap().GetPanRate());
    }

    // Render everything
    m_Lighting.Link();
    m_Background.Update();
    mp_Levels[0]->Update();
    m_Player.Update();

    // Update all of the enemies, rendering them and shooting if
    // the returned state specifies it.
    for(std::list<ai::CEnemyTank*>::iterator i = mp_Enemies.begin();
        i != mp_Enemies.end(); ++i)
    {
        int state = (*i)->Update();

        /// @todo Add a GetBarrelPosition() to the enemy base class.
        if(state & ai::e_FIRING_SECONDARY)
        {
            math::CVector2 Aim_Vec = (*i)->GetPosition() - 
                m_Player.GetPosition();
            obj::CBullet* pBullet = new obj::CBullet;
            pBullet->LoadFromTexture((*i)->GetSecondary().GetProjectileTexture());
            pBullet->SetDamage((*i)->GetSecondary().GetDamage());
            pBullet->Launch((*i)->GetBarrelPosition(), m_Player.GetPosition());
            pBullet->Rotate(math::deg(atan2(Aim_Vec.y, Aim_Vec.x)));
            mp_enemyBullets.push_back(pBullet);
        }
        else if(state & ai::e_FIRING_PRIMARY)
        {
            math::CVector2 Aim_Vec = (*i)->GetPosition() - 
                m_Player.GetPosition();
            obj::CBullet* pBullet = new obj::CBullet;
            pBullet->LoadFromTexture((*i)->GetPrimary().GetProjectileTexture());
            pBullet->SetDamage((*i)->GetPrimary().GetDamage());
            pBullet->Launch((*i)->GetBarrelPosition(), m_Player.GetPosition());
            pBullet->Rotate(math::deg(atan2(Aim_Vec.y, Aim_Vec.x)) + 180);
            mp_enemyBullets.push_back(pBullet);
        }
    }

    // Finished rendering.
    m_Lighting.Unlink();

    // World logic
    this->HandleWorldEvents();
    this->HandleCollisions();
}

/// Handles all collisions with elements such as the player and map.
void CWorld::HandleCollisions()
{
    // Check to see if the player can move to where they 
    // want to move to.
    if(mp_ActiveLevel->GetCollisionMap().FindTile(m_Player.GetCollisionBox()) != NULL)
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
    for(obj::pBulletCollection::iterator i = mp_playerBullets.begin();
        i != mp_playerBullets.end(); /* no third */)
    {
        if((*i)->GetLifetime() <= 0.0f)
        {
            i = mp_playerBullets.erase(i);
            continue;
        }

        obj::CGameObject* pCurrent_Tile = mp_ActiveLevel->GetCollisionMap().
            FindTile((*i)->GetPosition());

        if(pCurrent_Tile != NULL)
        {
            (*i)->LoadFromTexture(
                asset::CAssetManager::Create<asset::CTexture>(
                "Data/Textures/Sprites/Spark.png"));
            (*i)->Update();
            mp_ActiveLevel->GetCollisionMap().RemoveTile(pCurrent_Tile);
            i = mp_playerBullets.erase(i);
        }
        else
        {
            for(std::list<ai::CEnemyTank*>::iterator j = mp_Enemies.begin();
                j != mp_Enemies.end(); /* no third */)
            {
                if((*i)->CheckCollision((*j)->GetMainEntity()))
                {
                    (*i)->LoadFromTexture(
                        asset::CAssetManager::Create<asset::CTexture>(
                        "Data/Textures/Sprites/Spark.png"));
                    (*i)->SetLifetime(0.0f);    // It'll be deleted next frame
                    (*j)->Damage((*i)->GetDamage());
                    if(!(*j)->IsAlive())
                    {
                        j = mp_Enemies.erase(j);
                        m_Player.IncreaseKillCount();
                    }
                    else ++j;
                    continue;
                }

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
    for(obj::pBulletCollection::iterator j = mp_enemyBullets.begin();
            j != mp_enemyBullets.end(); /* no third */)
    {
        if((*j)->GetLifetime() <= 0.0f)
        {
            j = mp_enemyBullets.erase(j);
            continue;
        }

        obj::CGameObject* pCurrent_Tile = mp_ActiveLevel->GetCollisionMap().
            FindTile((*j)->GetPosition());
        
        if(pCurrent_Tile != NULL)
        {
            (*j)->LoadFromTexture(
                asset::CAssetManager::Create<asset::CTexture>(
                "Data/Textures/Sprites/Spark.png"));
            (*j)->Update();
            mp_ActiveLevel->GetCollisionMap().RemoveTile(pCurrent_Tile);
            j = mp_enemyBullets.erase(j);
        }
        else if(m_Player.GetTankEntity()->CheckCollision(*j) ||
            m_Player.GetTowerEntity()->CheckCollision(*j))
        {
            m_Player.Damage((*j)->GetDamage());
            j = mp_enemyBullets.erase(j);
        }
        else
        {
            (*j)->Update();
            ++j;
        }
    }
}

/**
 * Handles CWorld-specific events.
 * Leaves the SDL event queue untouched.
 */
void CWorld::HandleWorldEvents()
{
    const math::CVector2 Mouse   = game::GetMousePosition();
    const math::CVector2 Aim_Vec = m_Player.GetPosition() - Mouse;

    m_Player.Aim(Mouse);

    if(game::IsDown(SDLK_r))
    {
        m_Player.GetSecondary().mp_ReloadSound->Play();
    }

    if(game::IsPressed(SDL_BUTTON_LEFT))
    {
        if(m_Player.FirePrimary())
        {
            obj::CBullet* pBullet = new obj::CBullet;
            pBullet->LoadFromTexture(m_Player.GetPrimary().GetProjectileTexture());
            pBullet->SetDamage(m_Player.GetPrimary().GetDamage());
            pBullet->Launch(m_Player.GetBarrelPosition(), Mouse);
            pBullet->Rotate(math::deg(atan2(Aim_Vec.y, Aim_Vec.x)) + 180);

            mp_playerBullets.push_back(pBullet);
        }
    }
    if(game::IsPressed(SDL_BUTTON_RIGHT))
    {
        if(m_Player.FireSecondary())
        {
            obj::CBullet* pBullet = new obj::CBullet;
            pBullet->LoadFromTexture(m_Player.GetSecondary().GetProjectileTexture());
            pBullet->SetDamage(m_Player.GetSecondary().GetDamage());
            pBullet->Launch(m_Player.GetBarrelPosition(), Mouse);
            pBullet->Rotate(math::deg(atan2(Aim_Vec.y, Aim_Vec.x)));

            mp_playerBullets.push_back(pBullet);
        }
    }
}

/// Spawns an enemy in the game world.
bool CWorld::SpawnEnemy()
{
    g_Log.Flush();
    g_Log << "[DEBUG] Spawning enemy.\n";
    g_Log.ShowLastLog();

    std::vector<const obj::CGameObject*> p_allObjs;

    for(ai::CEnemies::iterator i = ai::CEnemy::p_allEnemies.begin();
        i != ai::CEnemy::p_allEnemies.end(); ++i)
    {
        (*i)->Update();
        p_allObjs.push_back((*i)->GetMainEntity());
    }

    obj::CEntity* p_Spawn = mp_ActiveLevel->
        GetObjectiveMap().GetAvailableEnemySpawn(p_allObjs);
    if(p_Spawn == NULL)
        return false;

    obj::CEntity* p_Dest = mp_ActiveLevel->
        GetObjectiveMap().GetNearestPOI(p_Spawn->GetPosition());
    if(p_Dest == NULL)
        return false;

    ai::CEnemyTank* p_Enemy = new ai::CEnemyTank(mp_ActiveLevel, m_Player);
    
    p_Enemy->Init(g_Settings);
    p_Enemy->Spawn(p_Spawn->GetPosition());
    p_Enemy->Update();
    p_Enemy->SetDestination(p_Dest->GetPosition() + math::CVector2(1.0f, 1.0f));
    mp_Enemies.push_back(p_Enemy);

    return true;
}

void CWorld::HandleGameEvent(const game::GameEvent* const pEvt)
{
}

obj::CPlayer& CWorld::GetPlayer()
{
    return m_Player;
}
