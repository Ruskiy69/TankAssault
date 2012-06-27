#include "Math/Vector2.h"

using MathHelper::Vector2;

bool Vector2::operator== (const Vector2& Other) const
{
    return (this->x == Other.GetX()) && (this->y == Other.GetY());
}

Vector2& Vector2::operator= (const Vector2& Other)
{
    this->Move(Other.GetX(), Other.GetY());
    return *this;
}

Vector2 Vector2::operator+ (const Vector2& Other)
{
    return Vector2(this->x + Other.GetX(), this->y + Other.GetY());
}

Vector2 Vector2::operator- (const Vector2& Other)
{
    return Vector2(this->x - Other.GetX(), this->y - Other.GetY());
}

Vector2 Vector2::operator/ (const Vector2& Other)
{
    return Vector2(this->x / Other.GetX(), this->y / Other.GetY());
}

Vector2 Vector2::operator* (const float scalar)
{
    return Vector2(this->x * scalar, this->y * scalar);
}

float Vector2::operator* (const Vector2& Other)
{
    return this->Dot(Other);
}

void Vector2::Print()
{
    printf("<%0.2f, %0.2f>\n", this->GetX(), this->GetY());
}

void Vector2::Move(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

void Vector2::Move(const Vector2& Pos)
{
    this->x = Pos.GetX();
    this->y = Pos.GetY();
}

Vector2 Vector2::Normal()
{
    Vector2 Norm;
    Norm.SetX(this->GetX() / this->Magnitude());
    Norm.SetY(this->GetY() / this->Magnitude());

    return Norm;
}

float Vector2::Dot(const Vector2& Other) const
{
    return (this->x * Other.GetX()) + (this->y * Other.GetY());
}

float Vector2::Magnitude() const
{
    return this->magnitude;
}

float Vector2::GetX() const
{
    return this->x;
}

float Vector2::GetY() const
{
    return this->y;
}

void Vector2::SetX(float x)
{
    this->x = x;
}

void Vector2::SetY(float y)
{
    this->y = y;
}