/**
 * @file
 *  Implementation of the Ray classes.
 *
 * @author  George Kudrayvtsev
 * @version 1.3.1
 **/

#include "Math/Ray.hpp"

using math::CRay2;

CRay2::CRay2(const float start_x, const float start_y, const float end_x, const float end_y)
{
    this->Start.Move(start_x, start_y);
    this->End.Move(end_x, end_y);
}

CRay2::CRay2(const CRay2& Copy)
{
    this->Start = Copy.Start;
    this->End   = Copy.End;
}

CRay2& CRay2::operator= (const CRay2& Copy)
{
    this->Start = Copy.Start;
    this->End   = Copy.End;

    return *this;
}

bool CRay2::operator== (const CRay2& Other) const
{
    return (this->Start == Other.Start && this->End == Other.End);
}

CRay2 CRay2::operator- (const math::CVector2& Other) const
{
    return CRay2(this->Start - Other, this->End - Other);
}

CRay2 CRay2::operator+ (const math::CVector2& Other) const
{
    return CRay2(this->Start + Other, this->End + Other);
}

/**
 * Checks for intersection with another line segment. Touching at
 * endpoints qualifies as intersections.
 *
 * @param math::CRay2 The other line
 *
 * @return TRUE if they intersect, FALSE otherwise.
 * @see http://gamedev.stackexchange.com/questions/26004/how-to-detect-2d-line-on-line-collision
 **/
bool CRay2::CheckCollision(const CRay2& Other, math::CVector2* p_Intersection) const
{
    /**
     * Solve for the intersection point.
     *
     * Derivation:
     *  First line:             y - y1 = m1(x - x1)
     *  Second line:            y - y2 = m2(x - x2)
     *  Equal to each other:    m1x - m1x1 + y1 = m2x - m2x2 + y2
     *  Solve for 'x':          m1x - m2x = -m2x2 + y2 + m1x1 + y1
     *                          x(m1 - m2) = -m2x2 + y2 + m1x1 + y1
     *                          x = (-m2x2 + y2 + m1x1 + y1) / (m1 - m2)
     *  Solve for 'y':          y = m1(x) - m1x1 + y1
     *  Thus (x, y) becomes the intersection point.
     *
     *  Then we must see if (x, y) is in the range of line segments given.
     **/

    math::CVector2 Intersect;
    float m1, m2;

    m1 = this->GetSlope();
    m2 = Other.GetSlope();

    // Test for infinite slopes or parallel lines.
    if(this->Start.x == this->End.x)
        Intersect.x = this->Start.x;
    else if(Other.Start.x == Other.End.x)
        Intersect.x = Other.Start.x;
    else if(m1 == m2)
        return false;
    else
        Intersect.x = (-m2 * Other.Start.x + Other.Start.y + m1 * this->Start.x - this->Start.y) / (m1 - m2);

    Intersect.y = m1 * Intersect.x - m1 * this->Start.x + this->Start.y;

    if(this->OnRay(Intersect) && Other.OnRay(Intersect))
    {
        if(p_Intersection != NULL)
            *p_Intersection = Intersect;
        return true;
    }
    else
        return false;
}

/**
 * @overload math::CRay2::CheckCollision(const CRay2&)
 *
 * @param math::CRect Rectangle
 *
 * @todo Check for collision inside of rect, not just edges.
 **/
bool CRay2::CheckCollision(const math::CRect& Other,
    math::CVector2* p_Intersection) const
{
    // Find all edges of the rectangle.
    CRay2 Top(Other.x, Other.y, Other.x + Other.w, Other.y);
    CRay2 Bottom(Other.x, Other.y + Other.h, Other.x + Other.w, Other.y + Other.h);
    CRay2 Left(Other.x, Other.y, Other.x, Other.y + Other.h);
    CRay2 Right(Other.x + Other.w, Other.y, Other.x + Other.w, Other.y + Other.h);

    // If any edges intersect, we intersect.
    if(this->CheckCollision(Top, p_Intersection))
        return true;
    else if(this->CheckCollision(Bottom, p_Intersection))
        return true;
    else if(this->CheckCollision(Left, p_Intersection))
        return true;
    else if(this->CheckCollision(Right, p_Intersection))
        return true;
    /* Inside of rect, not functional yet.
    else if(this->Start.x >= Right.Start.GetX() &&
        this->Start.x <= Left.Start.GetX() && 
        this->End.x >= Right.Start.GetX() &&
        this->End.x <= Left.Start.GetX())
        return true;
     **/
    else
        return false;
}

