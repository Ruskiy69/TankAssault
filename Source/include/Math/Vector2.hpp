/**
 * @file
 *  CVector2 class declarations.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.5.1
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Math
 **/
/// @{

#ifndef MATH__VECTOR2_HPP
#define MATH__VECTOR2_HPP

#include <iostream>

#include "Errors.hpp"
#include "Math/MathDef.hpp"

namespace math
{
    class CVector3;

    /// Two dimensional vector.
    struct CVector2
    {
    public:
        CVector2() : x(0), y(0) {}
        CVector2(float x, float y) : x(x), y(y) {}
        CVector2(int x, int y) : x(x * 1.0f), y(y * 1.0f) {}
        
        bool operator== (const CVector2& Other) const;
        bool operator!= (const CVector2& Other) const;
        bool operator<  (const CVector2& Other) const;
        bool operator>  (const CVector2& Other) const;
        CVector2& operator= (const CVector2& Other);
        
        /* None of these actually modify the current CVector2.
         * Example: 
         * CVector2 A(3, 4), B(6, 7);
         * A + B;       // Does nothing
         * A = A + B    // A is now <9, 11>
         **/
        CVector2 operator+ (const CVector2& Other) const;   // Vector addition
        CVector2 operator- (const CVector2& Other) const;   // Vector subtraction
        CVector2 operator/ (const CVector2& Other) const;   // Vector division
        CVector2 operator/ (const float scalar) const;      // Scalar division
        CVector2 operator* (const float scalar) const;      // Scalar multiplier
        float   operator* (const CVector2& Other) const;    // Dot product

        void Print() const;                                 // Print vector in <x, y> format

        void Move(const float x, const float y);            // These two are 
        void Move(const CVector2& Position);                // fairly self-explanatory

        void Rotate(const float rad_angle);                 // Rotate vector by some amount
        
        CVector3 Cross(const CVector2& Other) const;        // Cross product
        CVector2 Normal() const;                            // Get the normal vector
        void Normalize();                                   // Actually change the current vector to its normal
        float Dot(const CVector2& Other) const;             // Dot product
        float Magnitude() const;                            // Length of vector
        
        float x, y;
    };

    /**
     * Calculates the distances between two points.
     *  This is a re-implementation of the distance() function using vectors.
     * 
     * @param math::CVector2& First point
     * @param math::CVector2& Second point
     * @param bool Do the square-root calculation (optional) (default=true)
     *
     * @return The distance.
     **/
    float distance(const math::CVector2& One,
        const math::CVector2& Two,
        bool do_sqrt = true);
}

#endif // MATH__VECTOR2_HPP