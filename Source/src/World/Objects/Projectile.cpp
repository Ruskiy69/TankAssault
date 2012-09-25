#include "World/Objects/Projectile.hpp"

using obj::CProjectile;

CProjectile::CProjectile(const float life) : m_damage(0),
    m_lifetime(life) {}

/**
 * Fires the bullet toward a location. Determines the unit velocity
 * vector based on the current location and the provided target location.
 *
 * @param float Starting x-coordinate
 * @param float Starting y-coordinate
 * @param float Target x-coordinate
 * @param float Target y-coordinate
 **/
void CProjectile::Launch(const float start_x, const float start_y,
    const float end_x, const float end_y)
{
    // v means velocity vector (vx, vy)
    // x1 is the destination vector (x1, y1)

    // x0 is the shot location vector (x0, y0)
    // c is the speed
    // Then:
    //      v   = c * (x1 - x0) / ||x1 - x0||
    // So:
    //      vx  = c * (dest_x - start_x) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
    //      vy  = c * (dest_y - start_y) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)

    math::CVector2 Strt(start_x, start_y);
    math::CVector2 Dest(end_x, end_y);
    
    this->Launch(Strt, Dest);
}

void CProjectile::Launch(const math::CVector2& Start, 
    const math::CVector2& Target)
{
    // v means velocity vector (vx, vy)
    // x1 is the destination vector (x1, y1)
    // x0 is the shot location vector (x0, y0)
    // c is the speed
    // Then:
    //      v   = c * (x1 - x0) / ||x1 - x0||
    // So:
    //      vx  = c * (dest_x - start_x) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
    //      vy  = c * (dest_y - start_y) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)

    this->Move(Start);
    m_Rate = ((Target - Start) / 
        (Target - Start).Magnitude()) * 
        CProjectile::DEFAULT_PROJ_SPD;
}

/**
 * Renders the bullet on-screen and updates its position.
 * @pre A bullet image has been loaded.
 **/
void CProjectile::Update()
{
    if(m_lifetime > 0.0f)
    {
        this->Move_Rate(m_Rate);
        this->CGameObject::Update();
    }
}

void CProjectile::SetLifetime(const float value)
{
    m_lifetime = value;
}

void CProjectile::SetDamage(const u_int dmg)
{
    m_damage = dmg;
}

u_int CProjectile::GetDamage() const
{
    return m_damage;
}

float CProjectile::GetLifetime() const
{
    return m_lifetime;
}
