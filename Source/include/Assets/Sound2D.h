/**
 * @file
 *	Declarations for the OpenAL 2D sound class.
 *
 * @author  George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Assets
 */
/// @{

#ifndef SOUND_2D_H
#define SOUND_2D_H

#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "CollapseDef.h"
#include "Math/Math.h"
#include "Assets/Asset.h"

namespace asset
{
    /// All of the sources.
    static ALuint   s_sources[256];

    /// The index of the currently available source.
    static int      s_available     = 0;

    /// Total amount of AL assets loaded.
    static asset_id s_assetcount    = 0;

    /// Forward declaration of AL_AssetManager to declare it as a friend.
    class AL_AssetManager;

    class AL_Sound2D : public IAsset
    {
    public:
        ~AL_Sound2D();

        /// Initializes OpenAL and ALUT.
        static bool InitializeOpenAL();

        /**
         * Moves the sound source to a new location.
         *  Because OpenAL automatically handles sound volume based on
         *  location to the listener, it may sometimes be necessary to
         *  move the sound source closer or farther to the listener.
         *  This moves it at a certain rate, not to a certain location.
         *
         * @param math::ML_Vector2& Rate at which to move sound source.
         */
        void Move(const math::ML_Vector2& Rate);
        
        /**
         * Plays a loaded audio buffer.
         *  Due to the somewhat complicated source-management system in
         *  place, buffers are bound to an OpenAL source on demand.
         *  So whenever this method is called, an available source is
         *  found, and then the buffer is bound to it, and the audio
         *  is played. The AL_AssetManager is responsible for unloading the
         *  source after playback has completed in full.
         *  
         * @return TRUE if the sound played, FALSE if not.
         */
        bool Play();

        /**
         * Pauses current audio buffer.
         *  The buffer maintains linked to its source despite being paused,
         *  and will only be deleted if the source status is AL_STOPPED.
         *  
         * @return TRUE if paused, FALSE if nothing loaded.
         */
        bool Pause();

        /**
         * Stops the audio from playing.
         *  Audio looping is always enabled, with no method (currently)
         *  to disable it. This is due to the fact that I've never /not/
         *  needed to disable looping.
         *
         * @return TRUE if stopped, FALSE if nothing is loaded.
         * 
         * @see asset::AL_2DSound::Play()
         * @todo Add looping capabilities (or lack there-of).
         */
        bool Stop();

        /**
         * Sets the starting position of the sound source,
         * defaulting to <0, 0, 0>.
         * @param math::ML_Vector2& Position
         */
        void SetPosition(const math::ML_Vector2& Position);

        /**
         * Sets the velocity for the sound source, defaulting to <0, 0, 0>.
         *  If velocity is set to something other than the default value,
         *  a Doppler effect is produced as the sound source moves toward
         *  or away from the listener.
         *
         * @param math::ML_Vector2& Velocity
         */
        void SetVelocity(const math::ML_Vector2& Velocity);

        /**
         * Sets the direction the sound source is facing.
         * @param math::ML_Vector2& Direction
         */
        void SetDirection(const math::ML_Vector2& Direction);

        /**
         * Returns the current audio state.
         *  Possible states include AL_PAUSED, AL_PLAYING, and AL_STOPPED.
         *
         * @return The state.
         */
        int     GetAudioState() const;
        ALenum  GetLastError() const;
        ALuint  GetBuffer() const;
        ALuint  GetSource() const;
        ALuint  GetSourceIndex() const;

        /// The AL_AssetManager is the only thing capable of loading audio.
        friend class AL_AssetManager;

    private:
        /// Constructs an instance of the AL_Sound2D class.
        AL_Sound2D();

        /// Finds the next available OpenAL source.
        static void GetAvailableSource();

        /**
         * Loads an audio file into memory.
         *  The lib-vorbis SDK is used to determine whether or not the
         *  given filename refers to a valid .ogg file or not.
         *  If it is determined NOT to be an .ogg file, it is assumed
         *  to be a .wav file and loaded as such, returning errors on
         *  any failures. 
         *
         * @param char* Filename to load
         * @return TRUE if successfully loaded, FALSE otherwise.
         *  The actual error code can be checked by calling GetLastError().
         *
         * @todo Add the ability to stream large .ogg files.
         */
        bool LoadFromFile(const char* p_filename);

        /**
         * Loads audio from an existing AL_Sound2D class.
         *  This method is pretty untested, I wouldn't really recommend
         *  using it. It takes the buffer and source ID's from the
         *  provided, existing AL_Sound2D parameter.
         *
         * @param AL_Sound2D* The audio to copy.
         * @return Always TRUE.
         */
        bool LoadFromAudio(AL_Sound2D* const p_Copy);

        /**
         * Unloads the source being used for playback.
         *  Since OpenAL is limited to 256 audio sources, it is important
         *  to be conservative with the available resources.
         *  Though I doubt all 256 will ever be used simultaneously, a
         *  system is in place to minimize source-hogging. This method
         *  should only be called by an AL_AssetManager after checking
         *  that playback has been completed. 
         *
         * @return TRUE if the source was unloaded successfully,
         *  FALSE if not, or no source loaded.
         */
        bool UnloadSource();

        /**
         * Loads a .wav file.
         *  This is only called by LoadFromFile() after determining that
         *  the given data is not a valid Ogg-Vorbis audio file.
         *
         * @param char* Filename
         * @return TRUE on successful load, FALSE otherwise.
         */
        bool LoadFromFile_WAV(const char* p_filename);

        ALuint m_buffer, m_source;
        ALenum m_lasterror;
    };
}

#endif // SOUND_2D_H

/// @}