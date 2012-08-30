/**
 * @file
 *  Declarations for the Obj_Tank class.
 *
 * @author George Kudrayvtsev
 * @version 1.4.0
 *
 * @addtogroup Objects
 * A collection of classes that represent most objects
 * that appear on-screen. This can be anything ranging from enemies
 * to power-ups to items to the player. Though, there are several
 * things that appear on-screen that do not fall into this category.
 * These include the various maps, the HUD system, the inventory,
 * and menu screens.
 */
/// @{

#ifndef TANK_H
#define TANK_H

#include "CollapseDef.h"
#include "Assets/AssetManagers.h"

/// Namespace containing most in-game objects
namespace obj
{
    /// An base class for all in-game tanks.
    class Obj_Tank
    {
    public:
        Obj_Tank();
        virtual ~Obj_Tank(){}

        virtual bool Init();

        void Turn(const float degree);

        void Drive(const float speed);
        void Adjust(const math::ML_Vector2& Rate);

        void Aim(const int x, const int y);
        void Aim(const float x, const float y);
        void Aim(const math::ML_Vector2& Target);

        void RotateTower(const float degrees);

        bool FirePrimary();
        bool FireSecondary();

        void Damage(const int damage_points);

        virtual int Update();

        bool IsAlive();
        bool IsMoving();

        const math::ML_Vector2& GetBarrelPosition() const;
        const math::ML_Vector2& GetTankPosition() const;
        const math::ML_Rect& GetCollisionBox() const;
        const int GetPrimaryAmmoCount() const;

        static const int DEFAULT_TANK_SPEED = 3;

    protected:
        static const int ROCKET_DELAY = 60; // Delay for rocket shots
        static const int MG_DELAY = 5;      // Delay for default weapon, laser machine gun

        /// Indices for Obj_Tank::shotSounds
        enum { e_LASERSOUND = 0, e_ROCKETSOUND };

        // Sound effects for various weapon launches
        std::vector<asset::asset_id> m_shotSounds;

        asset::GL_Entity* mp_Tank;    // Two separate entities so that
        asset::GL_Entity* mp_Tower;   // They can rotate independently

        math::ML_Vector2 m_MovementRate;

        // The shots have to always come from the barrel, 
        // so I have to dedicate a way to keep track of it
        math::ML_Vector2 m_BarrelPosition;

        // Delays for weapon firing
        int m_primary_shot_delay, m_secondary_shot_delay;

        // Health and ammo count
        int m_health, m_primary_ammo;

        // Action tracking
        bool m_moving, m_shooting_primary, m_shooting_secondary;
    };
}

#endif // TANK_H

/// @}