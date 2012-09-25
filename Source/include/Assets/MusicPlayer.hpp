/**
 * @file
 *	Declarations for the CMusicPlayer class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1
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

#ifndef ASSETS__MUSIC_PLAYER_HPP
#define ASSETS__MUSIC_PLAYER_HPP

#include "CollapseDef.hpp"
#include "Assets/Sound2D.hpp"
#include "Assets/AssetManager.hpp"

namespace asset
{
    class CMusicPlayer
    {
    public:
        CMusicPlayer() : mp_CurrentSong(NULL), m_index(0) {}
        ~CMusicPlayer();
    
        bool AddSongToQueue(const char* pfilename);
        void PurgeQueue();

        void Play();
        void Pause();
        void Stop();

        void Update();

    private:
        bool NextSong();
        
        std::vector<asset::asset_id> m_allSongs;
        CSound2D* mp_CurrentSong;
        u_int m_index;
    };
}

#endif // ASSETS__MUSIC_PLAYER_HPP

/// @}