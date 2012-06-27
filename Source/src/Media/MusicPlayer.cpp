/************************************************************
 * The definitions for the MediaManager::MusicPlayer class  *
 ************************************************************/
#include "Media/MusicPlayer.h"

using namespace MediaManager; 
 
MusicPlayer::MusicPlayer() : Current_Song(NULL), index(0) {}

MusicPlayer::~MusicPlayer()
{
    for(size_t i = 0; i < this->Songs.size(); i++)
        if(this->Songs[i] != NULL)
            delete this->Songs[i];

    this->Songs.clear();
}

bool MusicPlayer::AddSongToQueue(const std::string& filename)
{
    return this->AddSongToQueue(filename.c_str());
}

bool MusicPlayer::AddSongToQueue(const char* filename)
{
    Music* Tmp = new Music();
    if(Tmp->Load(filename))
    {
        this->Songs.push_back(Tmp);
        return true;
    }
    else
        return false;
}

void MusicPlayer::PurgeQueue()
{
    for(size_t i = 0; i < this->Songs.size(); i++)
        delete this->Songs[i];

    this->Songs.clear();
    this->Current_Song = NULL;
}

void MusicPlayer::PlayMusic()
{
    if(this->Current_Song == NULL)
    {
        if(!this->NextSong())   // No songs available
            return;
    }
    
    if(!this->Current_Song->is_playing)
        this->Current_Song->Play();
}

void MusicPlayer::PauseMusic()
{
    if(this->Current_Song == NULL)  // Can't pause no song
        return;

    if(this->Current_Song->is_paused)   // Resume if paused
        this->Current_Song->Play();
    else
        this->Current_Song->Pause();    // Playing? Cool. Pause
}

void MusicPlayer::StopMusic()
{
    if(this->Current_Song == NULL)
        return;

    this->Current_Song->Stop();
}

void MusicPlayer::PlaySound(const std::string& filename)
{
    this->PlaySound(filename.c_str());
}

void MusicPlayer::PlaySound(const char* filename)
{
    SoundEffect* Sound = new SoundEffect;
    Sound->Load(filename);
    Sound->Play();

    delete Sound;
}

void MusicPlayer::SetVolume(const int v)
{
    for(size_t i = 0; i < this->Songs.size(); i++)
        this->Songs[i]->SetVolume(v);
}

void MusicPlayer::Update()
{
    static int frame = 0;
    frame++;

    if(frame % 120)     // Approx. every 2 seconds
    {
        if((!Mix_PlayingMusic()) && (!Mix_PausedMusic()))   // Not playing and not paused?
        {                                                   // I assume the song ended
            if(this->NextSong())
                this->PlayMusic();
            else
                index = 0;
            frame = 0;
        }
    }
}

bool MusicPlayer::NextSong()
{
    if(index >= this->Songs.size())     // Done playing queue of songs
        return false;
    
    if(this->Current_Song)
        this->Current_Song->Stop();

    this->Current_Song = this->Songs[this->index];
    this->index++;

    return true;
}