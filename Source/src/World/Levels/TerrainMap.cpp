/**
 * @file
 *  Definitions for the CTerrainMap class
 *
 * @author George Kudrayvtsev
 * @version 1.1.2
 **/

#include <sstream>
#include <fstream>

#include "World/Levels/TerrainMap.hpp"

using game::CTerrainMap;
using asset::CAssetManager;

/**
 * Figures out available tiles.
 *
 * @param bool Is it okay to edit the map?
 * @pre Data/Levels/ValidNames.dat must exist.
 * @see game::CMap::CMap()
 **/
CTerrainMap::CTerrainMap(bool edit) : CMap(edit)
{
    std::ifstream tile_file("Data/Levels/ValidNames.dat");
    std::string line;

    if(!tile_file.is_open())
        gk::handle_error("No textures available for level editing\n"
                         "Check Data/Levels folder and try again.");

    while(std::getline(tile_file, line))
    {
        if(line.empty() || line[0] == '/')
            continue;

        m_textureNames.push_back(line);
        CAssetManager::Create<asset::CTexture>(line.c_str());
    }

    if(m_textureNames.size() == 0)
        gk::handle_error("No textures available for level editing\n"
                         "Check Data/Map folder and try again.");

    if(edit)
    {
        mp_CurrentTile = new obj::CGameObject;
        mp_CurrentTile->LoadFromTexture((asset::CTexture*)
            CAssetManager::Find(m_textureNames[0].c_str()));
    }

    g_Log.Flush();
    g_Log << "[DEBUG] CTerrainMap::CTerrainMap() called.\n";
}

/**
 * Clears all texture names.
 **/
CTerrainMap::~CTerrainMap()
{
    g_Log.Flush();
    g_Log << "[DEBUG] CTerrainMap::~CTerrainMap() called.\n";
    m_textureNames.clear();
}

/**
 * Loads a .ctm map file. 
 *
 * @param char* Filename
 * @return TRUE if loaded successfully, FALSE if not or no filename.
 **/
bool CTerrainMap::Load(const char* pfilename)
{
    if(pfilename == NULL)
        return false;

    std::ifstream map(pfilename);
    std::string line, filename;
    std::vector<std::string> tileData;

    int x, y;

    g_Log.Flush();
    g_Log << "[INFO] Loading terrain level: " << pfilename << ".\n";

    // Failed to open
    if(!map.is_open())
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load terrain level: ";
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

        // Format is filename:x,y
        tileData = gk::split(line, ':');
        
        // Invalid data
        if(tileData.size() != 2)
            continue;
        if(!this->IsValidTextureName(tileData[0].c_str()))
            continue;

        // Set the texture id to a new tile.
        obj::CGameObject* p_Tile = new obj::CGameObject;
        p_Tile->LoadFromTexture((asset::CTexture*)CAssetManager::Find(
            tileData[0].c_str()));

        // tileData becomes just x,y
        tileData = gk::split(tileData[1], ',');

        // Parse the x y coordinates.
        x = atoi(tileData[0].c_str());
        y = atoi(tileData[1].c_str());

        // Actually tile the coordinates, just in case
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        // Move tile to parsed coordinates.
        p_Tile->Move(x, y);
        p_Tile->Update();

        // Add to all tiles.
        mp_allTiles.push_back(p_Tile);
    }

    tileData.clear();
    map.close();
    
    return true;
}

/**
 * Saves a map file.
 *
 * @param char* Filename
 * @pre Edit-mode must be enabled.
 * @return TRUE if saved, FALSE if failure, no filename, no tiles, or cannot edit.
 **/
bool CTerrainMap::Save(const char* p_filename)
{
    if(p_filename == NULL || !m_can_edit || mp_allTiles.size() == 0)
    {
        g_Log.Flush();
        g_Log << "[INFO] Unable to save terrain map: Not in editing mode.\n";
        return false;
    }

    if(std::string(p_filename).find(game::TERRAIN_MAP_EXT) == std::string::npos)
        gk::handle_error("Terrain files MUST be saved with a .ctm extension!");

    std::stringstream line;
    std::ofstream map_file(p_filename);

    g_Log.Flush();
    g_Log << "[INFO] Saving terrain map: " << p_filename << ".\n";

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

        line << mp_allTiles[i]->GetFilename();
        line << ":" << (int)mp_allTiles[i]->GetX() << ",";
        line << (int)mp_allTiles[i]->GetY() << std::endl;

        map_file << line.str();
        line.str(std::string());
    }

    line.str(std::string());
    map_file.close();
    return true;
}

/**
 * Sets the next tile in the available tile list as the active tile.
 **/
void CTerrainMap::NextTile()
{
    if(!m_can_edit)
        return;

    static u_int index = 0;
    index++;
    if(index >= m_textureNames.size())
        index = 0;

    g_Log.Flush();
    g_Log << "[DEBUG] Switching terrain map to new tile: ";
    g_Log << m_textureNames[index] << ".\n";

    mp_CurrentTile->LoadFromTexture((asset::CTexture*)CAssetManager::Find(
        m_textureNames[index].c_str()));
}

/**
 * Places a tile on the map.
 *
 * @param int X-coordinate
 * @param int Y-coordinate
 **/
void CTerrainMap::PlaceTile(int x, int y)
{
    if(!m_can_edit)
        return;

    obj::CGameObject* p_Tile = this->FindTile(x, y);
    if(p_Tile == NULL)
    {
        while(x % 32 != 0) x--;
        while(y % 32 != 0) y--;

        p_Tile = this->FindTile(x, y);
        if(p_Tile == NULL)
        {
            p_Tile = new obj::CGameObject;
            p_Tile->LoadFromTexture((asset::CTexture*)CAssetManager::Find(
                mp_CurrentTile->GetFilename().c_str()));
            p_Tile->Move(x, y);
            mp_allTiles.push_back(p_Tile);
        }
        else
        {
            if(mp_CurrentTile->GetFilename() == p_Tile->GetFilename())
                return;

            p_Tile = new obj::CGameObject;
            p_Tile->LoadFromTexture((asset::CTexture*)CAssetManager::Find(
                mp_CurrentTile->GetFilename().c_str()));
        }
    }
    else
    {
        if(mp_CurrentTile->GetFilename() == p_Tile->GetFilename())
                return;

        p_Tile = new obj::CGameObject;
        p_Tile->LoadFromTexture((asset::CTexture*)CAssetManager::Find(
            mp_CurrentTile->GetFilename().c_str()));
    }
}

/**
 * Updates all the tiles on-screen.
 * @param bool Should we show the main placeable tile?
 **/
void CTerrainMap::Update(bool show_active)
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
    {
        mp_allTiles[i]->Update();
    }

    if(m_can_edit && show_active)
    {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        mp_CurrentTile->Move(mouse_x, mouse_y);
        mp_CurrentTile->Update();
    }
}

/**
 * Tests for a valid, available texture name.
 *
 * @param char* Texture name to check
 * @return TRUE if valid, FALSE if not.
 **/
bool CTerrainMap::IsValidTextureName(const char* ptexture_name)
{
    for(size_t i = 0; i < m_textureNames.size(); ++i)
        if(m_textureNames[i] == ptexture_name)
            return true;

    return false;
}
