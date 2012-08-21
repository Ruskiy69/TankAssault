/**
 * @file
 *	Declarations for the AL_MusicPlayer class.
 *
 * @author  George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Assets
 */
/// @{

#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "CollapseDef.h"
#include "Assets/AudioAssetManager.h"

namespace asset
{
    class AL_MusicPlayer
    {
    public:
        AL_MusicPlayer() : mp_CurrentSong(NULL), m_index(0) {}
        ~AL_MusicPlayer();
    
        bool AddSongToQueue(const char* pfilename);
        void PurgeQueue();

        void Play();
        void Pause();
        void Stop();

        void Update();

    private:
        bool NextSong();
        
        std::vector<asset::asset_id> m_allSongs;
        AL_Sound2D* mp_CurrentSong;
        u_int m_index;
    };
}

#endif // MUSIC_PLAYER_H

/// @}