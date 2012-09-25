/**
 * @file
 *  Definitions for the Vector class
 *
 * @author  George Kudrayvtsev
 * @version 1.5.1
 **/

#include "Math/Vector2.hpp"

using math::CVector2;

/**
 * Checks a given vector for equality with this one.
 *  Floating point comparisons tend to be fairly inaccurate,
 *  so be wary when using this.
 *
 * @return TRUE if position is the same, FALSE otherwise.
 **/
bool CVector2::operator== (const CVector2& Other) const
{
    return (this->x == Other.x) && (this->y == Other.y);
}

/**
 * Checks a given vector for inequality with this one.
 *  Floating point comparisons tend to be fairly inaccurate,
 *  so be wary when using this.
 *
 * @return TRUE if position isn't the same, FALSE otherwise.
 **/
bool CVector2::operator!= (const CVector2& Other) const
{
    return (this->x != Other.x) || (this->y != Other.y);
}

/**
 * Checks if the current vector's (x, y) coordinates are greater
 * a given vector's (x, y).
 *
 * @return TRUE if so, FALSE if not.
 **/
bool CVector2::operator> (const CVector2& Other) const
{
    return (this->x > Other.x && this->y > Other.y);
}

/**
 * Checks if the current vector's (x, y) coordinates are less
 * a given vector's (x, y).
 *
 * @return TRUE if so, FALSE if not.
 **/
bool CVector2::operator< (const CVector2& Other) const
{
    return (this->x < Other.x && this->y < Other.y);
}

/**
 * Assigns a given vector as the current one.
 * @return The current vector with new values.
 **/
CVector2& CVector2::operator= (const CVector2& Other)
{
    this->Move(Other.x, Other.y);
    return *this;
}

/**
 * Adds a given vector with the current one.
 * @return The newly created vector with added parts.
 **/
CVector2 CVector2::operator+ (const CVector2& Other) const
{
    return CVector2(this->x + Other.x, this->y + Other.y);
}

/**
 * Subtracts a given vector from the current one.
 * @return The newly created vector with subtracted parts.
 **/
CVector2 CVector2::operator- (const CVector2& Other) const
{
    return CVector2(this->x - Other.x, this->y - Other.y);
}

/**
 * Divides a given vector from the current one.
 * @return The newly created vector with divided parts.
 **/
CVector2 CVector2::operator/ (const CVector2& Other) const
{
    return CVector2(this->x / Other.x, this->y / Other.y);
}

/**
 * Divides the current vector with a scalar.
 * @return The newly created vector with scalar divided parts.
 **/
CVector2 CVector2::operator/ (const float scalar) const
{
    return CVector2(this->x / scalar, this->y / scalar);
}

/**
 * Multiplies the given vector by a scalar.
 * @return The newly created vector with the scalar-multiplied parts.
 **/
CVector2 CVector2::operator* (const float scalar) const
{
    return CVector2(this->x * scalar, this->y * scalar);
}

/**
 * Evaluates the dot product of the current vector and a given one.
 * @return The dot product.
 **/
float CVector2::operator* (const CVector2& Other) const
{
    return this->Dot(Other);
}

/**
 * Prints the vector values in a neat format.
 **/
void CVector2::Print() const
{
    std::cout.precision(4);
    std::cout << "<" << this->x << ", " << this->y << ">" << std::endl;
}

/** 
 * Moves the vector to a specified position.
 *
 * @param float X-coordinate
 * @param float Y-coordinate
 **/
void CVector2::Move(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

/** 
 * Moves the vector to a specified position.
 * @param CVector2 Position
 **/
void CVector2::Move(const CVector2& Pos)
{
    this->x = Pos.x;
    this->y = Pos.y;
}

/**
 * Rotate the vector by a certain angle (in radians).
 * @param float Angle
 **/
void CVector2::Rotate(const float rad_angle)
{
    float final_x = (this->x * cos(rad_angle)) - (this->y * sin(rad_angle));
    float final_y = (this->x * sin(rad_angle)) + (this->y * cos(rad_angle));

    // We negate the X value because the OpenGL coordinate system
    // makes the origin in the top left instead of the bottom left.
    this->Move(-final_x, final_y);
}

/**
 * Calculates the unit vector.
 * @return The unit vector.
 * @see CVector2::Normalize()
 **/
CVector2 CVector2::Normal() const
{
    CVector2 Norm;
    Norm.x = this->x / this->Magnitude();
    Norm.y = this->y / this->Magnitude();

    return Norm;
}

/**
 * Calculates the unit vector and sets it to the current one.
 * @see CVector2::Normal()
 **/
void CVector2::Normalize()
{
    float magnitude = this->Magnitude();
    this->x /= magnitude;
    this->y /= magnitude;
}

/**
 * Calculates the 2D cross product with another vector. This
 * is equivalent to the magnitude of the 3D cross product. The formula is
 * such:
 *  U x V = Ux * Vy - Uy * Vx
 *
 * @param math::CVector2& Vector to do cross product with
 *
 * @return Float representing the solution.
 * @see http://www.gamedev.net/topic/289972-cross-product-of-2d-vectors/
 **/
//float CVector2::Cross(const CVector2& Other) const
//{
//    return (this->x * Other.y) - (this->y * Other.x);
//}

/** 
 * Calculates the dot product between a given vector and the current one.
 *
 * @return Dot product.
 * @see CVector2::operator*()
 **/
float CVector2::Dot(const CVector2& Other) const
{
    return (this->x * Other.x) + (this->y * Other.y);
}

/**
 * Calculates the magnitude.
 *  Magnitude is sqrt(x^2 + y^2)
 *
 * @return The magnitude.
 **/
float CVector2::Magnitude() const
{
    return sqrt(this->x*this->x + this->y*this->y);
}

/**
 * Calculates the distance formula.
 *
 * @param math::CVector2& First point
 * @param math::CVector2& Second point
 * @param bool Whether or not to do the square root (optional)
 *
 * @return Distance.
 **/
float math::distance(const math::CVector2& One,
    const math::CVector2& Two, bool do_sqrt)
{
    float value = ((One.x - Two.x) * (One.x - Two.x)) + 
        ((One.y - Two.y) * (One.y - Two.y));

    return do_sqrt ? sqrt(value) : value;
}