/**
 * @file
 *  Implementation of the Obj_Tank class.
 *
 * @author George Kudrayvtsev
 * @version 1.2.8
 */

#include "World/Objects/BaseTank.h"

using obj::Obj_Tank;
using game::g_Log;

using asset::g_AudioAssets;
using asset::g_TextureAssets;

/**
 * Initialize all values and load sound effects.
 *
 * If any of the sound effects fail to load, gk::handle_error()
 * is called, quitting the application.
 */
Obj_Tank::Obj_Tank() : m_MovementRate(0, 1), m_BarrelPosition(300.0f + 32.0f, 300.0f),
    m_primary_shot_delay(0), m_secondary_shot_delay(0),
    m_health(100), m_primary_ammo(100), m_moving(false) {}

bool Obj_Tank::Init()
{
    asset::asset_id laser_id = g_AudioAssets.LoadAudioFromFile("Data/Sounds/LaserFire.wav");
    asset::asset_id rocket_id = g_AudioAssets.LoadAudioFromFile("Data/Sounds/RocketFire.wav");

    g_AudioAssets.GetAudioByID(laser_id)->SetPosition(m_BarrelPosition);
    g_AudioAssets.GetAudioByID(rocket_id)->SetPosition(m_BarrelPosition);

    m_shotSounds.push_back(laser_id);
    m_shotSounds.push_back(rocket_id);

    return true;
}

/**
 * Rotate the tank body.
 * @param float The amount of degrees to rotate the body.
 *
 * @see Obj_Tank::Aim()
 */
void Obj_Tank::Turn(const float degree)
{
    mp_Tank->Rotate_Rate(degree);

    // Some trig magic to determine new movement speed
    float dx = sin(math::rad(mp_Tank->GetRotation()));
    float dy = cos(math::rad(mp_Tank->GetRotation()));
    m_MovementRate.Move(dx, dy);
}

/**
 * Drive the tank.
 *
 * The tank moves in a certain direction based on the current rotation.
 *
 * @param float The speed at which to drive.
 *  A negative speed will move the tank backwards, positive speed moves it forwards.
 */
void Obj_Tank::Drive(const float speed)
{
    if(speed != 0.0f)
    {
        m_moving = true;
        
        mp_Tank->Move_Rate(m_MovementRate * speed);
        mp_Tower->Move_Rate(m_MovementRate * speed);
    }
    else
        m_moving = false;
}

/**
 * Adjust the tank position.
 *
 * @param math::ML_Vector2 The rate at which to adjust the tank.
 */
void Obj_Tank::Adjust(const math::ML_Vector2& Rate)
{
    mp_Tank->Move_Rate(Rate);
    mp_Tower->Move_Rate(Rate);
}

/**
 * Aim the tank barrel.
 *
 * in accordance with this one. Angle toward target is determined, followed by
 * multiplication by a rotation matrix (ish) to track barrel location.
 *
 * @param float X-coordinate to aim at
 * @param float Y-coordinate to aim at
 *
 * @see http://www.euclideanspace.com/maths/geometry/affine/aroundPoint/matrix2d/index.htm
 * @see http://www.gamedev.net/topic/460026-rotate-point-around-origin-by-angle/
 * @see math::ML_Vector2::Rotate(float)
 *
 * @todo Actually use math::Matrix
 * @bug When the point to aim at is extremely close to the tank body, the angles
 *  aren't calculated properly.
 */
void Obj_Tank::Aim(const float x, const float y)
{
    // Triangles eh? Take the arctangent to determine the shot angle (in degrees)
    // The plus 180 at the end is necessary, though I forgot to add a comment explaining why...
    // I think it's due to the fact that the tower is stored facing down in the .png image.
    float angle = math::deg(atan2(mp_Tower->GetX() - x, mp_Tower->GetY() - y)) + 180;

    // A tiny optimization, I guess, so that the new barrel isn't calculated.
    // Yes, I'm aware of floating point inaccuracies.
    if(mp_Tower->GetRotation() == angle)
        return;
    
    mp_Tower->Rotate(angle);

    /**
     * Rotating the barrel:
     *  Adjust the barrel location by rotating it around the center of the tank body.
     *  The steps are as follows:
     *  Subtract the origin from the rotating point.
     *  Rotate around (0, 0)
     *  Add the original origin.
     */

    // Convert to radians for sin and cos in math::ML_Vector2::Rotate() to function.
    angle = math::rad(angle - 180.0f);
    
    // Find the center of the tank, this is the origin.
    float center_x = mp_Tank->GetX() + (mp_Tank->GetW() / 2);
    float center_y = mp_Tank->GetY() + (mp_Tank->GetH() / 2);

    // Reset the barrel position to the top point.
    m_BarrelPosition.Move(mp_Tank->GetX() - 2 + mp_Tank->GetW() / 2, mp_Tank->GetY());

    // Subtract the center from the barrel.
    math::ML_Vector2 Center(center_x, center_y);
    math::ML_Vector2 Final = m_BarrelPosition - Center;

    // Rotate
    Final.Rotate(angle);

    // Add the center back to the barrel position.
    Final = Final + Center;

    m_BarrelPosition.Move(Final);
}

/**
 * @overload Obj_Tank::Aim(float, float)
 */
void Obj_Tank::Aim(const int x, const int y)
{
    this->Aim(x * 1.0f, y * 1.0f);
}

