/** 
 * @file
 *  Timer class definitions.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 *
 */
#include "Timer.h"

using game::CL_Timer;

/**
 * Starts the timer.
 *
 * This should be called at the beginning of the game loop.
 *
 * @see CL_Timer::GetTicks()
 * @see CL_Timer::DelayFPS()
 */
void CL_Timer::Start()
{
    m_ticks = SDL_GetTicks();
    m_frame++;
}

/**
 * Pauses the application to maintain frame rate.
 *
 * This should be called at the end of the game loop.
 */
void CL_Timer::DelayFPS()
{
    if(this->GetTicks() < 1000.0f / m_FRAMERATE)
    {
        SDL_Delay((int)((1000.0f / m_FRAMERATE) - this->GetTicks()));
    }
}

/**
 * Sets a new frame rate
 *
 * @param int new frame rate
 */
void CL_Timer::SetFrameRate(const u_int new_fps)
{
    m_FRAMERATE = new_fps;
}

/**
 * Calculates number of milliseconds since the last call
 *
 * @return Duration (in milliseconds) since last call of CL_Timer::GetTicks().
 */
float CL_Timer::GetTicks()
{
    Uint32 old_ticks = m_ticks;
    m_ticks = SDL_GetTicks();
    return 1.0f * (m_ticks - old_ticks);
}

/**
 * Retrieves current frame
 *
 * @return Current frame.
 */
int CL_Timer::GetFrame() const
{
    return m_frame;
}

/**
 * Retrieves current frame rate
 *
 * @return Frame rate.
 */
int CL_Timer::GetFrameRate() const
{
    return m_FRAMERATE;
}