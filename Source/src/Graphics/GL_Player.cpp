#include "Graphics/GL_Player.h"

using Game::GL_Player;

GL_Player::GL_Player(const Game::Game_Timer& Timer): Rate(0, 1),
    facing_degree(0.0f), moving(false), shot_delay(0), mv_sound_channel(-1)
{
    this->Move(300.0f, 300.0f);
    this->Barrel.Move(this->GetX() + (this->GetW() / 2), this->GetY());

    /* Load the player sprite sheet and set up
     * all of the necessary images.
     */
    SDL_Surface* Sprite_Sheet = GFX::load_image_alpha("Data/Images/Player.png");

    this->SetEntity(Sprite_Sheet);
    this->SetAnimationRate(Timer.GetFrameRate() / 10);
    this->Resize(64, 64);       // So we only use half the sprite sheet (first texture)

    SDL_FreeSurface(Sprite_Sheet);

    /* Load shot sound */
    if(!this->Shot.Load("Data/Sounds/Player_Shot.wav") || 
        !this->MoveSound.Load("Data/Sounds/Player_Moving.wav"))
    {
        gk::handle_error(Mix_GetError());
    }

    this->MoveSound.SetVolume(69);
}

GL_Player::~GL_Player() {}

void GL_Player::RotateBody(const float degree)
{
    if(degree != 0.0f)
        this->moving = true;

    this->Rotate_Rate(degree);
    this->Rate.Move(sin(RAD(this->rotation)), cos(RAD(this->rotation)));
}

void GL_Player::Move(const float speed)
{
    if(speed != 0.0f)
    {
        this->moving = true;
        
        this->Move_Rate(this->Rate.GetX() * -speed, this->Rate.GetY() * -speed);
        this->Barrel.Move(this->GetX() + (this->GetW() / 2), this->GetY());
    }
    else
        this->moving = false;
}

bool GL_Player::Shoot(Game::GL_Bullets& Bullets)
{
    static int x = 0, y = 0;

    if(this->shot_delay <= 0)
    {
        /* Play the firing sound */
        this->shot_delay = GL_Player::SHOT_DELAY;
        this->Shot.Play_Force();

        /* Get target */
        SDL_GetMouseState(&x, &y);

        /* Create our new Bullet */
        Game::GL_Bullet* Bullet = new Game::GL_Bullet(this->GetPosition());
        //Bullet->Move(this->BarrelPosition);

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

        return true;
    }

    return false;
}

void GL_Player::Render()
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
        glTexCoord2f(0,     0); glVertex2f(this->Position.GetX(), this->Position.GetY());
        glTexCoord2f(0.5f,  0); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY());
        glTexCoord2f(0.5f,  1); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY() + this->Collision_Box.h);
        glTexCoord2f(0,     1); glVertex2f(this->Position.GetX(), this->Position.GetY() + this->Collision_Box.h);
    }
    else
    {
        glTexCoord2f(0.5f,  0); glVertex2f(this->Position.GetX(), this->Position.GetY());
        glTexCoord2f(1.0f,  0); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY());
        glTexCoord2f(1.0f,  1); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY() + this->Collision_Box.h);
        glTexCoord2f(0.5f,  1); glVertex2f(this->Position.GetX(), this->Position.GetY() + this->Collision_Box.h);
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

void GL_Player::Update()
{
    if(this->shot_delay > 0)
        this->shot_delay--;

    this->Render();
    if(this->IsMoving())
        this->MoveSound.Play();
    else
        this->MoveSound.Stop();

    this->moving = false;
}