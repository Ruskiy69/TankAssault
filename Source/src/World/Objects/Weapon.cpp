#include "World/Objects/Weapon.hpp"

using obj::CWeapon;
using asset::CAssetManager;

CWeapon::CWeapon() : mp_FireSound(NULL), mp_ReloadSound(NULL),
    m_mag_count(0), m_current_clip(0), m_fire_delay(0),
    m_reload_delay(0), m_DAMAGE(0), m_CLIP_SIZE(0), m_FIRING_DELAY(0),
    m_RELOAD_DELAY(0), m_ammo_count(0) {}

CWeapon::~CWeapon() {}

bool CWeapon::Init(const std::string& wp_data_filename)
{
    std::ifstream file(wp_data_filename);
    std::string line;
    std::vector<std::string> lineData;

    if(!file.is_open())
        return false;

    while(std::getline(file, line))
    {
        if(line.empty() || line[0] == '/' || line[0] == '[')
            continue;

        lineData = gk::split(line, '=');
        if(lineData.size() != 2)    // Corrupt line
            continue;

        if(lineData[0].find("WeaponName") != std::string::npos)
        {
            m_weapon_name = lineData[1];
        }
        else if(lineData[0].find("SFXShtLoc") != std::string::npos)
        {
            mp_FireSound = CAssetManager::Create<asset::CSound2D>(
                lineData[1].c_str());
        }
        else if(lineData[0].find("SFXRldLoc") != std::string::npos)
        {
            mp_ReloadSound = CAssetManager::Create<asset::CSound2D>(
                lineData[1].c_str());
        }
        else if(lineData[0].find("IMGLocSm") != std::string::npos)
        {
            m_OnTankSprite.LoadFromTexture(
                CAssetManager::Create<asset::CTexture>(
                lineData[1].c_str()));
        }
        else if(lineData[0].find("IMGLocLg") != std::string::npos)
        {
            m_InvSprite.LoadFromTexture(
                CAssetManager::Create<asset::CTexture>(
                lineData[1].c_str()));
        }
        else if(lineData[0].find("IMGLocPj") != std::string::npos)
        {
            mp_projectile_texture = CAssetManager::Create<asset::CTexture>(
                lineData[1].c_str());
        }
        else if(lineData[0].find("Damage") != std::string::npos)
        {
            m_DAMAGE = atoi(lineData[1].c_str());
        }
        else if(lineData[0].find("ClipSize") != std::string::npos)
        {
            m_CLIP_SIZE = atoi(lineData[1].c_str());
        }
        else if(lineData[0].find("ReloadDelay") != std::string::npos)
        {
            m_RELOAD_DELAY = atoi(lineData[1].c_str());
        }
        else if(lineData[0].find("FiringDelay") != std::string::npos)
        {
            m_FIRING_DELAY = atoi(lineData[1].c_str());
        }
    }

    return true;
}

bool CWeapon::Fire()
{
    if(m_fire_delay > 0 || m_reload_delay > 0 || 
        (m_ammo_count <= 0 && m_current_clip <= 0))
        return false;
    else if(m_current_clip == 0)
    {
        m_reload_delay = m_RELOAD_DELAY;
        mp_ReloadSound->Play();
        return false;
    }

    m_current_clip--;
    m_fire_delay = m_FIRING_DELAY;
    mp_FireSound->Play();
    return true;
}

void CWeapon::Update()
{
    if(m_fire_delay > 0) m_fire_delay--;
    if(m_reload_delay > 0)
    {
        m_reload_delay--;
        if(m_reload_delay == 0)
        {
            m_ammo_count  -= m_CLIP_SIZE;
            m_current_clip = m_CLIP_SIZE;            
        }
    }

    m_OnTankSprite.Update();
}

void CWeapon::SetClipCount(const u_int count)
{
    m_ammo_count    = count * m_CLIP_SIZE;
    m_current_clip  = m_CLIP_SIZE;
    m_ammo_count    -= m_current_clip;
}

int CWeapon::GetAmmoCount() const
{
    return m_ammo_count;
}

int CWeapon::GetClipCount() const
{
    return m_current_clip;
}

const std::string& CWeapon::GetName() const
{
    return m_weapon_name;
}

const asset::CTexture* CWeapon::GetProjectileTexture() const
{
    return mp_projectile_texture;
}

u_int CWeapon::GetDamage() const
{
    return m_DAMAGE;
}
