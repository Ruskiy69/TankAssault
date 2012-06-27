#include "Graphics/GL_Entity.h"

using namespace GFX;

GL_Entity::GL_Entity()
{
    this->texture   = -1;
    this->rotation  = 0.0f;
    this->texture   = 0;
    this->anim_rate = 15;
}

GL_Entity::~GL_Entity()
{
    for(size_t i = 0; i < this->Animations.size(); i++)
    {
        glDeleteTextures(1, &this->Animations[i]);
    }

    this->Animations.clear();
}

void GL_Entity::LoadEntity(const char* filename)
{
    if(this->texture >= 0)
        glDeleteTextures(1, &this->texture);

    this->texture = load_texture(filename);

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(this->Animations.size() == 0)
        this->Animations.push_back(texture);
    else
        this->Animations[0] = texture;

    /* Determine the width of the texture */
    SDL_Surface* Tmp = load_image(filename);
    this->Collision_Box.w = Tmp->w;
    this->Collision_Box.h = Tmp->h;

    SDL_FreeSurface(Tmp);
    Tmp = NULL;
}

void GL_Entity::LoadEntity(const std::string& filename)
{
    this->LoadEntity(filename.c_str());
}

void GL_Entity::SetEntity(SDL_Surface* Surface)
{
    if(this->texture >= 0)
        glDeleteTextures(1, &this->texture);

    this->texture = SDL_Surface_to_texture(Surface);
    this->Collision_Box.w = Surface->w;
    this->Collision_Box.h = Surface->h;

    if(this->Animations.size() == 0)
        this->Animations.push_back(texture);
    else
        this->Animations[0] = texture;
}

void GL_Entity::AddAnimation(SDL_Surface* Anim)
{
    this->Animations.push_back(SDL_Surface_to_texture(Anim));
}

void GL_Entity::AddAnimation(const unsigned int texture)
{
    this->Animations.push_back(texture);
}

void GL_Entity::SetAnimationRate(const unsigned int rate)
{
    this->anim_rate = rate;
}

void GL_Entity::Move(const float x, const float y)
{
    this->Position.Move(x, y);
    this->Collision_Box.Move(this->Position);
}

void GL_Entity::Move(const MathHelper::Vector2& Position)
{
    this->Position = Position;
    this->Collision_Box.Move(Position);
}

void GL_Entity::Move_Rate(const float dx, const float dy)
{
    this->Position.Move(this->Position.GetX() + dx,
        this->Position.GetY() + dy);

    this->Collision_Box.Move(this->Position);
}

void GL_Entity::Move_Rate(const MathHelper::Vector2& Rate)
{
    this->Position = this->Position + Rate;        
    this->Collision_Box.Move(this->Position);
}

void GL_Entity::Resize(const unsigned int w, const unsigned int h)
{
    this->Collision_Box.Resize(w, h);
}

void GL_Entity::Resize(const MathHelper::Rect& Size)
{
    this->Collision_Box.Resize(Size.w, Size.h);
}

void GL_Entity::Rotate(const float degree)
{
    this->rotation = degree;
}

void GL_Entity::Rotate_Rate(const float degrees)
{
    this->rotation += degrees;
}

bool GL_Entity::IsOffscreen(const GL_Display& Display) const
{
    return (this->GetX() > Display.GetWidth()) ||
        (this->GetX() < 0) || (this->GetY() < 0) ||
        (this->GetY() > Display.GetHeight());
}

bool GL_Entity::CheckCollision(const GL_Entity& Obj) const
{
    return this->CheckCollision(Obj.GetCollisionBox());
}

bool GL_Entity::CheckCollision(const MathHelper::Rect& Obj) const
{
    return this->Collision_Box.CheckCollision(Obj);
}

bool GL_Entity::CheckCollision(const MathHelper::Rects& Boxes) const
{
    bool done = false;

    for(size_t i = 0; i < Boxes.size(); i++)
    {
        done = this->CheckCollision(Boxes[i]);
        if(done)
            break;
    }

    return done;
}

bool GL_Entity::CheckCollision(const int x, const int y) const
{
    MathHelper::Rect Tmp(x, y, 0, 0);
    return Tmp.CheckCollision(this->Collision_Box);
}

int GL_Entity::GetH() const
{
    return this->Collision_Box.h;
}

int GL_Entity::GetW() const
{
    return this->Collision_Box.w;
}

float GL_Entity::GetX() const
{
    return this->Position.GetX();
}

float GL_Entity::GetY() const
{
    return this->Position.GetY();
}

const MathHelper::Vector2& GL_Entity::GetPosition() const
{
    return this->Position;
}

const MathHelper::Rect& GL_Entity::GetCollisionBox() const
{
    return this->Collision_Box;
}

void GL_Entity::Render() const
{
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
        glTexCoord2f(0, 0); glVertex2f(this->Position.GetX(), this->Position.GetY());
        glTexCoord2f(1, 0); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY());
        glTexCoord2f(1, 1); glVertex2f(this->Position.GetX() + this->Collision_Box.w, this->Position.GetY() + this->Collision_Box.h);
        glTexCoord2f(0, 1); glVertex2f(this->Position.GetX(), this->Position.GetY() + this->Collision_Box.h);
    glEnd();

    /* Disable blending if it was disabled originally */
    if(!blend)
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    glPopMatrix();
}

void GL_Entity::Animate()
{
    static int frame = 0;
    static size_t index = 0;
    frame++;

    if(frame % this->anim_rate == 0)
    {
        this->texture = this->Animations[index];
        index++;

        if(index >= this->Animations.size())
            index = 0;
    }
}

void GL_Entity::Update()
{
    this->Animate();
    this->Render();
}
