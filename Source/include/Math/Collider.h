/**
 * @file
 *	A collection of functions for handling collision detection
 *	between various shapes, lines, and points.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Math
 */
/// @{

#ifndef COLLIDER_H
#define COLLIDER_H

#include "Math/MathDef.h"
#include "Math/Shapes.h"

namespace math
{
    /// @todo Implement these functions.

    /**
     * A class designed for checking for collisions between
     * circles, rectangles, lines, and points.
     * It holds no data internally, but rather just provides a
     * collection of static methods.
     */
    class ML_Collider
    {
    public:
        static bool CircleWithCircle(ML_Circle& One, ML_Circle& Two);
        static bool CircleWithLine(ML_Circle& One, ML_Ray2& Two);
        static bool CircleWithPoint(ML_Circle& One, ML_Vector2& Two);
        static bool CircleWithRect(ML_Circle& One, ML_Rect& Two);

        static bool LineBetweenLines(ML_Ray2& Line,
            ML_Ray2& CheckOne, ML_Ray2& CheckTwo);
        static bool LineWithLine(ML_Ray2& One, ML_Ray2& Two);
        static bool LineWithPoint(ML_Ray2& One, ML_Vector2& Two);

        static bool PointBetweenLines(ML_Vector2& Point,
            ML_Ray2& CheckOne, ML_Ray2& CheckTwo);

        static bool RectWithCircle(ML_Rect& One, ML_Circle& Two);
        static bool RectWithLine(ML_Rect& One, ML_Ray2& Two);
        static bool RectWithPoint(ML_Rect& One, ML_Vector2& Two);
        static bool RectWithRect(ML_Rect& One, ML_Rect& Two);
    };
}

#endif // COLLIDER_H

/// @}