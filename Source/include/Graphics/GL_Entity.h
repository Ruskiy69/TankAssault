#ifndef GL_ENTITY_H
#define GL_ENTITY_H

#include <vector>

#include "GFX_Base.h"
#include "GL_Display.h"

namespace GFX
{
    class GL_Entity
    {
    public:
        GL_Entity();
        ~GL_Entity();
        
        /* Load the entity texture from a filename */
        void LoadEntity(const char* filename);
        void LoadEntity(const std::string& filename);

        /* Set a previously created entity as the texture */
        void SetEntity(SDL_Surface* Source);

        /* Animation methods */
        void AddAnimation(SDL_Surface* Anim);
        void AddAnimation(const unsigned int texture);
        void SetAnimationRate(const unsigned int rate);

        /* Move the entity to a location, or adjust it
         * by some rate.
         */
        void Move(const float x, const float y);
        void Move(const MathHelper::Vector2& Position);
        void Move_Rate(const float dx, const float dy);
        void Move_Rate(const MathHelper::Vector2& Rate);

        /* Resize the entity */
        void Resize(const unsigned int w, const unsigned int h);
        void Resize(const MathHelper::Rect& Size);

        /* Set the entity to a certain degree
         * rotation, or adjust it by a rate.
         */
        void Rotate(const float degrees);
        void Rotate_Rate(const float degrees);

        /* Is the entity off-screen? */
        bool IsOffscreen(const GL_Display& Display) const;

        /* Collision detection with other objects, or locations */
        bool CheckCollision(const GL_Entity& Entity) const;
        bool CheckCollision(const MathHelper::Rect& Box) const;
        bool CheckCollision(const MathHelper::Rects& Boxes) const;
        bool CheckCollision(const int x, const int y) const;

        /* Various get/set attributes */
        float GetX() const;
        float GetY() const;
        int   GetW() const;
        int   GetH() const;
        
        gl_texture  GetTexture() const;

        const MathHelper::Vector2&  GetPosition() const;
        const MathHelper::Rect&     GetCollisionBox() const;

        /* Render the entity on-screen */
        virtual void Update();

    protected:
        virtual void Render() const;
        virtual void Animate();

        MathHelper::Vector2 Position;
        MathHelper::Rect    Collision_Box;

        std::vector<gl_texture> Animations;

        gl_texture texture;
        float rotation;
        int anim_rate;
    };
}

#endif // __GL_ENTITY_H__
