#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <vector>

#include "Media/SoundEffect.h"
#include "Media/Music.h"

namespace MediaManager
{
    class MusicPlayer
    {
    public:
        MusicPlayer();
        ~MusicPlayer();

        bool AddSongToQueue(const char* filename);
        bool AddSongToQueue(const std::string& filename);

        void PurgeQueue();

        void PlayMusic();
        void PauseMusic();
        void StopMusic();

        void PlaySound(const std::string& filename);
        void PlaySound(const char* filename);

        void SetVolume(const int volume);

        void Update();

    private:
        bool NextSong();    // Returns true if there is another song, false otherwise

        Music* Current_Song;
        std::vector<Music*> Songs;
        unsigned int index;
    };
}
    
#endif // MUSIC_PLAYER_H
