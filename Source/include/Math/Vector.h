/**
 * @file
 *  Vector class declarations.
 *
 * @author George Kudrayvtsev
 * @version 1.5.1
 *
 * @addtogroup Math
 */
/// @{

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

#include "Errors.h"
#include "Math/MathDef.h"

namespace math
{
    /**
     * Three dimensional vector.
     * @todo Implement this.
     */
    class Vector3
    {
    public:
        Vector3() : x(0), y(0), z(0), magnitude(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z),
            magnitude(sqrt(x*x + y*y + z*z)) {}
        
        bool operator== (const Vector3& Other) const;
        bool operator!= (const Vector3& Other) const;
        Vector3& operator= (const Vector3& Other);
        
        /* None of these actually modify the current Vector3.
         * Example: 
         * Vector3 A(3, 4, 5), B(6, 7, 8);
         * A + B;       // Does nothing
         * A = A + B    // A is now <9, 11, 13>
         */
        Vector3 operator+ (const Vector3& Other) const;     // Vector addition
        Vector3 operator- (const Vector3& Other) const;     // Vector subtraction
        Vector3 operator/ (const Vector3& Other) const;     // Vector division
        Vector3 operator/ (const float scalar) const;       // Scalar division
        Vector3 operator* (const float scalar) const;       // Scalar multiplier
        float   operator* (const Vector3& Other) const;     // Dot product

        void Print() const;                                 // Print vector in <x, y, z> format

        void Move(const float x, const float y, const float z);
        void Move(const Vector3& Position);                 

        void Rotate(const float angle);                     // Rotate vector by some amount
        
        Vector3 Normal() const;                             // Get the normal vector
        void Normalize();                                   // Actually change the current vector to its normal
        //Vector3 Cross(const ML_Vector2& Other);                // Cross product
        float Dot(const Vector3& Other) const;              // Dot product
        float Magnitude() const;                            // Length of vector
        
        float GetX() const;
        float GetY() const;
        float GetZ() const;
        
        void SetX(float x);
        void SetY(float y);
        void SetZ(float z);
        
    private:
        float magnitude;
        float x, y, z;
    };

    /**
     * Two dimensional vector.
     */
    struct ML_Vector2
    {
    public:
        ML_Vector2() : x(0), y(0) {}
        ML_Vector2(float x, float y) : x(x), y(y) {}
        ML_Vector2(int x, int y) : x(x * 1.0f), y(y * 1.0f) {}
        
        bool operator== (const ML_Vector2& Other) const;
        bool operator!= (const ML_Vector2& Other) const;
        bool operator<  (const ML_Vector2& Other) const;
        bool operator>  (const ML_Vector2& Other) const;
        ML_Vector2& operator= (const ML_Vector2& Other);
        
        /* None of these actually modify the current ML_Vector2.
         * Example: 
         * ML_Vector2 A(3, 4), B(6, 7);
         * A + B;       // Does nothing
         * A = A + B    // A is now <9, 11>
         */
        ML_Vector2 operator+ (const ML_Vector2& Other) const; // Vector addition
        ML_Vector2 operator- (const ML_Vector2& Other) const; // Vector subtraction
        ML_Vector2 operator/ (const ML_Vector2& Other) const; // Vector division
        ML_Vector2 operator/ (const float scalar) const;   // Scalar division
        ML_Vector2 operator* (const float scalar) const;   // Scalar multiplier
        float   operator* (const ML_Vector2& Other) const; // Dot product

        void Print() const;                             // Print vector in <x, y> format

        void Move(const float x, const float y);        // These two are 
        void Move(const ML_Vector2& Position);             // fairly self-explanatory

        void Rotate(const float rad_angle);                 // Rotate vector by some amount
        
        ML_Vector2 Normal() const;                         // Get the normal vector
        void Normalize();                               // Actually change the current vector to its normal
        float Cross(const ML_Vector2& Other) const;
        float Dot(const ML_Vector2& Other) const;          // Dot product
        float Magnitude() const;                        // Length of vector
        
        float x, y;
    };

    /**
     * Calculates the distances between two points.
     *  This is a re-implementation of the distance() function using vectors.
     * 
     * @param math::ML_Vector2& First point
     * @param math::ML_Vector2& Second point
     * @param bool Do the square-root calculation (optional) (default=true)
     *
     * @return The distance.
     */
    float distance(const math::ML_Vector2& One,
        const math::ML_Vector2& Two,
        bool do_sqrt = true);
}

#endif // VECTOR_H

/// @}