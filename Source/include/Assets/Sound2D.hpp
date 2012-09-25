/**
 * @file
 *	Declarations for the OpenAL 2D sound class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Assets
 **/
/// @{

#ifndef ASSETS__SOUND_2D_HPP
#define ASSETS__SOUND_2D_HPP

#include <string>
#include <vector>

#include "AL/al.h"
#include "AL/alut.h"
#include "vorbis/vorbisfile.h"

#include "CollapseDef.hpp"
#include "Math/Math.hpp"
#include "Assets/Asset.hpp"

namespace asset
{
    /// All of the sources.
    static ALuint   s_sources[256];

    /// The index of the currently available source.
    static int      s_available     = 0;

    /// Total amount of AL assets loaded.
    static asset_id s_assetcount    = 0;

    /// Forward declaration of CAudioManager to declare it as a friend.
    class CAssetManager;

    class CSound2D : public CAsset
    {
    public:
        ~CSound2D();

        /// Initializes OpenAL and ALUT.
        static bool InitializeOpenAL();

        /**
         * Moves the sound source to a new location.
         *  Because OpenAL automatically handles sound volume based on
         *  location to the listener, it may sometimes be necessary to
         *  move the sound source closer or farther to the listener.
         *  This moves it at a certain rate, not to a certain location.
         *
         * @param math::CVector2& Rate at which to move sound source.
         **/
        void Move(const math::CVector2& Rate);
        
        bool Play();
        bool Pause();
        bool Stop();
        bool UnloadSource();

        void SetPosition(const math::CVector2& Position);
        void SetVelocity(const math::CVector2& Velocity);
        void SetDirection(const math::CVector2& Direction);

        int     GetAudioState() const;
        ALenum  GetLastError() const;
        ALuint  GetBuffer() const;
        ALuint  GetSource() const;
        ALuint  GetSourceIndex() const;

        /// The CAssetManager is the only thing capable of loading audio.
        friend class CAssetManager;

        static std::vector<asset::CSound2D*> mp_allSounds;

    private:
        /// Constructs an instance of the CSound2D class.
        CSound2D();

        /// Finds the next available OpenAL source.
        static void GetAvailableSource();

        bool LoadFromFile(const char* p_filename);
        bool LoadFromAudio(CSound2D* const p_Copy);
        bool LoadFromFile_WAV(const char* p_filename);

        ALuint  m_buffer;
        ALint   m_source;
        ALenum  m_lasterror;
    };
}

#endif // ASSETS__SOUND_2D_HPP

/// @}