/**
 * @file
 *	Declarations for several shape classes.
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

#ifndef MATH__SHAPES_HPP
#define MATH__SHAPES_HPP

#include "Math/MathDef.hpp"
#include "Math/Vector2.hpp"

namespace math
{
    /// A rectangle.
    struct CRect
    {
        CRect() : x(0), y(0), w(0), h(0) {}
        CRect(int x, int y) : x(x), y(y), w(0), h(0) {}
        CRect(int x, int y, u_int w, u_int h) : x(x), y(y), w(w), h(h) {}

        bool operator== (const CRect& Other);

        void Move(int x, int y);
        void Move(const CVector2& Position);
        void Resize(u_int w, u_int h);

        bool CheckCollision(const CRect& Other) const;
        bool IsCovering(const CRect& Other) const;

        int x, y;
        u_int w, h;
    };

    /// A rectangle, supporting floating point numbers.
    struct CRectf
    {
        CRectf() : x(0), y(0), w(0), h(0) {}
        CRectf(float x, float y) : x(x), y(y), w(0), h(0) {}
        CRectf(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

        bool operator== (const CRectf& Other);

        void Move(float x, float y);
        void Move(const CVector2& Position);
        void Resize(float w, float h);

        bool CheckCollision(const CRectf& Other) const;
        bool IsCovering(const CRect& Other) const;

        float x, y;
        float w, h;
    };

    /// A circle.
    struct CCircle
    {
        CCircle() : x(0), y(0), r(0) {}
        CCircle(int x, int y) : x(x), y(y) {}
        CCircle(int x, int y, int r) : x(x), y(y), r(r) {}

        void Move(int x, int y);
        void Move(const CVector2& Position);
        void Resize(u_int r);

        bool CheckCollision(const CCircle& Other) const;

        int x, y, r;    
    };

    /// Overrides the SDL implementation.
    typedef math::CRect SDL_Rect;
}


#endif // MATH__SHAPES_HPP

/// @}