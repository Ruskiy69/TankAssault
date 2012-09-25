#include "World/Objects/Entity.hpp"

using obj::CEntity;

CEntity::CEntity() : m_RenderDimensions(0, 0, 1.0f, 1.0f), 
    m_rotation_angle(0.0f), m_useblending(false)
{
    memset(&m_vertices, 0, sizeof m_vertices);
}

CEntity::~CEntity()
{}

bool CEntity::LoadFromFile(const char* pfilename)
{
    assert(pfilename != NULL);

    if(m_Texture.GetFilename() == pfilename)
        return true;

    return m_Texture.LoadFromFile(pfilename);
}

bool CEntity::LoadFromTexture(const asset::CTexture* pTexture)
{
    assert(pTexture != NULL);
    m_Texture = *pTexture;
    return true;
}

bool CEntity::LoadFromSurface(SDL_Surface* pSurface)
{
    return m_Texture.LoadFromSurface(pSurface);
}

bool CEntity::LoadFromEntity(CEntity* pOther)
{
    assert(pOther != NULL);

    // Copy the entity attributes.
    m_Texture = pOther->GetTexture();
    return true;
}

void CEntity::Move(const math::CVector2& Position)
{
    m_MovementRate.Move(Position - m_Position);
}

void CEntity::Move(const float x, const float y)
{
    m_MovementRate.Move(x - m_Position.x, y - m_Position.y);
}

void CEntity::Move_Rate(const math::CVector2& Rate)
{
    m_MovementRate.x += Rate.x;
    m_MovementRate.y += Rate.y;
}

void CEntity::Move_Rate(const float dx, const float dy)
{
    m_MovementRate.x += dx;
    m_MovementRate.y += dy;
}

void CEntity::Rotate(const float deg)
{
    m_rotation_angle = deg;
}

void CEntity::Rotate_Rate(const float ddeg)
{
    m_rotation_angle += ddeg;
}

void CEntity::ResizeTexture(const u_int w, const u_int h)
{
    m_Texture.Resize(w, h);
}

void CEntity::ResizeTexture(const math::CRect& NewSize)
{
    m_Texture.Resize(NewSize.w, NewSize.h);
}

void CEntity::Update()
{
    m_Position = m_Position + m_MovementRate;
    m_MovementRate.Move(0, 0);

    m_vertices[0] = m_Position.x;
    m_vertices[1] = m_Position.y;
    m_vertices[2] = m_Position.x + m_Texture.GetW();
    m_vertices[3] = m_Position.y + m_Texture.GetH();

    float* pvertices = this->GetVertices();
    const math::CRectf& Rendering = this->GetRenderDimensions();

    glLoadIdentity();

    // Rotate the object around its origin.
    glTranslatef(
        this->GetX() + this->GetW() / 2,
        this->GetY() + this->GetH() / 2,
        0.0f);
    glRotatef(this->GetRotationAngle(), 0.0f, 0.0f, 1.0f);
    glTranslatef(
        -this->GetX() - this->GetW() / 2,
        -this->GetY() - this->GetH() / 2,
        0.0f);

    // Activate the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->GetGLTexture());

    glBegin(GL_QUADS);

    // Top left corner
    glTexCoord2f(Rendering.x, Rendering.y);
    glVertex2f(pvertices[0], pvertices[1]);

    // Top right corner
    glTexCoord2f(Rendering.x + Rendering.w, Rendering.y);
    glVertex2f(pvertices[2], pvertices[1]);

    // Bottom right corner
    glTexCoord2f(Rendering.x + Rendering.w, Rendering.y + Rendering.h);
    glVertex2f(pvertices[2], pvertices[3]);

    // Bottom left corner
    glTexCoord2f(Rendering.x, Rendering.y + Rendering.h);
    glVertex2f(pvertices[0], pvertices[3]);

    glEnd();
}

void CEntity::SetBlending(bool flag)
{
    m_useblending = flag;
}

void CEntity::SetRenderDimensions(const math::CRectf& RenderDimensions)
{
    m_RenderDimensions = RenderDimensions;
}

void CEntity::SetMovementRate(const math::CVector2& NewRate)
{
    m_MovementRate = NewRate;
}

const math::CVector2& CEntity::GetPosition() const
{
    return m_Position;
}

const math::CVector2& CEntity::GetMovementRate() const
{
    return m_MovementRate;
}

const math::CRectf& obj::CEntity::GetRenderDimensions() const
{
    return m_RenderDimensions;
}

const std::string& CEntity::GetFilename() const
{
    return m_Texture.GetFilename();
}

float* CEntity::GetVertices()
{
    return m_vertices;
}

float CEntity::GetRotationAngle() const
{
    return m_rotation_angle;
}

float CEntity::GetX() const
{
    return m_Position.x;
}

float CEntity::GetY() const
{
    return m_Position.y;
}

const asset::CTexture& CEntity::GetTexture() const
{
    return m_Texture;
}

u_int CEntity::GetGLTexture() const
{
    return m_Texture.GetTexture();
}

int CEntity::GetW() const
{
    return m_Texture.GetW();
}

int CEntity::GetH() const
{
    return m_Texture.GetH();
}

/**
 * Fades entities off of the screen.
 *
 * Should be called within a game loop, once per frame.
 * 
 * @param std::vector<GL_Entity*> Entities to fade out
 * @param float Rate to fade out at, default is 0.01 (optional)
 *
 * @return bool TRUE when done fading, FALSE otherwise.
 */
bool gfx::FadeOut(const std::vector<obj::CEntity*> pentitiesToFade,
    const float rate)
{
    static float alpha = 1.0f;

    glColor4f(1, 1, 1, alpha);
    for(size_t i = 0; i < pentitiesToFade.size(); ++i)
    {
        pentitiesToFade[i]->Update();
    }
    glColor4f(1, 1, 1, 1);

    alpha -= rate;

    if(alpha < 0.0f)
    {
        alpha = 1.0f;
        return true;
    }
    else
        return false;
}

bool gfx::FadeOut(obj::CEntity* p_ToFade, const float rate)
{
    static float alpha = 1.0f;

    glColor4f(1, 1, 1, alpha);
    p_ToFade->Update();
    glColor4f(1, 1, 1, 1);

    alpha -= rate;

    if(alpha < 0.0f)
    {
        alpha = 1.0f;
        return true;
    }
    else
        return false;
}

/**
 * Fades entities into the screen.
 *
 * Should be called within a game loop, once per frame.
 * 
 * @param std::vector<GL_Entity*> Entities to fade in
 * @param float Rate to fade in at, default is 0.01 (optional)
 *
 * @return bool TRUE when done fading, FALSE otherwise.
 */
bool gfx::FadeIn(const std::vector<obj::CEntity*> pentitiesToFade,
    const float rate)
{
    static float alpha = 0.0f;

    glColor4f(1, 1, 1, alpha);
    for(size_t i = 0; i < pentitiesToFade.size(); ++i)
    {
        pentitiesToFade[i]->Update();
    }
    glColor4f(1, 1, 1, 1);

    alpha += rate;

    if(alpha > 1.0f)
    {
        alpha = 0.0f;
        return true;
    }
    else
        return false;
}

bool gfx::FadeIn(obj::CEntity* p_ToFade, const float rate)
{
    static float alpha = 0.0f;

    glColor4f(1, 1, 1, alpha);
    p_ToFade->Update();
    glColor4f(1, 1, 1, 1);

    alpha += rate;

    if(alpha > 1.0f)
    {
        alpha = 0.0f;
        return true;
    }
    else
        return false;
}