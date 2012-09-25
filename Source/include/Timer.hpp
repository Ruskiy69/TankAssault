/**
 * @file
 *  Declarations for the Timer class.
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
 * @addtogroup Game
 **/
/// @{

#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

typedef unsigned int u_int;

namespace game
{
    /// A timer for controlling frame rates.
    class CTimer
    {
    public:
        CTimer() : m_FRAMERATE(60), m_ticks(0), m_frame(0) {}
        
        /**
         * Starts the timer.
         *  This should be called at the start of every frame, with
         *  DelayFPS() called at the end of it in order to maintain
         *  a consistent frame rate.
         **/
        void Start();

        /**
         * Delays the current frame to be in accordance with the frame rate.
         *  This should be called at the end of every frame after Start()
         *  was called at the beginning.
         **/
        void DelayFPS();
        
        /**
         * Retrieves the amount of time (in ms) that has elapsed
         * since the last call to Start().
         *  
         * @return Time elapsed.
         **/
        float   GetTicks();

        int     GetFrame() const;
        int     GetFrameRate() const;
        void    SetFrameRate(const u_int new_fps);

    private:
        u_int m_FRAMERATE;
        int m_ticks;
        int m_frame;
    };
}

#endif // TIMER_H

/// @}