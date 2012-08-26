/**
 * @file
 *  Implementation of the Obj_Bullet base class.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 */

#include "World/Objects/Bullet.h"

using obj::Obj_Bullet;

/**
 * Creates a bullet and moves it to a starting position.
 * The starting position should (theoretically) be the location
 * of the barrel on whatever is shooting the bullet.
 *
 * @param float Starting x-coordinate
 * @param float Starting y-coordinate
 */
Obj_Bullet::Obj_Bullet(const float x, const float y)
{
    this->Move(x, y);
}

/**
 * @overload Obj_Bullet::Obj_Bullet(const float, const float)
 *
 * @param math::Vector Starting position
 */
Obj_Bullet::Obj_Bullet(const math::ML_Vector2& Start_Pos)
{
    this->Move(Start_Pos);
}

/**
 * Fires the bullet toward a location. Determines the unit veclocity
 * vector based on the current location and the provided target location.
 *
 * @param float Target x-coordinate
 * @param float Target y-coordinate
 */
void Obj_Bullet::Fire(const float x, const float y)
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

    math::ML_Vector2 Dest(x, y);
    
    m_Rate = ((Dest - this->GetPosition()) / (Dest - this->GetPosition()).Magnitude()) * Obj_Bullet::DEFAULT_BULLET_SPEED;
}

/**
 * @overload Obj_Bullet::Fire()
 *
 * @param math::ML_Vector2 Target location
 */
void Obj_Bullet::Fire(const math::ML_Vector2& Target)
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

    m_Rate = ((Target - this->GetPosition()) / (Target - this->GetPosition()).Magnitude()) * Obj_Bullet::DEFAULT_BULLET_SPEED;
}

/**
 * Renders the bullet on-screen and updates its position.
 *
 * @pre A bullet image has been loaded.
 */
void Obj_Bullet::Update()
{
    this->Move_Rate(m_Rate);
    this->GL_Entity::Update();
}

/**
 * Sets the bullet damage.
 */
void Obj_Bullet::SetDamage(const u_int dmg)
{
    m_damage = dmg;
}

/**
 * Retrieves the bullet damage.
 */
u_int Obj_Bullet::GetDamage() const
{
    return m_damage;
}

Obj_Bullet::Obj_Bullet()
{}

void Obj_Bullet::SetStartPosition(const math::ML_Vector2& Pos)
{
    this->Move(Pos);
}