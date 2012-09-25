/**
 * @file
 *	Definitions for the various shape classes.
 *
 * @author  George Kudrayvtsev
 * @version 1.3.1
 **/

#include "Math/Shapes.hpp"

using math::CRect;
using math::CRectf;
using math::CCircle;

/*************************************
 * math::CRect method definitions. *
 *************************************/

/**
 * Compares the current rectangle with a given one.
 *
 * Two rectangles are equal if their dimensions and locations are the same.
 *
 * @param CRect& The rectangle to compare.
 *
 * @return TRUE if equal, FALSE otherwise.
 **/
bool CRect::operator== (const CRect& Other)
{
    return (Other.x == this->x && Other.y == this->y &&
        Other.w == this->w && Other.h == this->h);
}

/**
 * Moves the rectangle.
 *
 * @param int x-coordinate
 * @param int y-coordinate
 **/
void CRect::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the rectangle.
 *
 * @overload CRect::Move(int, int)
 *
 * @param math::CVector2 Position
 **/
void CRect::Move(const math::CVector2& Pos)
{
    this->x = (int)Pos.x;
    this->y = (int)Pos.y;
}

/**
 * Changes the dimensions of the rectangle.
 *
 * @param int Width
 * @param int Height
 **/
void CRect::Resize(u_int w, u_int h)
{
    this->w = w;
    this->h = h;
}

/**
 * Checks for collision or overlap with another math::CRect.
 *
 * @param CRect Other rectangle
 *
 * @return TRUE if overlap, FALSE otherwise.
 **/
bool CRect::CheckCollision(const CRect& Other) const
{
    int top2, bottom2, right2, left2;
    
    left2   = Other.x;
    right2  = Other.x + Other.w;
    top2    = Other.y;
    bottom2 = Other.y + Other.h;

    if(this->y + this->h < top2)
        return false;
    if(this->y > bottom2)
        return false;
    if(this->x + this->w < left2)
        return false;
    if(this->x > right2)
        return false;

    return true;
}

/**
 * Checks if the current rect is completely covering another one.
 *
 * @param math::CRect Check if this rect is inside the current one
 *
 * @return TRUE if given rect is covered, FALSE otherwise.
 * @todo Clean this up, instead of just using a 0-w/h rect for each point.
 **/
bool CRect::IsCovering(const CRect& Other) const
{
    CRect Top_Left(Other.x, Other.y);
    CRect Bottom_Left(Other.x, Other.y + Other.h);
    CRect Top_Right(Other.x + Other.w, Other.y);
    CRect Bottom_Right(Other.x + Other.w, Other.y + Other.h);

    return (
        this->CheckCollision(Top_Left)      &&
        this->CheckCollision(Top_Right)     &&
        this->CheckCollision(Bottom_Left)   && 
        this->CheckCollision(Bottom_Right)
        );
}

/***********************************
 * math::CRectf method definitions. *
 ***********************************/

/**
 * Compares the current rectangle with a given one.
 *
 * Two rectangles are equal if their dimensions and locations are the same.
 *
 * @param CRectf& The rectangle to compare.
 *
 * @return TRUE if equal, FALSE otherwise.
 **/
bool CRectf::operator== (const CRectf& Other)
{
    return (Other.x == this->x && Other.y == this->y &&
        Other.w == this->w && Other.h == this->h);
}

/**
 * Moves the rectangle.
 *
 * @param float x-coordinate
 * @param float y-coordinate
 **/
void CRectf::Move(float x, float y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the rectangle.
 *
 * @overload CRectf::Move(int, int)
 *
 * @param math::CVector2 Position
 **/
void CRectf::Move(const math::CVector2& Pos)
{
    this->x = Pos.x;
    this->y = Pos.y;
}

/**
 * Changes the dimensions of the rectangle.
 *
 * @param float Width
 * @param float Height
 **/
void CRectf::Resize(float w, float h)
{
    this->w = w;
    this->h = h;
}

/**
 * Checks for collision or overlap with another math::CRect.
 *
 * @param CRectf Other rectangle
 *
 * @return TRUE if overlap, FALSE otherwise.
 **/
bool CRectf::CheckCollision(const CRectf& Other) const
{
    float top2, bottom2, right2, left2;
    
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

/************************************
 * math::CCircle method definitions. *
 ************************************/

/**
 * Moves the circle.
 *
 * @param int x-coordinate
 * @param int y-coordinate
 **/
void CCircle::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the circle.
 *
 * \overload CCircle::Move(int, int)
 *
 * @param math::CVector2 Position
 **/
void CCircle::Move(const math::CVector2& Pos)
{
    this->x = (int)Pos.x;
    this->y = (int)Pos.y;
}

/**
 * Changes the radius of the circle.
 *
 * @param int Radius
 **/
void CCircle::Resize(u_int r)
{
    this->r = r;
}

/**
 * Checks for collision or overlap with another math::CCircle.
 *
 * @param CCircle Other circle
 *
 * @return TRUE if overlap, FALSE otherwise.
 **/
bool CCircle::CheckCollision(const CCircle& Other) const
{
    // Compare the distances between the radii
    int dx, dy, rs;
    
    dx = Other.x - this->x;
    dy = Other.y - this->y;
    rs = this->r + Other.r;
    
    return ((dx*dx) + (dy*dy)) < (rs*rs);
}