#include "World/Objects/Tank.hpp"

using obj::CTank;
using game::g_Log;
using asset::CAssetManager;

CTank::CTank() : m_MovementRate(0.0f, 1.0f),
    m_BarrelPosition(330.0f, 300.0f), m_moving(false) {}

CTank::~CTank(){}

/**
 * Rotates the tank body.
 * @param float The amount of degrees to rotate the body
 *
 * @see CTank::Aim()
 * @bug The cos() having +math::PI normally shouldn't be there.
 **/
void CTank::Turn(const float deg)
{
    m_Tank.Rotate_Rate(deg);

    // Use trigonometry to determine new movement rate.
    float dx = sin(math::rad(m_Tank.GetRotationAngle()));
    float dy = cos(math::rad(m_Tank.GetRotationAngle()) + math::PI);
    m_MovementRate.Move(dx, dy);
}

/**
 * Drives the tank.
 *  The tank moves in a certain direction based on the current rotation.
 *
 * @param float The speed at which to drive.
 *  A negative speed will move the tank backwards, positive speed moves it forwards.
 **/
void CTank::Drive(const float speed)
{
    if(speed == 0.0f)
        m_moving = false;
    else
    {
        m_moving = true;
        m_Tank.Move_Rate(m_MovementRate * speed);
        m_Tower.Move_Rate(m_MovementRate * speed);
    }
}

/**
 * Adjusts the tank position.
 * @param math::CVector2 Tank adjustment rate
 **/
void CTank::Adjust(const math::CVector2& Rate)
{
    m_Tank.Move_Rate(Rate);
    m_Tower.Move_Rate(Rate);
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
 * @see math::CVector2::Rotate(float)
 *
 * @todo Actually use math::CMatrix
 * @bug When the point to aim at is extremely close to the tank body, the angles
 *  aren't calculated properly.
 * @bug In the atan2() sequence, the + 90.0f is there for an unknown reason.
 **/
void CTank::Aim(const float x, const float y)
{
    // Triangles eh? Take the arctangent to determine the shot angle (in degrees)
    // The plus 180 at the end is necessary, though I forgot to add a comment explaining why...
    // I think it's due to the fact that the tower is stored facing down in the .png image.
    float angle = math::deg(atan2(m_Tower.GetY() - y, m_Tower.GetX() - x)) + 90;

    // A tiny optimization, I guess, so that the new barrel isn't calculated.
    // Yes, I'm aware of floating point inaccuracies.
    if(m_Tower.GetRotationAngle() == angle)
        return;

    this->RotateTower(angle - m_Tower.GetRotationAngle());
}

/// @overload CTank::Aim(float, float)
void CTank::Aim(const math::CVector2& Target)
{
    this->Aim(Target.x, Target.y);
}

/**
 * Rotates the tank tower.
 * @param float Degrees to rotate.
 **/
void CTank::RotateTower(const float deg)
{
    // Rotate the tower entity
    m_Tower.Rotate_Rate(deg);

    // Convert to radians for sin and cos in math::CVector2::Rotate() to function.
    float rad = -math::rad(m_Tower.GetRotationAngle());

    /**
     * Rotating the barrel:
     *  Adjust the barrel location by rotating it around the center of the tank body.
     *  The steps are as follows:
     *  Subtract the origin from the rotating point.
     *  Rotate around (0, 0)
     *  Add the original origin.
     **/
    
    // Find the center of the tank, this is the origin.
    float center_x = m_Tank.GetX() + (m_Tank.GetW() / 2);
    float center_y = m_Tank.GetY() + (m_Tank.GetH() / 2);

    // Reset the barrel position to the top point.
    m_BarrelPosition.Move(center_x - 2, center_y + 32);

    // Subtract the center from the barrel.
    math::CVector2 Center(center_x, center_y);
    math::CVector2 Final = m_BarrelPosition - Center;

    // Rotate
    Final.Rotate(rad);

    // Add the center back to the barrel position.
    Final = Final + Center;
    m_BarrelPosition = Final;
}

/**
 * Damages the tank.
 * @param u_int Damage points
 **/
void CTank::Damage(const u_int dmg)
{
    /// @todo Find where the damage should go
    m_Tank.Damage(dmg);
}

/**
 * Adds a weapon to the tank body.
 * @param WeaponType The type of weapon to add.
 * @return TRUE if weapon added, FALSE if not.
 *  The tank may only have 2 weapons on it at a time.
 **/
bool CTank::AddWeapon(const std::string& data_file)
{
    return m_Weapon1.Init(data_file);
}

bool CTank::FirePrimary()
{
    return m_Weapon1.Fire();
}

bool CTank::FireSecondary()
{
    return m_Weapon2.Fire();
}

bool CTank::IsAlive() const
{
    return (m_Tank.IsAlive() && m_Tower.IsAlive());
}

/**
 * Checks if the tank is in motion.
 * @return Moving or not?
 **/
bool CTank::IsMoving() const
{
    return m_moving;
}

/**
 * Updates weapons and tank entities.
 **/
int CTank::Update()
{
    m_Tank.Update();
    m_Tower.Update();
    m_Weapon1.Update();
    m_Weapon2.Update();
    return 0;
}

/**
 * Retrieves barrel position.
 * @return Barrel position.
 **/
const math::CVector2& CTank::GetBarrelPosition() const
{
    return m_BarrelPosition;
}

/**
 * Retrieves tank location.
 * @return Location.
 **/
const math::CVector2& CTank::GetPosition() const
{
    return m_Tank.GetPosition();
}

/**
 * Retrieves the tank collision box.
 * @return Collision box.
 **/
const math::CRect& CTank::GetCollisionBox() const
{
    return m_Tank.GetCollisionBox();
}

obj::CWeapon& CTank::GetPrimary()
{
    return m_Weapon1;
}

obj::CWeapon& CTank::GetSecondary()
{
    return m_Weapon2;
}

obj::CGameObject* CTank::GetTankEntity()
{
    return &m_Tank;
}

obj::CGameObject* CTank::GetTowerEntity()
{
    return &m_Tower;
}
