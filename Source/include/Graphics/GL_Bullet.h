#ifndef GL_BULLET_H
#define GL_BULLET_H

#include <vector>

#include "Math/MathHelper.h"
#include "Graphics/GFX_Base.h"
#include "Graphics/GL_Entity.h"

namespace Game
{
    class GL_Bullet : public GFX::GL_Entity
    {
    public:
        GL_Bullet(const int x, const int y);
        GL_Bullet(const MathHelper::Vector2& Position);
        ~GL_Bullet(){}

        void RicochetHorizontal();
        void RicochetVertical();
        void Update();

    private:
        static const int BULLET_SPEED = 16;

        /* Bullet target */
        int dest_x, dest_y;

        /* Bullet destination, position */
        float unit_vector;
        float dx, dy;
    };

    /* Makes it easier to have a container of CBullets. */
    typedef std::vector<GL_Bullet*> GL_Bullets;
}

#endif // GL_BULLET_H