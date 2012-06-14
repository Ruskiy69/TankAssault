#include "CGL_Player.h"

using namespace gk_gl;
using namespace gk_sdl;

CGL_Player::CGL_Player(CTimer& Timer)
{
    this->CGL_Entity::Move(300.0f, 300.0f);

    this->facing_degree = 0.0f;
    this->moving = false;
    this->shot_delay = 0;
    
    memset(&this->Rate, 0, sizeof(GL_Vertex2f));
    memset(&this->Barrel, 0, sizeof(GL_Vertex2f));
    memset(&this->Barrel_Rate, 0, sizeof(GL_Vertex2f));

    this->Rate.x = 0.0f;
    this->Rate.y = 1.0f;
    this->Barrel.x = this->Position.x + (this->GetW() / 2);
    this->Barrel.y = this->Position.y;

    SDL_Surface* sheet = LoadImage_Alpha("Data/Images/Player.png");
    SDL_Surface* anim_1 = clip_sprite_sheet(sheet, 0, 0, 64, 64);
    SDL_Surface* anim_2 = clip_sprite_sheet(sheet, 64, 0, 64, 64);

    this->SetEntity(anim_1);
    this->AddAnimation(SDL_Surface_to_texture(anim_2));
    this->SetAnimationRate(Timer.GetFrameRate() / 10);

    this->Shot = Mix_LoadWAV("Data/Sounds/Player_Shot.wav");

    SDL_FreeSurface(sheet);
    SDL_FreeSurface(anim_1);
    SDL_FreeSurface(anim_2);
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

void CGL_Player::Shoot(CBullets& Bullets)
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
        CBullet* Bullet = new CBullet((int)this->GetX(), (int)this->GetY());
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

    /* Animate the tank (if moving),
     * and render it on the screen.
     */
    this->Animate();
    this->Render();

    this->moving = false;
}

void CGL_Player::Animate()
{
    static int index = 0;

    if(moving)
    {
        this->CGL_Entity::Animate();
    }
}