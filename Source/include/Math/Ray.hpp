/**
 * @file
 *  Declarations for the Ray class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.3
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

#ifndef MATH__MATH_RAY_HPP
#define MATH__MATH_RAY_HPP

#include <vector>

#include "Math/MathDef.hpp"
#include "Math/Shapes.hpp"
#include "Math/Vector2.hpp"

namespace math
{
    /**
     * A class made to represent a 2-d ray or line segment.
     **/
    class CRay2
    {
    public:
        CRay2(math::CVector2& Start, math::CVector2& End) : Start(Start), End(End) {}
        CRay2(const float start_x, const float start_y,
            const float end_x, const float end_y);

        CRay2(const CRay2& Copy);
        CRay2& operator= (const CRay2& Copy);
        
        CRay2 operator- (const CVector2& Other) const;
        CRay2 operator+ (const CVector2& Other) const;
        bool operator== (const CRay2& Other)    const;

        bool CheckCollision(const CRay2& Other,
            math::CVector2* p_Intersection = NULL) const;
        bool CheckCollision(const CRect& Other,
            math::CVector2* p_Intersection = NULL) const;

        void Rotate(const float angle);
        void Print() const;

        bool OnRay(const math::CVector2& Point) const;

        float GetSlope()  const;
        float GetLength() const;
        std::vector<CVector2*> GetPoints() const;

        CVector2 Start;
        CVector2 End;
    };
}

#endif // MATH__MATH_RAY_HPP

/// @}