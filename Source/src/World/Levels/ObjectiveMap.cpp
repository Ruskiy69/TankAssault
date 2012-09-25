/**
 * @file
 *  Implementation of the CObjectiveMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.3
 **/

#include <sstream>
#include <fstream>

#include "World/Levels/ObjectiveMap.hpp"

using game::CObjectiveMap;
using asset::CAssetManager;

/**
 * Creates the main tile (blue) that can be placed on the map in edit mode.
 * @param bool Is it okay to edit the map?
 * @see game::CMap::CMap()
 **/
CObjectiveMap::CObjectiveMap(bool can_edit) : CMap(can_edit)
{
    mp_Overlay = gfx::create_surface_alpha(32, 32, gfx::PURPLE);

    if(can_edit)
    {
        mp_CurrentTile = new obj::CGameObject;
        mp_CurrentTile->LoadFromSurface(mp_Overlay);
    }
}

/**
 * Cleans up memory by deleting the placeable tile.
 **/
CObjectiveMap::~CObjectiveMap()
{
    SDL_FreeSurface(mp_Overlay);
}

/**
 * Loads a .cam map file. 
 *
 * @param char* Filename
 * @return TRUE if loaded successfully, FALSE if not or no filename.
 **/
bool CObjectiveMap::Load(const char* pfilename)
{
    if(pfilename == NULL)
        return false;

    std::ifstream map(pfilename);
    std::string line;
    std::vector<std::string> tileData;

    int x, y;

    g_Log.Flush();
    g_Log << "[INFO] Loading objective level: " << pfilename << ".\n";

    // Failed to open
    if(!map.is_open())
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load objective level: ";
        g_Log << pfilename << ".\n";
        return false;
    }

    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        delete mp_allTiles[i];
    mp_allTiles.clear();

    while(std::getline(map, line))
    {
        tileData.clear();

        // Comment or empty line
        if(line.empty() || line[0] == '/')
            continue;

        // Format is TYPE:x,y
        tileData = gk::split(line, ':');

        // Invalid?
        if(tileData.size() != 2)
            gk::handle_error("AI map file is corrupt.");

        // Determine tile type
        m_allTileAttributes.push_back((TileAttributes)atoi(tileData[0].c_str()));
        if(m_allTileAttributes.back() == e_POI)
            gfx::fill_rect(mp_Overlay, NULL, gfx::PURPLE);
        else if(m_allTileAttributes.back() == e_PLAYER_SPAWN)
            gfx::fill_rect(mp_Overlay, NULL, gfx::BLACK);
        else if(m_allTileAttributes.back() == e_ENEMY_SPAWN)
            gfx::fill_rect(mp_Overlay, NULL, gfx::RED);

        // Make a new tile
        obj::CGameObject* pTile = new obj::CGameObject;
        pTile->LoadFromSurface(mp_Overlay);

        tileData = gk::split(tileData[1], ',');

        // Parse coordinates
        x = atoi(tileData[0].c_str());
        y = atoi(tileData[1].c_str());

        // Tile them to align, just in case
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        // Move the tile and it to all tiles
        pTile->Move(x, y);
        mp_allTiles.push_back(pTile);
    }

    if(m_can_edit)
        mp_CurrentTile->LoadFromSurface(mp_Overlay);

    tileData.clear();
    map.close();

    return true;
}

/**
 * Saves a map file.
 *
 * @param char* Filename
 * @return TRUE if saved, FALSE if failure, no filename, no tiles, or cannot edit.
 * @pre Edit-mode must be enabled.
 * @bug If there's nothing in the top left corner, map is saved incorrectly
 **/
bool CObjectiveMap::Save(const char* pfilename)
{
    if(pfilename == NULL || !m_can_edit || mp_allTiles.size() == 0)
        return false;

    if(std::string(pfilename).find(game::OBJ_MAP_EXT) == std::string::npos)
        gk::handle_error("AI files MUST be saved with a .cam extension!");

    std::stringstream line;
    std::ofstream map_file(pfilename);

    // First, adjust all map tiles so the first one is at (0, 0)
    // Basically, undo any panning operations.
    float min_x = mp_allTiles[0]->GetX();
    float min_y = mp_allTiles[0]->GetY();
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        min_x = min(mp_allTiles[i]->GetX(), min_x);
    }
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        min_y = min(mp_allTiles[i]->GetY(), min_y);
    }

    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        mp_allTiles[i]->Move_Rate(-min_x, -min_y);

        line << m_allTileAttributes[i] << ":";
        line << (int)mp_allTiles[i]->GetX() << ",";
        line << (int)mp_allTiles[i]->GetY() << std::endl;

        map_file << line.str();
        line.str(std::string());
    }

    line.str(std::string());
    map_file.close();
    return true;
}

/**
 * Iterates through the list of available tiles.
 **/
void CObjectiveMap::NextTile()
{
    static TileAttributes current = e_POI;
    if(current == e_POI)
    {
        current = e_ENEMY_SPAWN;
        gfx::fill_rect(mp_Overlay, NULL, gfx::PURPLE);
    }
    else if(current == e_ENEMY_SPAWN)
    {
        current = e_PLAYER_SPAWN;
        gfx::fill_rect(mp_Overlay, NULL, gfx::RED);
    }
    else if(current == e_PLAYER_SPAWN)
    {
        current = e_POI;
        gfx::fill_rect(mp_Overlay, NULL, gfx::BLACK);
    }
    else
        gk::handle_error("Invalid AI tile state!");

    mp_CurrentTile->LoadFromSurface(mp_Overlay);
}

