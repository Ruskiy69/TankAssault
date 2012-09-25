/**
 * @file
 *  Implementation of the CPlayer class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 **/
#include "World/Objects/Player.hpp"

using obj::CPlayer;
using asset::CAssetManager;


CPlayer::CPlayer() : m_kills(0), m_survivors_saved(0), m_days_survived(1){}

/**
 * Initializes values for the class.
 *  The main sprite image is loaded here, which resides in "Data/Images"
 *  for the time being. Since there are multiple sprites in the sheet,
 *  they are each cut out, and the GL_Entity is resized to 64x64 to
 *  exclusively accompany the first sprite. Then, in the Update() method,
 *  only half of the whole texture is rendered, properly displaying the
 *  sprite in use.
 **/
bool CPlayer::Init(game::CSettings& Settings)
{
    // Load tank base images
    SDL_Surface* pSprite_Sheet = gfx::load_image(
        Settings.GetValueAt("PlayerIMG1").c_str());
    m_Tank.LoadFromSurface(pSprite_Sheet);
    m_Tank.ResizeCollisionBox(64, 64);
    m_Tank.ResizeTexture(64, 64);
    m_Tank.SetRenderDimensions(math::CRectf(0, 0, 0.5f, 1.0f));
    m_Tank.Move(0, 0);
    SDL_FreeSurface(pSprite_Sheet);

    // Load tank tower image
    /// @todo Fix animations.
    m_Tower.LoadFromTexture(CAssetManager::Create<asset::CTexture>(
        Settings.GetValueAt("PlayerIMG2").c_str()));
    m_Tower.Move(0, 0);

    if(!(m_Weapon1.Init("Data/Data Files/hrl.cwp") &&
        m_Weapon2.Init("Data/Data Files/lpmg.cwp")))
        return false;

    m_Weapon1.SetClipCount(10);
    m_Weapon2.SetClipCount(100);

    return true;
}

/**
 * Render the tank and the tower on-screen.
 * @todo Animate tank tower on firing.
 **/
int CPlayer::Update()
{
    return CTank::Update();
}

void CPlayer::SetSpawn(const math::CVector2& Pos)
{
    m_Tank.Move(Pos);
    m_Tower.Move(Pos);
}

void CPlayer::IncreaseKillCount()
{
    ++m_kills;
}

void CPlayer::IncreaseSurvivorCount()
{
    ++m_survivors_saved;
}

void CPlayer::IncreaseDayCount()
{
    ++m_days_survived;
}

u_int CPlayer::GetKills() const
{
    return m_kills;
}

u_int CPlayer::GetSurvivorsSaved() const
{
    return m_survivors_saved;
}

u_int CPlayer::GetDaysSurvived() const
{
    return m_days_survived;
}

u_int CPlayer::GetTowerHealth() const
{
    return m_Tower.GetHealth();
}

u_int CPlayer::GetTankHealth() const
{
    return m_Tank.GetHealth();
}
