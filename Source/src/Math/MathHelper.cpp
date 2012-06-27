#include "Math/MathHelper.h"

using namespace MathHelper;

void Rect::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Rect::Move(const Vector2& Pos)
{
    this->x = Pos.GetX();
    this->y = Pos.GetY();
}

void Rect::Resize(int w, int h)
{
    this->w = w;
    this->h = h;
}

bool Rect::CheckCollision(const Rect& Other) const
{
    int top2, bottom2, right2, left2;
    
    left2   = Other.x;
    right2  = Other.x + Other.w;
    top2    = Other.y;
    bottom2 = Other.y + Other.h;

    if(this->y + this->h <= top2)
        return false;
    if(this->y >= bottom2)
        return false;
    if(this->x + this->w <= left2)
        return false;
    if(this->x >= right2)
        return false;

    return true;
}

void Circle::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Circle::Move(const Vector2& Pos)
{
    this->x = Pos.GetX();
    this->y = Pos.GetY();
}

void Circle::Resize(int r)
{
    this->r = r;
}

bool Circle::CheckCollision(const Circle& Other) const
{
    /* Compare the distances between the radii */
    int dx, dy, rs;
    
    dx = Other.x - this->x;
    dy = Other.y - this->y;
    rs = this->r + Other.r;
    
    return ((dx*dx) + (dy*dy)) < (rs*rs);
}