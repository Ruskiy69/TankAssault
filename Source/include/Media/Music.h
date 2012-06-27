#ifndef MUSIC_H
#define MUSIC_H

#include <string>

#include "SDL/SDL_mixer.h"

namespace MediaManager
{
    class Music
    {
    public:
        Music();
        Music(const char* filename);
        Music(const std::string& filename);
        ~Music();

        bool Load(const std::string& filename);
        bool Load(const char* filename);

        void SetVolume(const int volume);

        /* Decrease / Increase the volume by an
         * increment of 5.
         */
        void IncreaseVolume();
        void DecreaseVolume();
        
        void Play();
        void Pause();
        void Stop();
        void FadeOut(const int milliseconds);

        bool is_playing, is_paused;
        
    private:
        unsigned int volume;
        Mix_Music* Song;
    };
}

#endif // MUSIC_H
