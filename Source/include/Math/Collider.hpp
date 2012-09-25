/**
 * @file
 *	A collection of functions for handling collision detection
 *	between various shapes, lines, and points.
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

#ifndef MATH__COLLIDER_HPP
#define MATH__COLLIDER_HPP

#include "Math/MathDef.hpp"
#include "Math/Shapes.hpp"

namespace math
{
    /// @todo Implement these functions.

    /**
     * A class designed for checking for collisions between
     * circles, rectangles, lines, and points.
     * It holds no data internally, but rather just provides a
     * collection of static methods.
     **/
    class CCollider
    {
    public:
        static bool CircleWithCircle(CCircle& One, CCircle& Two);
        static bool CircleWithLine(CCircle& One, CRay2& Two);
        static bool CircleWithPoint(CCircle& One, CVector2& Two);
        static bool CircleWithRect(CCircle& One, CRect& Two);

        static bool LineBetweenLines(CRay2& Line,
            CRay2& CheckOne, CRay2& CheckTwo);
        static bool LineWithLine(CRay2& One, CRay2& Two);
        static bool LineWithPoint(CRay2& One, CVector2& Two);

        static bool PointBetweenLines(CVector2& Point,
            CRay2& CheckOne, CRay2& CheckTwo);

        static bool RectWithCircle(CRect& One, CCircle& Two);
        static bool RectWithLine(CRect& One, CRay2& Two);
        static bool RectWithPoint(CRect& One, CVector2& Two);
        static bool RectWithRect(CRect& One, CRect& Two);
    };
}

#endif // MATH__COLLIDER_HPP

/// @}