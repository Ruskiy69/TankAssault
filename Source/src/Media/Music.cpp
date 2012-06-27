/************************************************************
 * The definitions for the MediaManager::Music class        *
 ************************************************************/
 
#include "Media/Music.h"

using namespace MediaManager;

Music::Music() : Song(NULL), volume(MIX_MAX_VOLUME), is_playing(false), is_paused(false) {}

Music::~Music() {}

bool Music::Load(const char* filename)
{
    if(this->Song != NULL)      // Some song is loaded already
        Mix_FreeMusic(this->Song);
        
    this->Song = Mix_LoadMUS(filename);
    
    return this->Song == NULL ? false : true;
}

bool Music::Load(const std::string& filename)
{
    return this->Load(filename.c_str());
}

void Music::SetVolume(const int volume)
{
    this->volume = Mix_VolumeMusic(volume);
}

void Music::DecreaseVolume()
{
    this->SetVolume(this->volume - 5);
}

void Music::IncreaseVolume()
{
    this->SetVolume(this->volume + 5);
}

void Music::Play()
{
    if(this->Song == NULL)
        return;

    if(this->is_paused)     // We're just paused, resume music
    {
        Mix_ResumeMusic();
        this->is_paused = false;
        this->is_playing = true;
    }
    else    // Stopped or hasn't started, so start over
    {
        int error = Mix_PlayMusic(this->Song, 1);
        
        if(error == -1) // Failed to play lol
            ;   // Do nothing, yolo
        else
        {
            this->is_playing = true;
            this->is_paused  = false;
        }
    }
}

void Music::Pause()
{
    if(this->is_playing && !this->is_paused)
    {
        Mix_PauseMusic();
        this->is_paused = true;
        this->is_playing = false;
    }
}

void Music::Stop()
{
    Mix_HaltMusic();
    this->is_playing = false;
    this->is_paused = false;
}

void Music::FadeOut(const int ms)
{
    Mix_FadeOutMusic(ms);
}