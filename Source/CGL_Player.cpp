#include "CGL_Player.h"

using namespace gk_gl;

CGL_Player::CGL_Player(CTimer& Timer): Rate(0, 1),
    facing_degree(0.0f), moving(false), shot_delay(0)
{
    this->CGL_Entity::Move(300.0f, 300.0f);
    this->Barrel.x = this->GetX() + (this->GetW() / 2);
    this->Barrel.y = this->GetY();

    /* Load the player sprite sheet and set up
     * all of the necessary images.
     */
    SDL_Surface* Sprite_Sheet = gk_sdl::LoadImage_Alpha("Data/Images/Player.png");
    SDL_Surface* Main   = gk_sdl::clip_sprite_sheet(Sprite_Sheet, 0, 0, 64, 64);
    SDL_Surface* Anim_1 = gk_sdl::clip_sprite_sheet(Sprite_Sheet, 64, 0, 64, 64);

    this->SetEntity(Main);
    this->AddAnimation(Anim_1);
    this->SetAnimationRate(Timer.GetFrameRate() / 10);

    SDL_FreeSurface(Sprite_Sheet);
    SDL_FreeSurface(Main);
    SDL_FreeSurface(Anim_1);

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
    this->facing_degree += degree;
    this->Rotate_Rate(degree);
    this->Rate.x = sin(RAD(this->facing_degree));
    this->Rate.y = cos(RAD(this->facing_degree));
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

void CGL_Player::Update()
{
    if(this->shot_delay > 0)
        this->shot_delay--;

    if(this->IsMoving())
        this->Animate();
    this->Render();

    this->moving = false;
}