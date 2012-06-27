#include "Graphics/GL_Bullet.h"

using Game::GL_Bullet;

GL_Bullet::GL_Bullet(const int x, const int y)
{
    /* Force the bullet to move to the given (x, y)
     * location, which should technically be the
     * "barrel" of whatever is shooting.
     */
    this->Move(x * 1.0f, y * 1.0f);

    /* Load the image for the bullet */
    this->LoadEntity("Data/Images/Player_Shot.png");

    /* Acquire target */
    SDL_GetMouseState(&this->dest_x, &this->dest_y);

    /* v means velocity vector (vx, vy)
     * x1 is the destination vector (x1, y1)
     * x0 is the shot location vector (x0, y0)
     * c is the speed
     * Then:
     *      v   = c * (x1 - x0) / ||x1 - x0||
     * So:
     *      vx  = c * (dest_x - start_x) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     *      vy  = c * (dest_y - start_y) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     */

    this->unit_vector   = sqrt(1.0f * ((dest_x - x)*(dest_x - x) + (dest_y - y)*(dest_y - y)));
    this->dx            = BULLET_SPEED * (dest_x - x) / this->unit_vector;
    this->dy            = BULLET_SPEED * (dest_y - y) / this->unit_vector;
}

GL_Bullet::GL_Bullet(const MathHelper::Vector2& Pos)
{
    this->Move(Pos);

    /* Load the image for the bullet */
    this->LoadEntity("Data/Images/Player_Shot.png");

    /* Acquire target */
    SDL_GetMouseState(&this->dest_x, &this->dest_y);

    /* v means velocity vector (vx, vy)
     * x1 is the destination vector (x1, y1)
     * x0 is the shot location vector (x0, y0)
     * c is the speed
     * Then:
     *      v   = c * (x1 - x0) / ||x1 - x0||
     * So:
     *      vx  = c * (dest_x - start_x) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     *      vy  = c * (dest_y - start_y) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     */

    this->unit_vector   = sqrt(1.0f * ((dest_x - this->GetX())*(dest_x - this->GetX()) + (dest_y - this->GetY())*(dest_y - this->GetY())));
    this->dx            = BULLET_SPEED * (dest_x - this->GetX()) / this->unit_vector;
    this->dy            = BULLET_SPEED * (dest_y - this->GetY()) / this->unit_vector;
}

void GL_Bullet::RicochetVertical()
{
    this->dx = -this->dx;
}

void GL_Bullet::RicochetHorizontal()
{
    this->dy = -this->dy;
}

void GL_Bullet::Update()
{
    /* Update position using the unit vector */
    this->Move_Rate(this->dx, this->dy);
    this->Render();
}