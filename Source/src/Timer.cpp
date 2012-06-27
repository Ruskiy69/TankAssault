#include "Timer.h"

using Game::Game_Timer;

Game_Timer::Game_Timer()
{
    this->FRAME_RATE    = 45;
    this->ticks         = 0;
    this->frame         = 0;
}

Game_Timer::~Game_Timer()
{}

void Game_Timer::Start()
{
    this->ticks = SDL_GetTicks();
    this->frame++;
}

float Game_Timer::GetTicks()
{
    Uint32 oldTicks = this->ticks;
    this->ticks = SDL_GetTicks();
    return 1.0f * (this->ticks - oldTicks);
}

int Game_Timer::GetFrame() const
{
    return (int)this->frame;
}

void Game_Timer::DelayFPS()
{
    if(((unsigned)this->GetTicks() < 1000 / this->FRAME_RATE))
    {
        SDL_Delay((1000 / this->FRAME_RATE) - (int)this->GetTicks());
    }
}
