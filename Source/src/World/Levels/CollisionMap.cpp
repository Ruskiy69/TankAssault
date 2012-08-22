/**
 * @file
 *  Definitions for the CL_CollisionMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.2.4
 */

#include <sstream>
#include <fstream>

#include "World/Levels/CollisionMap.h"

using game::CL_CollisionMap;
using asset::g_TextureAssets;

/**
 * Creates the main tile (yellow) that can be placed on the map in edit mode.
 *
 * @param bool Is it okay to edit the map?
 *
 * @see game::CL_Map::CL_Map()
 */
CL_CollisionMap::CL_CollisionMap(bool edit) : CL_Map(edit)
{
    mp_Overlay = gfx::create_surface_alpha(32, 32, gfx::YELLOW);

    if(edit)
        mp_CurrentTile = g_TextureAssets.GetEntityByID(
            g_TextureAssets.LoadEntityFromSurface(mp_Overlay));
}

/**
 * Cleans up memory by deleting the placeable tile.
 */
CL_CollisionMap::~CL_CollisionMap()
{
    g_Log.Flush();
    g_Log << "[DEBUG] CL_CollisionMap::~CL_CollisionMap() called.\n";

    SDL_FreeSurface(mp_Overlay);
}

/**
 * Loads a .ccm map file. 
 *
 * @param char* Filename
 *
 * @return TRUE if loaded successfully, FALSE if not or no filename.
 */
bool CL_CollisionMap::Load(const char* pfilename)
{
    if(pfilename == NULL)
        return false;

    std::ifstream map(pfilename);
    std::string line;
    std::vector<std::string> tileData;

    int x, y;

    g_Log.Flush();
    g_Log << "[INFO] Loading collision level: " << pfilename << ".\n";

    // Failed to open
    if(!map.is_open())
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load collision level: ";
        g_Log << pfilename << ".\n";
        return false;
    }

    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        delete mp_allTiles[i];
    mp_allTiles.clear();

    while(std::getline(map, line))
    {
        tileData.clear();

        // Comment or empty
        if(line.empty() || line[0] == '/')
            continue;

        // Format is x,y
        tileData = gk::split(line, ',');
        
        // Invalid data
        if(tileData.size() != 2)
        {
            gk::handle_error("Collision map file is corrupt.");
        }

        // Set the texture id to a new tile.
        asset::GL_Entity* pTile = g_TextureAssets.GetEntityByID(
            g_TextureAssets.LoadEntityFromSurface(mp_Overlay));

        // Parse the x y coordinates.
        x = atoi(tileData[0].c_str());
        y = atoi(tileData[1].c_str());

        // Actually tile the coordinates, just in case
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        // Move tile to parsed coordinates.
        pTile->Move(x, y);

        // Add to all ptiles.
        mp_allTiles.push_back(pTile);
    }

    tileData.clear();
    map.close();
    return true;
}

/**
 * Saves a map file.
 *
 * @param char* Filename
 *
 * @pre Edit-mode must be enabled.
 *
 * @return TRUE if saved, FALSE if failure, no filename, no tiles, or cannot edit.
 */
bool CL_CollisionMap::Save(const char* pfilename)
{
    if(pfilename == NULL || !m_can_edit || mp_allTiles.size() == 0)
        return false;

    if(std::string(pfilename).find(game::COLLISION_MAP_EXT) == std::string::npos)
        gk::handle_error("Collision files MUST be saved with a .ccm extension!");

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
 * Places a tile on the map.
 *
 * @param int X-coordinate
 * @param int Y-coordinate
 */
void CL_CollisionMap::PlaceTile(int x, int y)
{
    if(!m_can_edit)
        return;

    asset::GL_Entity* pTile = this->FindTile(x, y);
    if(pTile == NULL)
    {
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        pTile = this->FindTile(x, y);
        if(pTile == NULL)
        {
            pTile = g_TextureAssets.GetEntityByID(
                g_TextureAssets.LoadEntityFromSurface(mp_Overlay));
            pTile->Move(x, y);
            mp_allTiles.push_back(pTile);
        }
        else
        {
            for(std::vector<asset::GL_Entity*>::iterator i = mp_allTiles.begin();
                i != mp_allTiles.end(); /* no third */)
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
        for(std::vector<asset::GL_Entity*>::iterator i = mp_allTiles.begin();
            i != mp_allTiles.end(); /* no third */)
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
 *
 * @param bool Should we show the main placeable tile?
 */
void CL_CollisionMap::Update(bool show_active)
{
    if(m_can_edit)
    {
        glColor4f(1, 1, 1, 0.3f);
        for(size_t i = 0; i < mp_allTiles.size(); ++i)
        {
            if(mp_allTiles[i] != NULL)
                mp_allTiles[i]->Update();
        }
        glColor4f(1, 1, 1, 1);

        if(show_active)
        {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            mp_CurrentTile->Move(mouse_x, mouse_y);
            mp_CurrentTile->Update();
        }
    }
}