/**
 * Rotates the line segment by a certain amount of radians.
 *
 * @param float Angle in radians.
 **/
void CRay2::Rotate(const float angle)
{
    this->Start.Rotate(angle);
    this->End.Rotate(angle);
}

/**
 * Neatly print to stdout the ray start and end cooordinates.
 **/
void CRay2::Print() const
{
    std::cout << "From (" << this->Start.x << ", ";
    std::cout << this->Start.y << ") to (";
    std::cout << this->End.x << ", ";
    std::cout << this->End.y << ")" << std::endl;
}

/**
 * Determines if the given point is on the ray.
 *
 * @param math::CVector2& Point to check.
 *
 * @return TRUE if on ray, FALSE if not.
 **/
bool CRay2::OnRay(const math::CVector2& Point) const
{
    // Final return var
    bool on_line = false;

    // Determine if the current ray goes left-to-right
    // (from Start to End) or right-to-left (from End to Start)
    bool ltr = (this->Start.x < this->End.x);

    // Determine if the current ray goes top-to-bottom
    // (from Start to End) or bottom-to-top (from End to Start)
    bool ttb = (this->Start.y < this->End.y);

    // Check x range
    on_line = ltr ? 
        Point.x >= this->Start.x && 
        Point.x <= this->End.x : 
            Point.x >= this->End.x && 
            Point.x <= this->Start.x;

    if(!on_line)
        return false;
    else
        return (ttb ? 
            Point.y >= this->Start.y && 
            Point.y <= this->End.y : 
                Point.y >= this->End.y && 
                Point.y <= this->Start.y);
}

/**
 * Calculates the slope of the ray. Line segments with infinite
 * slopes present undefined behavior.
 *
 * @pre Non-infinite slope.
 * @return Slope.
 **/
float CRay2::GetSlope() const
{
    if(this->End.x - this->Start.x == 0)
        return 0;

    return (this->End.y - this->Start.y) / (this->End.x - this->Start.x);
}

/**
 * Calculates the length of the ray using the Pythagorean theorum.
 *
 * @return Length.
 **/
float CRay2::GetLength() const
{
    float x = this->Start.x - this->End.x;
    float y = this->Start.y - this->End.y;

    return sqrt(x*x + y*y);
}

/**
 * Calculate every single point on the line segment.
 *
 * @return A vector of the points.
 **/
std::vector<math::CVector2*> CRay2::GetPoints() const
{
    std::vector<math::CVector2*> ppoints;
    
    int dx, dy, inx, iny, e;

    math::CVector2 TmpStart(this->Start);

    dx = this->End.y - this->Start.x;
    dy = this->End.y - this->Start.x;
    inx = dx > 0 ? 1 : -1;
    iny = dy > 0 ? 1 : -1;
 
    dx = abs(dx);
    dy = abs(dy);
     
    if(dx >= dy)
    {
        dy <<= 1;
        e = dy - dx;
        dx <<= 1;
        while((int)TmpStart.x != (int)this->End.x)
        {
            ppoints.push_back(new math::CVector2(TmpStart));
            if(e >= 0)
            {
                TmpStart.y = TmpStart.y + iny;
                e -= dx;
            }
            e += dy;
            TmpStart.x = TmpStart.x + inx;
        }
    } 
    else
    {
        dx <<= 1;
        e = dx - dy;
        dy <<= 1;
        while ((int)TmpStart.y != (int)End.y)
        {
            ppoints.push_back(new math::CVector2(TmpStart));
            if(e >= 0)
            {
                TmpStart.x = TmpStart.x + inx;
                e -= dy;
            }
            e += dx;
            TmpStart.y = TmpStart.y + iny;
        }
    }
    
    return ppoints;
}