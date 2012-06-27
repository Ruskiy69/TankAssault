#ifndef MATH_HELPER_H
#define MATH_HELPER_H

/* For calculating collections of Rects and/or Circles. */
#include <vector>

#include "Math/Vector2.h"
#include "Math/Matrix.h"

/* Conversion from radians to degrees */
#define RAD(x) x * PI / 180.0f
#define DEG(x) x * 180.0f / PI
#define PI 3.141592653f

namespace MathHelper
{
    struct Rect
    {
        Rect() : x(0), y(0), w(0), h(0) {}
        Rect(int x, int y) : x(x), y(y), w(0), h(0) {}
        Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
        
        void Move(int x, int y);
        void Move(const Vector2& Position);
        void Resize(int w, int h);
        
        bool CheckCollision(const Rect& Other) const;
    
        int x, y, w, h;
    };
    
    struct Circle
    {
        Circle() : x(0), y(0), r(0) {}
        Circle(int x, int y) : x(x), y(y) {}
        Circle(int x, int y, int r) : x(x), y(y), r(r) {}
        
        void Move(int x, int y);
        void Move(const Vector2& Position);
        void Resize(int r);
        
        bool CheckCollision(const Circle& Other) const;
        
        int x, y, r;    
    };
    
    typedef Rect SDL_Rect;
    typedef std::vector<Rect> Rects;
    typedef std::vector<Circle> Circles;
    
    /* TODO: Functions */
    bool check_collision(const Rect& One, const Circle& Two);
    bool check_collision(const Rect& One, const Rects& Twos);
    bool check_collision(const Rect& One, const Circles& Twos);
    bool check_collision(const Circle& One, const Rects& Two);
    bool check_collision(const Circle& One, const Circles& Two);
}

#endif // MATH_HELPER_H