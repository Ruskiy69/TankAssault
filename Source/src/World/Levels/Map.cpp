/**
 * @file
 *  Definitions for the CMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.2.1
 **/

#include <sstream>
#include <fstream>

#include "World/Levels/Map.hpp"

using game::CMap;

CMap::CMap(bool edit_mode /*= false**/) : 
    m_can_edit(edit_mode), m_pan_adjustment_rate(32), mp_CurrentTile(NULL)
{
    mp_allTiles.clear();
}

/**
 * Cleans up memory by deleting all tiles.
 **/
CMap::~CMap()
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        delete mp_allTiles[i];

    mp_allTiles.clear();
}

/**
 * @overload CMap::FindTile(const math::CVector2& Pos)
 *
 * @param int X-coordinate
 * @param int Y-coordinate
 **/
obj::CGameObject* CMap::FindTile(const int x, const int y) const
{
    return this->FindTile(math::CVector2(x, y));
}

/**
 * Finds a tile in the list of tiles that's located in the given position.
 *
 * @param math::CVector2& Position to find tile in.
 * @return Tile that collides with area given, NULL otherwise.
 **/
obj::CGameObject* CMap::FindTile(const math::CVector2& Pos) const
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        if(mp_allTiles[i]->CheckCollision(Pos.x, Pos.y))
            return mp_allTiles[i];

    return NULL;
}

/**
 * @overload CMap::FindTile(const math::CVector2& Pos)
 * @param math::Rect& Area to find tile in.
 **/
obj::CGameObject* CMap::FindTile(const math::CRect& Area) const
{
    for(size_t i = 0; i < mp_allTiles.size(); ++i)
        if(mp_allTiles[i]->CheckCollision(Area))
            return mp_allTiles[i];

    return NULL;
}

/**
 * Removes a tile from the total list of tiles that matches a location.
 *
 * @param int X-coordinate
 * @param int Y-coordinate
 **/
void CMap::RemoveTile(int x, int y)
{
    /// @todo This shouldn't even work.
    obj::CGameObject* pTile = this->FindTile(x, y);

    for(std::vector<obj::CGameObject*>::iterator i = mp_allTiles.begin();
        i != mp_allTiles.end(); /* no third **/)
    {
        if((*i) == pTile)
            i = mp_allTiles.erase(i);
        else
            ++i;
    }
}

/**
 * @overload CMap::RemoveTile(const int x, const int y);
 * @param math::CVector2& Position to remove tile from, if it exists
 **/
void CMap::RemoveTile(const math::CVector2& Pos)
{
    obj::CGameObject* pTile = this->FindTile(Pos);

    for(std::vector<obj::CGameObject*>::iterator i = mp_allTiles.begin();
        i != mp_allTiles.end(); /* no third **/)
    {
        if((*i) == pTile)
            i = mp_allTiles.erase(i);
        else
            ++i;
    }
}

/**
 * @overload CMap::RemoveTile(const int x, const int y);
 * @param obj::CGameObject* Tile to remove.
 **/
void CMap::RemoveTile(const obj::CGameObject* pTile)
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

/**
 * Pans the map in the proper direction if the given position
 * is within 100 pixels of the screen boundaries.
 *
 * @param math::CVector2& Position
 * @return TRUE if panning was done, FALSE otherwise.
 **/
bool CMap::Pan(const math::CVector2& Position)
{
    static int dx, dy;
    dx = dy = 0;
    m_PanRate.Move(dx, dy);

    if(Position.x > 800.0f - 100.0f)
        dx = -m_pan_adjustment_rate;
    else if(Position.x < 100.0f)
        dx = m_pan_adjustment_rate;
    if(Position.y > 600.0f - 100.0f)
        dy = -m_pan_adjustment_rate;
    else if(Position.y < 100.0f)
        dy = m_pan_adjustment_rate;

    for(size_t i = 0; i < mp_allTiles.size(); i++)
    {
        mp_allTiles[i]->Move_Rate(dx, dy);
    }

    m_PanRate.Move(dx, dy);

    return (dx != 0 || dy != 0);
}

/**
 * Sets the rate at which to pan the map.
 * @param int Rate
 **/
void CMap::SetPanRate(const int rate)
{
    m_pan_adjustment_rate = rate;
}

/**
 * Retrieves the latest panning rate.
 *
 * @see CMap::Pan()
 * @return A vector representing the pan rate.
 **/
const math::CVector2& CMap::GetPanRate() const
{
    return m_PanRate;
}

/**
 * Retrieves the current map tiles.
 * @return An unmodifiable vector reference to the current tiles.
 **/
const std::vector<obj::CGameObject*>& CMap::GetTiles() const
{
    return mp_allTiles;
}
