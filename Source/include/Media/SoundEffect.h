#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include <string>

#include "SDL/SDL_mixer.h"

namespace MediaManager
{
    static int avail_channel = 0;

    class SoundEffect
    {
    public:
        SoundEffect();
        ~SoundEffect();
        
        bool Load(const char* filename);
        bool Load(const std::string& filename);

        void SetVolume(const int volume);

        /* Decrease / Increase the volume by an
         * increment of 5.
         */
        void IncreaseVolume();
        void DecreaseVolume();

        void Play();
        void Play_Force();
        void Stop();
        
    private:
        Mix_Chunk* Sound;
        unsigned int volume;
        int channel;
    };
}

#endif // SOUND_EFFECT_H
