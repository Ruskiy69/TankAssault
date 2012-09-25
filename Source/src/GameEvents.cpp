#include "GameEvents.hpp"

using game::CGameEventQueue;

CGameEventQueue::~CGameEventQueue()
{
    for(size_t i = 0; i < mp_eventQueue.size(); ++i)
    {
        delete mp_eventQueue[i];
        mp_eventQueue[i] = NULL;
    }

    mp_eventQueue.clear();
}

CGameEventQueue& CGameEventQueue::GetInstance()
{
    static CGameEventQueue g_GameEventQueue;
    return g_GameEventQueue;
}

void CGameEventQueue::PushEvent(game::GameEvent* pEvt)
{
    mp_eventQueue.push_back(pEvt);
}

game::GameEvent* CGameEventQueue::PopEvent()
{
    if(mp_eventQueue.size() == 0)
        return NULL;
    game::GameEvent* pLatest = mp_eventQueue.front();
    mp_eventQueue.pop_front();
    return pLatest;
}