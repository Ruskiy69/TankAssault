#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>
#include <cstdio>

namespace MathHelper
{
    class Vector2
    {
    public:
        Vector2() : x(0), y(0), magnitude(0) {}
        Vector2(float x, float y) : x(x), y(y), magnitude(sqrt(x*x + y*y)) {}
        
        bool operator== (const Vector2& Other) const;
        Vector2& operator= (const Vector2& Other);
        
        /* None of these actually modify the current Vector2.
         * Example: 
         * Vector2 A(3, 4), B(6, 7);
         * A + B;       // Does nothing
         * A = A + B    // A is now <9, 11>
         */
        Vector2 operator+ (const Vector2& Other);   // Vector addition
        Vector2 operator- (const Vector2& Other);   // Vector subtraction
        Vector2 operator/ (const Vector2& Other);   // Vector division
        Vector2 operator* (const float scalar);     // Scalar multiplier
        float   operator* (const Vector2& Other);   // Dot product

        void Print();                               // Print vector in <x, y> format

        void Move(const float x, const float y);    // These two are 
        void Move(const Vector2& Position);         // fairly self-explanatory
        
        Vector2 Normal();                           // Normal vector
        //Vector3 Cross(const Vector2& Other);        // Cross product
        float Dot(const Vector2& Other) const;      // Dot product
        float Magnitude() const;                    // Length of vector
        
        float GetX() const;
        float GetY() const;
        
        void SetX(float x);
        void SetY(float y);
        
    private:
        float magnitude;
        float x, y;
    };
}

#endif // VECTOR_2_H