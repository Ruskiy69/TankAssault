#ifndef __GL_ENTITY_H__
#define __GL_ENTITY_H__

#include <vector>

#include "SDL_GL_Base.h"
#include "GL_Display.h"

namespace gk_gl
{
    class CGL_Entity
    {
    public:
        CGL_Entity();
        ~CGL_Entity();

        /* Load the entity texture from a filename */
        void LoadEntity(const char* filename);
        void LoadEntity(const std::string& filename);

        /* Set a previously created entity as the texture */
        void SetEntity(const unsigned int texture, 
            const int w, const int h);
        void SetEntity(SDL_Surface* Source);

        /* Animation methods */
        void AddAnimation(SDL_Surface* Anim);
        void AddAnimation(const unsigned int texture);
        void SetAnimationRate(const unsigned int rate);

        /* Move the entity to a location, or adjust it
         * by some rate.
         */
        void Move(const float x, const float y);
        void Move(const GL_Vertex2f& Position);
        void Move_Rate(const float dx, const float dy);
        void Move_Rate(const GL_Vertex2f& Rate);

        /* Resize the entity */
        void Resize(const unsigned int w, const unsigned int h);

        /* Set the entity to a certain degree
         * rotation, or adjust it by a rate.
         */
        void Rotate(const float degrees);
        void Rotate_Rate(const float degrees);

        /* Is the entity off-screen? */
        bool IsOffscreen(const CGL_Display& Display) const;

        /* Collision detection with other objects, or locations */
        bool CheckCollision(const CGL_Entity& Entity) const;
        bool CheckCollision(const GL_Rect& Box) const;
        bool CheckCollision(const GL_Pixel& Pixel) const;
        bool CheckCollision(const std::vector<GL_Rect>& Boxes) const;
        bool CheckCollision(const std::vector<GL_Pixel>& Pixels) const;
        bool CheckCollision(const std::vector<GL_Pixel*>& Pixels) const;
        bool CheckCollision(const int x, const int y) const;

        /* Various get/set attributes */
        float GetX() const;
        float GetY() const;
        int   GetW() const;
        int   GetH() const;

        gk_gl::gl_texture   GetTexture() const;

        const GL_Vertex2f&  GetPosition() const;
        const GL_Rect&      GetDimensions() const;         // <----\___ Same thing
        const GL_Rect&      GetCollisionBox() const;       // <----/


        /* Render the entity on-screen */
        virtual void Update();

    protected:
        virtual void Render() const;
        virtual void Animate();

        GL_Vertex2f Position;
        GL_Rect CollisionBox;

        std::vector<unsigned int> Animations;

        unsigned int texture;
        float rotation;
        int anim_rate;
    };
}

#endif // __GL_ENTITY_H__