/**
 * Places a tile on the map.
 *
 * @param int X-coordinate
 * @param int Y-coordinate
 **/
void CObjectiveMap::PlaceTile(int x, int y)
{
    if(!m_can_edit)
        return;

    obj::CGameObject* pTile = this->FindTile(x, y);
    if(pTile == NULL)
    {
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        pTile = this->FindTile(x, y);
        if(pTile == NULL)
        {
            pTile->LoadFromSurface(mp_Overlay);
            pTile->Move(x, y);
            mp_allTiles.push_back(pTile);
            Uint32 raw_color = gfx::get_pixel(mp_Overlay, 0, 0);

            // Convert 32-bit int to rgb values
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            SDL_Color Color = {
                (raw_color & 0x00ff0000) / 0x10000,
                (raw_color & 0x0000ff00) / 0x100,
                (raw_color & 0x000000ff),
                0};
#else
            SDL_Color Color = {
                (raw_color & 0x000000ff),
                (raw_color & 0x0000ff00) / 0x100,
                (raw_color & 0x00ff0000) / 0x10000,
                0};
#endif
            if(Color == gfx::PURPLE) // purple
                m_allTileAttributes.push_back(e_POI);
            else if(Color == gfx::BLACK) // nigga
                m_allTileAttributes.push_back(e_PLAYER_SPAWN);
            else if(Color == gfx::RED) // red
                m_allTileAttributes.push_back(e_ENEMY_SPAWN);
            else
                gk::handle_error("Invalid color?");
        }
        else
        {
            for(std::vector<obj::CGameObject*>::iterator i = mp_allTiles.begin();
                i != mp_allTiles.end(); /* no third **/)
            {
                if((*i) == pTile)
                    i = mp_allTiles.erase(i);
                else
                    ++i;
            }
        }
    }
    else
    {
        for(std::vector<obj::CGameObject*>::iterator i = mp_allTiles.begin();
            i != mp_allTiles.end(); /* no third **/)
        {
            if((*i) == pTile)
                i = mp_allTiles.erase(i);
            else
                ++i;
        }
    }
}

/**
 * Updates all the tiles on-screen if edit mode is enabled.
 * @param bool Should we show the main placeable tile?
 **/
void CObjectiveMap::Update(bool show_active)
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        if(mp_allTiles[i] != NULL)
            mp_allTiles[i]->Update();

    if(m_can_edit)
    {
        if(show_active)
        {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            mp_CurrentTile->Move(mouse_x, mouse_y);
            mp_CurrentTile->Update();
        }
    }
}

/**
 * Finds the nearest point-of-interest.
 *  Given a location, finds the nearest point of interest that is at
 *  least 64px away from the position.
 *
 * @param math::CVector2& Position
 * @return NULL if no position found, the point-of-interest tile otherwise.
 **/
obj::CGameObject* CObjectiveMap::GetNearestPOI(
    const math::CVector2& Position) const
{
    // Find the first point of interest
    float min_d = 0.0f;
    int index = 0;
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        if(m_allTileAttributes[i] == e_POI)
        {
            float d = math::distance(Position.x, Position.y, 
                mp_allTiles[i]->GetX(), mp_allTiles[i]->GetY());
            if(d > 64.0f)
                min_d = d;
            index = i;
            break;
        }
    }

    for(size_t i = index; i < mp_allTiles.size(); ++i)
    {
        float d = math::distance(Position.x, Position.y, 
            mp_allTiles[i]->GetX(), mp_allTiles[i]->GetY());
        if(d < min_d && d > 64.0f && m_allTileAttributes[i] == e_POI)
        {
            min_d = d;
            index = i;
        }
    }

    if(min_d < 64.0f)
        return NULL;
    else
        return mp_allTiles[index];
}

/**
 * Finds the next available enemy spawn.
 *  Iterating through the list of spawns, the map checks to make sure there
 *  is not an enemy already spawn at that location (at least within 64px).
 *
 * @param std::vector<obj::CGameObject*> All of the in-game objects
 * @return The next available tile to spawn on.
 **/
obj::CGameObject* CObjectiveMap::GetAvailableEnemySpawn(
    const std::vector<const obj::CGameObject*>& p_allObjects) const
{
    bool touches = false;
    for(size_t i = 1; i < mp_allTiles.size(); ++i)
    {
        for(size_t j = 0; j < p_allObjects.size(); ++j)
        {
            // Make sure no objects are within a 64px radius of the spawn
            math::CRect Boundary(p_allObjects[j]->GetX(), 
                p_allObjects[j]->GetY(), 64, 64);

            if(mp_allTiles[i]->CheckCollision(Boundary) &&
                m_allTileAttributes[i] == e_ENEMY_SPAWN)
            {
                touches = true;
                break;
            }
        }

        if(m_allTileAttributes[i] == e_ENEMY_SPAWN && !touches)
            return mp_allTiles[i];
        
        touches = false;
    }

    return NULL;
}

/**
 * Finds the player spawn on the map.
 * @return The tile for the player to spawn on.
 **/
obj::CGameObject* CObjectiveMap::GetPlayerSpawn() const
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        if(m_allTileAttributes[i] == e_PLAYER_SPAWN)
            return mp_allTiles[i];
    }

    return NULL;
}