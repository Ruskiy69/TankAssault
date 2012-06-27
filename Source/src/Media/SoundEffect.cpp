/************************************************************
 * The definitions for the MediaManager::SoundEffect class  *
 ***********************************************************/

#include "Media/SoundEffect.h"

using namespace MediaManager;
 
SoundEffect::SoundEffect() : Sound(NULL), volume(MIX_MAX_VOLUME), channel(avail_channel)
{
    avail_channel++;
}

SoundEffect::~SoundEffect()
{
    if(this->Sound != NULL)
        Mix_FreeChunk(this->Sound);
}

bool SoundEffect::Load(const char* filename)
{
    if(this->Sound != NULL)     // Some sound is loaded already
        Mix_FreeChunk(this->Sound);
        
    this->Sound = Mix_LoadWAV(filename);
    
    return this->Sound == NULL ? false : true;
}

bool SoundEffect::Load(const std::string& filename)
{
    return this->Load(filename.c_str());
}

void SoundEffect::SetVolume(const int volume)
{
    this->volume = volume;
    Mix_Volume(this->channel, this->volume);
}

void SoundEffect::DecreaseVolume()
{
    this->SetVolume(this->volume - 5);
}

void SoundEffect::IncreaseVolume()
{
    this->SetVolume(this->volume + 5);
}

void SoundEffect::Play()
{
    if(!Mix_Playing(this->channel))
        Mix_PlayChannel(this->channel, this->Sound, 0);
}

void SoundEffect::Play_Force()
{
    /* Plays sound regardless of whether or not
     * it is already currently playing.
     */
    Mix_PlayChannel(this->channel, this->Sound, 0);
}


void SoundEffect::Stop()
{
    if(Mix_Playing(channel))
        Mix_HaltChannel(this->channel);
}
