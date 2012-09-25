/**
 * @file
 *  Vector class declarations.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
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

#ifndef MATH__VECTOR3_HPP
#define MATH__VECTOR3_HPP

#include <iostream>

#include "Errors.hpp"
#include "Math/MathDef.hpp"

namespace math
{
    /**
     * Three dimensional vector.
     * @todo Implement this.
     **/
    class CVector3
    {
    public:
        CVector3() : x(0), y(0), z(0) {}
        CVector3(float x, float y, float z) : x(x), y(y), z(z) {}
        
        bool operator== (const CVector3& Other) const;
        bool operator!= (const CVector3& Other) const;
        CVector3& operator= (const CVector3& Other);
        
        /* None of these actually modify the current CVector3.
         * Example: 
         * CVector3 A(3, 4, 5), B(6, 7, 8);
         * A + B;       // Does nothing
         * A = A + B    // A is now <9, 11, 13>
         **/
        CVector3 operator+ (const CVector3& Other) const;   // Vector addition
        CVector3 operator- (const CVector3& Other) const;   // Vector subtraction
        CVector3 operator/ (const CVector3& Other) const;   // Vector division
        CVector3 operator/ (const float scalar)    const;   // Scalar division
        CVector3 operator* (const float scalar)    const;   // Scalar multiplier
        float    operator* (const CVector3& Other) const;   // Dot product

        void Print() const;                                 // Print vector in <x, y, z> format

        void Move(const float x, const float y, const float z);
        void Move(const CVector3& Position);                 

        void Rotate(const float angle);                     // Rotate vector by some amount
        
        CVector3 Normal() const;                            // Get the normal vector
        void Normalize();                                   // Actually change the current vector to its normal
        CVector3 Cross(const CVector3& Other);              // Cross product
        float Dot(const CVector3& Other) const;             // Dot product
        float Magnitude() const;                            // Length of vector
        
        float x, y, z;
    };

    /**
     * Calculates the distances between two points.
     *  This is a re-implementation of the distance() function using vectors.
     * 
     * @param math::CVector3& First point
     * @param math::CVector3& Second point
     * @param bool Do the square-root calculation (optional) (default=true)
     *
     * @return The distance.
     **/
    float distance(const math::CVector3& One,
        const math::CVector3& Two,
        bool do_sqrt = true);
}

#endif // MATH__VECTOR3_HPP

/// @}