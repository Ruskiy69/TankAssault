#include "GL_Player.h"

using namespace gk_gl;

CGL_Player::CGL_Player(gk::CTimer& Timer): Rate(0, 1),
    facing_degree(0.0f), moving(false), shot_delay(0)
{
    this->CGL_Entity::Move(300.0f, 300.0f);
    this->Barrel.x = this->GetX() + (this->GetW() / 2);
    this->Barrel.y = this->GetY();

    /* Load the player sprite sheet and set up
     * all of the necessary images.
     */
    SDL_Surface* Sprite_Sheet = gk_sdl::LoadImage_Alpha("Data/Images/Player.png");

    this->SetEntity(Sprite_Sheet);
    this->SetAnimationRate(Timer.GetFrameRate() / 10);
    this->Resize(64, 64);       // So we only use half the sprite sheet (first texture)

    SDL_FreeSurface(Sprite_Sheet);

    /* Load shot sound */
    this->Shot = Mix_LoadWAV("Data/Sounds/Player_Shot.wav");
}

CGL_Player::~CGL_Player()
{
    Mix_FreeChunk(this->Shot);
}

void CGL_Player::RotateBody(const float degree)
{
    this->moving = true;
    this->Rotate_Rate(degree);
    this->Rate.x = sin(RAD(this->rotation));
    this->Rate.y = cos(RAD(this->rotation));
}

void CGL_Player::Move(const float speed)
{
    this->moving = true;
    this->Move_Rate(this->Rate.x * -speed, this->Rate.y * -speed);

    this->Barrel.x = this->GetX() + (this->GetW() / 2);
    this->Barrel.y = this->GetY();
}

void CGL_Player::Shoot(CGL_Bullets& Bullets)
{
    static int x, y;

    if(this->shot_delay <= 0)
    {
        /* Play the firing sound */
        this->shot_delay = this->SHOT_DELAY;
        Mix_PlayChannel(-1, this->Shot, 0);
        
        /* Get target */
        GetMousePosition(x, y);
        
        /* Create our new Bullet */
        CGL_Bullet* Bullet = new CGL_Bullet((int)this->GetX(), (int)this->GetY());
        Bullet->Move(this->Barrel);

        /* Determine the angle necessary to
         * hit the designated (x, y) target.
         */
        float angle = DEG(atan(abs(y - this->GetY()) / abs(x - this->GetX()) * 1.0f));
        
        /* Since arc-tangent limits us to the first quadrant, 
         * we need to adjust our angles in order to 
         * acheive the proper rotation of the missile.
         */
        if(y > this->GetY() && x < this->GetX())        // Third quadrant
            Bullet->Rotate(angle + 180);
        else if(y > this->GetY() && x > this->GetX())   // Fourth
            Bullet->Rotate(360 - angle);
        else if(y < this->GetY() && x > this->GetX())   // First
            Bullet->Rotate(angle);
        else                                            // Second
            Bullet->Rotate(180 - angle);

        /* Finally, add the bullet to the vector */
        Bullets.push_back(Bullet);
    }
}

void CGL_Player::Render()
{
    static int frame = 0;
    static bool texture_pos = 0;

    frame++;
    if(moving && (frame % this->anim_rate == 0))
        texture_pos = !texture_pos;

    /* Alpha channel is gucci */
    GLboolean blend = glIsEnabled(GL_BLEND);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Render the primitive with the texture on top.
     * First, rotate around the origin.
     */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(this->GetX() + this->GetW() / 2,
        this->GetY() + this->GetH() / 2, 0.0f);
    glRotatef(-this->rotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(-this->GetX() - this->GetW() / 2,
        -this->GetY() - this->GetH() / 2, 0.0f);
    
    /* Then bind the texture */
    glBindTexture(GL_TEXTURE_2D, this->texture);

    /* Draw the primitive */
    glBegin(GL_QUADS);
    if(texture_pos)
    {
        glTexCoord2f(0,     0); glVertex2f(this->Position.x, this->Position.y);
        glTexCoord2f(0.5f,  0); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y);
        glTexCoord2f(0.5f,  1); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y + this->CollisionBox.h);
        glTexCoord2f(0,     1); glVertex2f(this->Position.x, this->Position.y + this->CollisionBox.h);
    }
    else
    {
        glTexCoord2f(0.5f,  0); glVertex2f(this->Position.x, this->Position.y);
        glTexCoord2f(1.0f,  0); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y);
        glTexCoord2f(1.0f,  1); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y + this->CollisionBox.h);
        glTexCoord2f(0.5f,  1); glVertex2f(this->Position.x, this->Position.y + this->CollisionBox.h);
    }
    glEnd();

    /* Disable blending if it was disabled originally */
    if(!blend)
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    glPopMatrix();
}

void CGL_Player::Update()
{
    if(this->shot_delay > 0)
        this->shot_delay--;

    this->Render();

    this->moving = false;
}