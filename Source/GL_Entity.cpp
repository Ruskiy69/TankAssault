#include "GL_Entity.h"

using namespace gk_gl;
using gk_sdl::LoadImage_Alpha;

CGL_Entity::CGL_Entity()
{
    this->rotation = 0.0f;
    this->texture = 0;
    this->anim_rate = 15;
    
    memset(&this->CollisionBox, 0, sizeof(GL_Rect));
    memset(&this->Position, 0, sizeof(GL_Vertex2f));
}

CGL_Entity::~CGL_Entity()
{
}

void CGL_Entity::LoadEntity(const char* filename)
{
    /* Load the texture using the handy SOIL library */
    this->texture = ReloadTexture(filename, this->texture, SOIL_FLAG_POWER_OF_TWO);
        
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(this->Animations.size() == 0)
        this->Animations.push_back(texture);
    else
        this->Animations[0] = texture;

    /* Determine the width of the texture */
    SDL_Surface* tmp = LoadImage_Alpha(filename);
    this->CollisionBox.w = tmp->w;
    this->CollisionBox.h = tmp->h;

    SDL_FreeSurface(tmp);
    tmp = NULL;
}

void CGL_Entity::LoadEntity(const std::string& filename)
{
    this->LoadEntity(filename.c_str());
}

void CGL_Entity::SetEntity(gl_texture texture, const int w, const int h)
{
    this->texture = texture;
    if(this->Animations.size() == 0)
        this->Animations.push_back(texture);
    else
        this->Animations[0] = texture;
}

void CGL_Entity::SetEntity(SDL_Surface* Surface)
{
    this->texture = SDL_Surface_to_texture_reload(Surface, this->texture);
    this->CollisionBox.w = Surface->w;
    this->CollisionBox.h = Surface->h;

    if(this->Animations.size() == 0)
        this->Animations.push_back(texture);
    else
        this->Animations[0] = texture;
}

void CGL_Entity::AddAnimation(SDL_Surface* Anim)
{
    this->Animations.push_back(gk_gl::SDL_Surface_to_texture(Anim));
}

void CGL_Entity::AddAnimation(const unsigned int texture)
{
    this->Animations.push_back(texture);
}

void CGL_Entity::SetAnimationRate(const unsigned int rate)
{
    this->anim_rate = rate;
}

void CGL_Entity::Move(const float x, const float y)
{
    this->Position.x = x;
    this->Position.y = y;
    this->CollisionBox.Position = this->Position;
}

void CGL_Entity::Move(const GL_Vertex2f& Position)
{
    this->Position.x = Position.x;
    this->Position.y = Position.y;
    this->CollisionBox.Position = this->Position;
}

void CGL_Entity::Move_Rate(const float dx, const float dy)
{
    this->Position.x += dx;
    this->Position.y += dy;
    this->CollisionBox.Position = this->Position;
}

void CGL_Entity::Move_Rate(const GL_Vertex2f& Rate)
{
    this->Position.x += Rate.x;
    this->Position.y += Rate.y;
    this->CollisionBox.Position = this->Position;
}

void CGL_Entity::Resize(const unsigned int w, const unsigned int h)
{
    this->CollisionBox.w = w;
    this->CollisionBox.h = h;
}

void CGL_Entity::Rotate(const float degree)
{
    this->rotation = degree;
}

void CGL_Entity::Rotate_Rate(const float degrees)
{
    this->rotation += degrees;
}

bool CGL_Entity::IsOffscreen(const CGL_Display& Display) const
{
    return (this->GetX() > Display.GetWidth()) ||
        (this->GetX() < 0) || (this->GetY() < 0) ||
        (this->GetY() > Display.GetHeight());
}

bool CGL_Entity::CheckCollision(const CGL_Entity& Obj) const
{
    return this->CheckCollision(Obj.GetCollisionBox());
}

bool CGL_Entity::CheckCollision(const GL_Rect& Obj) const
{
    return detect_collision(Obj, this->CollisionBox);
}

bool CGL_Entity::CheckCollision(const int x, const int y) const
{
    GL_Rect Tmp = {gk_gl::GL_Vertex2f(x, y), 0, 0};
    return detect_collision(Tmp, this->CollisionBox);
}

int CGL_Entity::GetH() const
{
    return this->CollisionBox.h;
}

int CGL_Entity::GetW() const
{
    return this->CollisionBox.w;
}

float CGL_Entity::GetX() const
{
    return this->Position.x;
}

float CGL_Entity::GetY() const
{
    return this->Position.y;
}

const GL_Vertex2f& CGL_Entity::GetPosition() const
{
    return this->Position;
}

const GL_Rect& CGL_Entity::GetDimensions() const
{
    return this->CollisionBox;
}

const GL_Rect& CGL_Entity::GetCollisionBox() const
{
    return this->CollisionBox;
}

void CGL_Entity::Render() const
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
        glTexCoord2f(0, 0); glVertex2f(this->Position.x, this->Position.y);
        glTexCoord2f(1, 0); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y);
        glTexCoord2f(1, 1); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y + this->CollisionBox.h);
        glTexCoord2f(0, 1); glVertex2f(this->Position.x, this->Position.y + this->CollisionBox.h);
    glEnd();

    /* Disable blending if it was disabled originally */
    if(!blend)
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    glPopMatrix();
}

void CGL_Entity::Animate()
{
    static int frame = 0;
    static int index = 0;
    frame++;
    
    if(frame % this->anim_rate == 0)
    {
        this->texture = this->Animations[index];
        index++;
        
        if(index >= this->Animations.size())
            index = 0;
    }
}

void CGL_Entity::Update()
{
    this->Animate();
    this->Render();
}
