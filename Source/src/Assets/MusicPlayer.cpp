/**
 * @file
 *  Definitions for the CMusicPlayer class.
 *
 * @author George Kudrayvtsev
 * @version 1.0.1
 **/

#include "Assets/MusicPlayer.hpp"

using asset::CMusicPlayer;
using asset::CAssetManager;
using game::g_Log;
 
/**
 * Cleans up all loaded music.
 * @todo Clean up assets from manager;
 **/
CMusicPlayer::~CMusicPlayer()
{
}

/**
 * Load a music file and add it to the play list.
 * @param char* filename
 * @return TRUE if loaded, FALSE if not.
 **/
bool CMusicPlayer::AddSongToQueue(const char* pfilename)
{
    m_allSongs.push_back(
        CAssetManager::Create<asset::CSound2D>(pfilename)->GetID());
    return true;
}

/**
 * Deletes all music from the current play list.
 **/
void CMusicPlayer::PurgeQueue()
{
    m_allSongs.clear();
    mp_CurrentSong = NULL;
}

/**
 * Starts playing music.
 **/
void CMusicPlayer::Play()
{
    if(mp_CurrentSong == NULL)
    {
        if(!this->NextSong())   // No songs available
            return;
    }
    
    if(mp_CurrentSong->GetAudioState() != AL_PLAYING)
        mp_CurrentSong->Play();
}

/// Pauses music.
void CMusicPlayer::Pause()
{
    if(mp_CurrentSong == NULL)      // Can't pause no song
        return;

    if(mp_CurrentSong->GetAudioState() == AL_PAUSED)    // Resume if paused
        mp_CurrentSong->Play();
    else
        mp_CurrentSong->Pause();    // Playing? Cool. Pause
}

/// Stops music.
void CMusicPlayer::Stop()
{
    if(mp_CurrentSong == NULL)
        return;

    mp_CurrentSong->Stop();
}

/**
 * Checks if the song is done playing, if so, plays next song.
 *  This should be called within a loop, it will test if it is time to
 *  play the next song every 2 seconds or so (assuming a 60fps game).
 **/
void CMusicPlayer::Update()
{
    static int frame = 0;
    frame++;

    if(frame % 120)     // Approx. every 2 seconds
    {
        if(mp_CurrentSong == NULL)
        {
            this->NextSong();
            return;
        }

        int state = mp_CurrentSong->GetAudioState();
        if(state != AL_PLAYING && state != AL_PAUSED)
        {
            if(this->NextSong())
            {
                g_Log.Flush();
                g_Log << "[INFO] Playing next song, ";
                g_Log << mp_CurrentSong->GetFilename() << ".\n";

                this->Play();
            }
            else
            {
                g_Log.Flush();
                g_Log << "[INFO] Song queue end reached, looping.\n";

                m_index = 0;
            }
            frame = 0;
        }
    }
}

/**
 * Loads the next song in the play list.
 * @return bool TRUE if a song was loaded, FALSE if there are no more songs.
 **/
bool CMusicPlayer::NextSong()
{
    if(m_index >= m_allSongs.size())     // Done playing queue of songs
        return false;
    
    if(mp_CurrentSong)
        mp_CurrentSong->Stop();

    mp_CurrentSong = (asset::CSound2D*)CAssetManager::Find(
        m_allSongs[m_index]);
    ++m_index;

    return true;
}
