/**
 * @file
 *	Function definitions for various calculations and conversions.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Math
 */
/// @{

#ifndef MATH_DEF_H
#define MATH_DEF_H

#include <cmath>
#include "Errors.h"

/// Couldn't find it in any default headers
typedef unsigned int u_int;

/// Contains all math-related data.
namespace math
{
    /// Well-known constant. Special thanks to Windows calculator.
    static const float PI = 3.14159265358979323846f;

    /**
     * Convert from radians to degrees.
     *
     * @param float Number to convert
     * 
     * @return Converted number.
     */
    float rad(const float x);

    /**
     * Convert from degrees to radians.
     *
     * @param float Number to convert
     * 
     * @return Converted number.
     */
    float deg(const float x);

    /**
     * @overload distance(const math::ML_Vector2&, const math::ML_Vector2&, bool)
     */
    float distance(const float x1, const float y1,
        const float x2, const float y2,
        bool do_sqrt = true);
}

#endif // MATH_DEF_H

/// @}