#include "World/Objects/GameObject.hpp"

using obj::CGameObject;

bool CGameObject::LoadFromFile(const char* pfilename)
{
    bool val = CEntity::LoadFromFile(pfilename);
    m_CollisionBox.Resize(m_Texture.GetW(), m_Texture.GetH());
    return val;
}

bool CGameObject::LoadFromTexture(const asset::CTexture* pTexture)
{
    bool val = CEntity::LoadFromTexture(pTexture);
    m_CollisionBox.Resize(m_Texture.GetW(), m_Texture.GetH());
    return val;
}

bool CGameObject::LoadFromSurface(SDL_Surface* pSurface)
{
    bool val = CEntity::LoadFromSurface(pSurface);
    m_CollisionBox.Resize(m_Texture.GetW(), m_Texture.GetH());
    return val;
}

/**
 * Checks for collision with another CGameObject.
 *
 * @param CGameObject Other entity to check collision with.
 * @return TRUE if they collide anywhere, FALSE otherwise.
 **/
bool CGameObject::CheckCollision(const CGameObject* pObj) const
{
    return this->CheckCollision(pObj->GetCollisionBox());
}

/**
 * Checks for collision with a CEntity.
 *
 * @param CEntity Other entity to check collision with.
 * @return TRUE if they collide anywhere, FALSE otherwise.
 **/
bool CGameObject::CheckCollision(const CEntity* pObj) const
{
    math::CRect Boundary(
        pObj->GetX(),
        pObj->GetY(),
        pObj->GetTexture().GetW(),
        pObj->GetTexture().GetH());

    return this->CheckCollision(Boundary);
}

/**
 * @overload CGameObject::CheckCollision(const CGameObject& Obj)
 *
 * @param math::CRect CRect to check collision with.
 * @return TRUE if they collide anywhere, FALSE otherwise.
 * @see math::CRect
 **/
bool CGameObject::CheckCollision(const math::CRect& Obj) const
{
    return Obj.CheckCollision(m_CollisionBox);
}

/**
 * @overload CGameObject::CheckCollision(const CGameObject& Obj)
 *
 * @param int x
 * @param int y
 * @return TRUE if they collide anywhere, FALSE otherwise.
 **/
bool CGameObject::CheckCollision(const float x, const float y) const
{
    math::CRect Tmp(x, y);
    return Tmp.CheckCollision(m_CollisionBox);
}

void CGameObject::ResizeCollisionBox(const u_int w, const u_int h)
{
    m_CollisionBox.Resize(w, h);
}

void CGameObject::ResizeCollisionBox(const math::CRect& NewSize)
{
    m_CollisionBox = NewSize;
}

void CGameObject::Update()
{
    CEntity::Update();
    m_CollisionBox.Move(m_Position);
}

void CGameObject::SetCollisionBox(const math::CRect& NewBox)
{
    m_CollisionBox = NewBox;
}

const math::CRect& CGameObject::GetCollisionBox() const
{
    return m_CollisionBox;
}

void CGameObject::Damage(const u_int dmg)
{
    if(m_health > 0)
        m_health -= dmg;
}

bool CGameObject::IsAlive() const
{
    return (m_health > 0);
}

u_int CGameObject::GetHealth() const
{
    return m_health;
}
