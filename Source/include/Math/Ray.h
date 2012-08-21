/**
 * @file
 *  Declarations for the Ray class.
 *
 * @author George Kudrayvtsev
 * @version 1.3.0
 *
 * @addtogroup Math
 */
/// @{

#ifndef RAY_H
#define RAY_H

#include <vector>

#include "Math/MathDef.h"
#include "Math/Shapes.h"
#include "Math/Vector.h"

namespace math
{
    /**
     * A class made to represent a 2-d ray or line segment.
     */
    class ML_Ray2
    {
    public:
        ML_Ray2(math::ML_Vector2& Start, math::ML_Vector2& End) : Start(Start), End(End) {}
        ML_Ray2(const float start_x, const float start_y,
            const float end_x, const float end_y);

        ML_Ray2(const ML_Ray2& Copy);
        ML_Ray2& operator= (const ML_Ray2& Copy);
        
        ML_Ray2 operator- (const ML_Vector2& Other) const;
        ML_Ray2 operator+ (const ML_Vector2& Other) const;
        bool operator== (const ML_Ray2& Other) const;

        bool CheckCollision(const ML_Ray2& Other,
            math::ML_Vector2* p_Intersection = NULL) const;
        bool CheckCollision(const ML_Rect& Other,
            math::ML_Vector2* p_Intersection = NULL) const;

        void Rotate(const float angle);
        void Print() const;

        bool OnRay(const math::ML_Vector2& Point) const;

        float GetSlope() const;
        float GetLength() const;
        std::vector<ML_Vector2*> GetPoints() const;

        ML_Vector2 Start;
        ML_Vector2 End;
    };
}

#endif // RAY_H

/// @}