#include "World/Levels/Level.hpp"

using game::CLevel;

CLevel::CLevel() 
#ifdef _DEBUG
    : m_TerrainMap(true), m_CollisionMap(true),
        m_ObjectiveMap(true)
#endif // _DEBUG
    {}

bool CLevel::LoadLevel(const int level_no)
{
    std::stringstream filename;
    filename << "Data/Levels/Level" << level_no;
    m_levelname = filename.str();

    g_Log.Flush();
    g_Log << "[INFO] Loading level " << m_levelname << "*\n";
    g_Log.ShowLastLog();

    filename << game::TERRAIN_MAP_EXT;
    if(!m_TerrainMap.Load(filename.str().c_str()))
        return false;

    filename.str(std::string());
    filename << m_levelname << game::COLLISION_MAP_EXT;
    if(!m_CollisionMap.Load(filename.str().c_str()))
        return false;

    filename.str(std::string());
    filename << m_levelname << game::OBJ_MAP_EXT;
    if(!m_ObjectiveMap.Load(filename.str().c_str()))
        return false;

    return true;
}

game::CTerrainMap& CLevel::GetTerrainMap()
{
    return m_TerrainMap;
}

game::CCollisionMap& CLevel::GetCollisionMap()
{
    return m_CollisionMap;
}

game::CObjectiveMap& CLevel::GetObjectiveMap()
{
    return m_ObjectiveMap;
}

const std::string& CLevel::GetLevelName() const
{
    return m_levelname;
}

void CLevel::SetPanRate(const float rate)
{
    m_TerrainMap.SetPanRate(rate);
    m_CollisionMap.SetPanRate(rate);
    m_ObjectiveMap.SetPanRate(rate);
}

void CLevel::Update()
{
    m_TerrainMap.Update(false);
#ifdef _DEBUG
    glColor4f(1, 1, 1, 0.3f);
#else
    glColor4f(1, 1, 1, 0);
#endif // _DEBUG
    m_CollisionMap.Update(false);
    m_ObjectiveMap.Update(false);
    glColor4f(1, 1, 1, 1);
}

bool CLevel::PanMaps(const math::CVector2& Pos)
{
    return( m_TerrainMap.Pan(Pos)   && 
            m_CollisionMap.Pan(Pos) &&
            m_ObjectiveMap.Pan(Pos));
}
