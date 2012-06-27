#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

namespace Game
{
    class Game_Timer
    {
    public:
        Game_Timer();
        ~Game_Timer();

        void Start();
        float GetTicks();
        int GetFrame() const;
        void SetFrameRate(const unsigned int new_fps){ this->FRAME_RATE = new_fps; }
        int  GetFrameRate() const{ return this->FRAME_RATE; }
        void DelayFPS();

    private:
        unsigned int FRAME_RATE;
        int ticks;
        double frame;
    };
}

#endif // TIMER_H