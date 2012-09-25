/**
 * @file
 *  Implementation of the CBullet base class.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 */

#include "World/Objects/Bullet.hpp"

using obj::CBullet;

CBullet::CBullet() : m_damage(0) {}

/**
 * Creates a bullet and moves it to a starting position.
 * The starting position should (theoretically) be the location
 * of the barrel on whatever is shooting the bullet.
 *
 * @param float Starting x-coordinate
 * @param float Starting y-coordinate
 */
CBullet::CBullet(const float x, const float y)
{
    this->Move(x, y);
}

/**
 * @overload CBullet::CBullet(const float, const float)
 *
 * @param math::Vector Starting position
 */
CBullet::CBullet(const math::CVector2& Start_Pos)
{
    this->Move(Start_Pos);
}

/**
 * Fires the bullet toward a location. Determines the unit velocity
 * vector based on the current location and the provided target location.
 *
 * @param float Target x-coordinate
 * @param float Target y-coordinate
 */
void CBullet::Fire(const float x, const float y)
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

    math::CVector2 Dest(x, y);
    
    m_Rate = ((Dest - this->GetPosition()) / 
        (Dest - this->GetPosition()).Magnitude()) * 
        CBullet::DEFAULT_BULLET_SPEED;
}

/**
 * @overload CBullet::Fire()
 *
 * @param math::CVector2 Target location
 */
void CBullet::Fire(const math::CVector2& Target)
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

    m_Rate = ((Target - this->GetPosition()) / 
        (Target - this->GetPosition()).Magnitude()) * 
        CBullet::DEFAULT_BULLET_SPEED;
}

/**
 * Renders the bullet on-screen and updates its position.
 * @pre A bullet image has been loaded.
 */
void CBullet::Update()
{
    this->Move_Rate(m_Rate);
    this->CEntity::Update();
}

/**
 * Sets the bullet damage.
 */
void CBullet::SetDamage(const u_int dmg)
{
    m_damage = dmg;
}

/**
 * Retrieves the bullet damage.
 */
u_int CBullet::GetDamage() const
{
    return m_damage;
}

void CBullet::SetStartPosition(const math::CVector2& Pos)
{
    this->Move(Pos);
}