/**
 * @file
 *	Declarations for several shape classes.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Math
 */
/// @{

#ifndef SHAPES_H
#define SHAPES_H

#include "Math/MathDef.h"
#include "Math/Vector.h"

namespace math
{
    /// A rectangle.
    struct ML_Rect
    {
        ML_Rect() : x(0), y(0), w(0), h(0) {}
        ML_Rect(int x, int y) : x(x), y(y), w(0), h(0) {}
        ML_Rect(int x, int y, u_int w, u_int h) : x(x), y(y), w(w), h(h) {}

        bool operator== (const ML_Rect& Other);

        void Move(int x, int y);
        void Move(const ML_Vector2& Position);
        void Resize(u_int w, u_int h);

        bool CheckCollision(const ML_Rect& Other) const;
        bool IsCovering(const ML_Rect& Other) const;

        int x, y;
        u_int w, h;
    };

    /// A rectangle, supporting floating point numbers.
    struct ML_Rectf
    {
        ML_Rectf() : x(0), y(0), w(0), h(0) {}
        ML_Rectf(float x, float y) : x(x), y(y), w(0), h(0) {}
        ML_Rectf(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

        bool operator== (const ML_Rectf& Other);

        void Move(float x, float y);
        void Move(const ML_Vector2& Position);
        void Resize(float w, float h);

        bool CheckCollision(const ML_Rectf& Other) const;
        bool IsCovering(const ML_Rect& Other) const;

        float x, y;
        float w, h;
    };

    /// A circle.
    struct ML_Circle
    {
        ML_Circle() : x(0), y(0), r(0) {}
        ML_Circle(int x, int y) : x(x), y(y) {}
        ML_Circle(int x, int y, int r) : x(x), y(y), r(r) {}

        void Move(int x, int y);
        void Move(const ML_Vector2& Position);
        void Resize(u_int r);

        bool CheckCollision(const ML_Circle& Other) const;

        int x, y, r;    
    };

    /// Overrides the SDL implementation.
    typedef math::ML_Rect SDL_Rect;
}


#endif // SHAPES_H

/// @}