/**
 * @overload Obj_Tank::Aim(float, float)
 */
void Obj_Tank::Aim(const math::ML_Vector2& Target)
{
    this->Aim(Target.x, Target.y);
}

/**
 * Rotates the tank tower.
 *
 * @param float Degrees to rotate.
 */
void Obj_Tank::RotateTower(const float degrees)
{
    mp_Tower->Rotate_Rate(degrees);
    float angle = math::rad(mp_Tower->GetRotation());

    // Find the center of the tank, this is the origin.
    float center_x = mp_Tank->GetX() + (mp_Tank->GetW() / 2);
    float center_y = mp_Tank->GetY() + (mp_Tank->GetH() / 2);

    // Reset the barrel position to the top point.
    m_BarrelPosition.Move(mp_Tank->GetX() - 2 + mp_Tank->GetW() / 2,
        mp_Tank->GetY() + 64);

    // Subtract the center from the barrel.
    math::ML_Vector2 Center(center_x, center_y);
    math::ML_Vector2 Final = m_BarrelPosition - Center;

    // Rotate
    Final.Rotate(angle);

    // Add the center back to the barrel position.
    Final = Final + Center;

    m_BarrelPosition.Move(Final);
}

/**
 * Check primary weapon firing ability.
 *
 * The ability to fire is determined based on ammo count, and the DELAY
 * variable in Obj_Tank that corresponds with the currently equipped
 * primary weapon. It assigns the internal Obj_Tank::shooting_primary
 * variable to whether or not the tank is shooting. This assumes that the
 * caller will actually shoot if the tank can. Also, play the firing sound
 * of the primary weapon.
 *
 * @return TRUE if the tank can fire its primary weapon, FALSE if it cannot.
 *
 * @todo Check for ammo type
 */
bool Obj_Tank::FirePrimary()
{
    if(m_primary_shot_delay <= 0)
    {
        if(m_primary_ammo == -10)   // Infinite ammo
            g_AudioAssets.GetAudioByID(
                m_shotSounds[Obj_Tank::e_ROCKETSOUND])->Play();
        else if(m_primary_ammo > 0) // Some amount of ammo left
            m_primary_ammo--;
        else    // No ammo lol
        {
            m_shooting_primary = false;
            return false;
        }

        g_AudioAssets.GetAudioByID(
            m_shotSounds[Obj_Tank::e_ROCKETSOUND])->Play();
        m_primary_shot_delay = Obj_Tank::ROCKET_DELAY;
        m_shooting_primary = true;
        return true;
    }

    return false;
}

/**
 * Checks the secondary weapon, a laser machine gun, firing ability.
 *
 * The ability to fire is determined via the Obj_Tank::MG_DELAY variable.
 * This also sets the internal Obj_Tank::shooting_secondary variable to
 * the return value. So, this assumes the caller will actually shoot if
 * the tank is able to. Also, play the firing sound of the laser.
 *
 * @return TRUE if the tank can fire its machine gun, FALSE if it cannot.
 */
bool Obj_Tank::FireSecondary()
{
    if(m_secondary_shot_delay <= 0)
    {
        g_AudioAssets.GetAudioByID(
            m_shotSounds[Obj_Tank::e_LASERSOUND])->Play();
        m_secondary_shot_delay = Obj_Tank::MG_DELAY;
        m_shooting_secondary = true;
    }
    else
        m_shooting_secondary = false;

    return m_shooting_secondary;
}

/**
 * Do damage to the tank, reducing health.
 */
void Obj_Tank::Damage(const int damage_pts)
{
    m_health -= damage_pts;
}

/**
 * Updates tower and tank on-screen, recommended to be over-written.
 */
int Obj_Tank::Update()
{
    if(m_secondary_shot_delay > 0)
        m_secondary_shot_delay--;
    if(m_primary_shot_delay > 0)
        m_primary_shot_delay--;

    mp_Tank->Update();
    mp_Tower->Update();

    return 1;
}

/**
 * Checks if the tank is alive, based on Obj_Tank::health.
 *
 * @return TRUE if health is greater than 0, FALSE otherwise.
 */
bool Obj_Tank::IsAlive()
{
    return (m_health > 0);
}

/**
 * Checks if the tank is in motion, based on the Obj_Tank::moving variable.
 *
 * @return TRUE if in motion, FALSE otherwise.
 *
 * @see Obj_Tank::Drive()
 */
bool Obj_Tank::IsMoving()
{
    return m_moving;
}

/**
 * Retrieves barrel position.
 *
 * @return Barrel position.
 */
const math::ML_Vector2& Obj_Tank::GetBarrelPosition() const
{
    return m_BarrelPosition;
}

/**
 * Retrieves tank location.
 *
 * @return Location.
 */
const math::ML_Vector2& Obj_Tank::GetTankPosition() const
{
    return mp_Tank->GetPosition();
}

/**
 * Retrieves the tank collision box.
 *
 * @return Collision box.
 */
const math::ML_Rect& Obj_Tank::GetCollisionBox() const
{
    return mp_Tank->GetCollisionBox();
}

/**
 * Returns the primary ammo count.
 *
 * There is no Obj_Tank::GetSecondaryAmmoCount() because the player
 * has an infinite amount of laser machine gun battery power.
 *
 * @return Primary ammo count.
 */
const int Obj_Tank::GetPrimaryAmmoCount() const
{
    return m_primary_ammo;
}