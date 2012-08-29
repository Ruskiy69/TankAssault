/**
 * @file
 *  Definitions for the Obj_Bullet class.
 *
 * @author George Kudrayvtsev
 * @version 1.2
 *
 * @addtogroup Objects
 */
/// @{

#ifndef BULLET_H
#define BULLET_H

#include <list>

#include "Shader.h"
#include "Assets/TextureAssetManager.h"

namespace obj
{
    /**
     * A base class for any type of weapon. Handles motion,
     * target location, and updating. The only thing necessary
     * for the inheriting classes is to load the bullet image
     * and set the damage.
     */
    class Obj_Bullet : public asset::GL_Entity
    {
    public:
        Obj_Bullet(gfx::GL_Shader* p_VShader, gfx::GL_Shader* p_FShader);
        Obj_Bullet(const float x, const float y);
        Obj_Bullet(const math::ML_Vector2& Start_Position);
        Obj_Bullet();

        void Fire(const float x, const float y);
        void Fire(const int x, const int y);
        void Fire(const math::ML_Vector2& Target);

        void Update();

        void SetStartPosition(const math::ML_Vector2& Pos);
        void SetDamage(const u_int dmg);
        u_int GetDamage() const;

    protected:
        u_int m_damage;

    private:
        static const int DEFAULT_BULLET_SPEED = 16;
        math::ML_Vector2 m_Rate;
    };

    /// Easily create a container of bullets
    typedef std::list<obj::Obj_Bullet*> Obj_pBullets;
}

#endif // BULLET_H

/// @}