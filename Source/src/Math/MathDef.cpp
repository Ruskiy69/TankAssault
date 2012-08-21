/**
 * @file
 *	Definitions for math helper functions found in MathDef.h
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 */

#include "Math/MathDef.h"

/**
 * Converts a number from degrees to radians.
 *
 * @param float The number in degrees.
 *
 * @return The number in radians.
 */
float math::rad(const float x)
{
    return x * math::PI / 180.0f;
}

/**
 * Converts a number from radians to degrees.
 *
 * @param float The number in radians.
 *
 * @return The number in degrees.
 */
float math::deg(const float x)
{
    return x * 180.0f / math::PI;
}

/**
 * Calculates the distance formula.
 *
 * @param float First x-coordinate
 * @param float First y-coordinate
 * @param float Second x-coordinate
 * @param float Second y-coordinate
 * @param bool Whether or not to do the square root (optional)
 *
 * @return Distance.
 */
float math::distance(const float x1, const float y1, const float x2, const float y2, bool do_sqrt)
{
    if(do_sqrt)
        return sqrt(((x1 - x2)*(x1 - x2)) + 
            ((y1 - y2)*(y1 - y2)));
    else
        return ((x1 - x2)*(x1 - x2)) + 
            ((y1 - y2)*(y1 - y2));
}