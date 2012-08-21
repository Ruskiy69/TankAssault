/**
 * @file
 *	Definitions for the various shape classes.
 *
 * @author  George Kudrayvtsev
 * @version 1.3.1
 */

#include "Math/Shapes.h"

using math::ML_Rect;
using math::ML_Rectf;
using math::ML_Circle;

/*************************************
 * math::ML_Rect method definitions. *
 ************************************/

/**
 * Compares the current rectangle with a given one.
 *
 * Two rectangles are equal if their dimensions and locations are the same.
 *
 * @param ML_Rect& The rectangle to compare.
 *
 * @return TRUE if equal, FALSE otherwise.
 */
bool ML_Rect::operator== (const ML_Rect& Other)
{
    return (Other.x == this->x && Other.y == this->y &&
        Other.w == this->w && Other.h == this->h);
}

/**
 * Moves the rectangle.
 *
 * @param int x-coordinate
 * @param int y-coordinate
 */
void ML_Rect::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the rectangle.
 *
 * @overload ML_Rect::Move(int, int)
 *
 * @param math::ML_Vector2 Position
 */
void ML_Rect::Move(const math::ML_Vector2& Pos)
{
    this->x = (int)Pos.x;
    this->y = (int)Pos.y;
}

/**
 * Changes the dimensions of the rectangle.
 *
 * @param int Width
 * @param int Height
 */
void ML_Rect::Resize(u_int w, u_int h)
{
    this->w = w;
    this->h = h;
}

/**
 * Checks for collision or overlap with another math::ML_Rect.
 *
 * @param ML_Rect Other rectangle
 *
 * @return TRUE if overlap, FALSE otherwise.
 */
bool ML_Rect::CheckCollision(const ML_Rect& Other) const
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
 * @param math::ML_Rect Check if this rect is inside the current one
 *
 * @return TRUE if given rect is covered, FALSE otherwise.
 * @todo Clean this up, instead of just using a 0-w/h rect for each point.
 */
bool ML_Rect::IsCovering(const ML_Rect& Other) const
{
    ML_Rect Top_Left(Other.x, Other.y);
    ML_Rect Bottom_Left(Other.x, Other.y + Other.h);
    ML_Rect Top_Right(Other.x + Other.w, Other.y);
    ML_Rect Bottom_Right(Other.x + Other.w, Other.y + Other.h);

    return (
        this->CheckCollision(Top_Left)      &&
        this->CheckCollision(Top_Right)     &&
        this->CheckCollision(Bottom_Left)   && 
        this->CheckCollision(Bottom_Right)
        );
}

/***********************************
 * math::ML_Rectf method definitions. *
 **********************************/

/**
 * Compares the current rectangle with a given one.
 *
 * Two rectangles are equal if their dimensions and locations are the same.
 *
 * @param ML_Rectf& The rectangle to compare.
 *
 * @return TRUE if equal, FALSE otherwise.
 */
bool ML_Rectf::operator== (const ML_Rectf& Other)
{
    return (Other.x == this->x && Other.y == this->y &&
        Other.w == this->w && Other.h == this->h);
}

/**
 * Moves the rectangle.
 *
 * @param float x-coordinate
 * @param float y-coordinate
 */
void ML_Rectf::Move(float x, float y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the rectangle.
 *
 * @overload ML_Rectf::Move(int, int)
 *
 * @param math::ML_Vector2 Position
 */
void ML_Rectf::Move(const math::ML_Vector2& Pos)
{
    this->x = Pos.x;
    this->y = Pos.y;
}

/**
 * Changes the dimensions of the rectangle.
 *
 * @param float Width
 * @param float Height
 */
void ML_Rectf::Resize(float w, float h)
{
    this->w = w;
    this->h = h;
}

/**
 * Checks for collision or overlap with another math::ML_Rect.
 *
 * @param ML_Rectf Other rectangle
 *
 * @return TRUE if overlap, FALSE otherwise.
 */
bool ML_Rectf::CheckCollision(const ML_Rectf& Other) const
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
 * math::ML_Circle method definitions. *
 ***********************************/

/**
 * Moves the circle.
 *
 * @param int x-coordinate
 * @param int y-coordinate
 */
void ML_Circle::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * Moves the circle.
 *
 * \overload ML_Circle::Move(int, int)
 *
 * @param math::ML_Vector2 Position
 */
void ML_Circle::Move(const math::ML_Vector2& Pos)
{
    this->x = (int)Pos.x;
    this->y = (int)Pos.y;
}

/**
 * Changes the radius of the circle.
 *
 * @param int Radius
 */
void ML_Circle::Resize(u_int r)
{
    this->r = r;
}

/**
 * Checks for collision or overlap with another math::ML_Circle.
 *
 * @param ML_Circle Other circle
 *
 * @return TRUE if overlap, FALSE otherwise.
 */
bool ML_Circle::CheckCollision(const ML_Circle& Other) const
{
    // Compare the distances between the radii
    int dx, dy, rs;
    
    dx = Other.x - this->x;
    dy = Other.y - this->y;
    rs = this->r + Other.r;
    
    return ((dx*dx) + (dy*dy)) < (rs*rs);
}