/** 
 * @file
 *  Timer class definitions.
 *
 * @author  George Kudrayvtsev
 * @version 1.0.1
 *
 **/
#include "Timer.hpp"

using game::CTimer;

/**
 * Starts the timer.
 *  This should be called at the beginning of the game loop.
 *
 * @see CTimer::GetTicks()
 * @see CTimer::DelayFPS()
 **/
void CTimer::Start()
{
    m_ticks = SDL_GetTicks();
    m_frame++;
}

/**
 * Pauses the application to maintain frame rate.
 *  This should be called at the end of the game loop.
 **/
void CTimer::DelayFPS()
{
    if(this->GetTicks() < 1000.0f / m_FRAMERATE)
    {
        SDL_Delay((int)((1000.0f / m_FRAMERATE) - this->GetTicks()));
    }
}

void CTimer::SetFrameRate(const u_int new_fps)
{
    m_FRAMERATE = new_fps;
}

/**
 * Calculates number of milliseconds since the last call.
 * @return Duration (in milliseconds) since last call of CTimer::GetTicks().
 **/
float CTimer::GetTicks()
{
    Uint32 old_ticks = m_ticks;
    m_ticks = SDL_GetTicks();
    return 1.0f * (m_ticks - old_ticks);
}

int CTimer::GetFrame() const
{
    return m_frame;
}

int CTimer::GetFrameRate() const
{
    return m_FRAMERATE;
}