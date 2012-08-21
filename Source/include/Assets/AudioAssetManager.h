/**
 * @file
 *	Declarations for the AL_AssetManager interface.
 *
 * @author  George Kudrayvtsev
 * @version 1.1
 *
 * @addtogroup Assets
 */
/// @{

#ifndef AUDIO_ASSET_MANAGER_H
#define AUDIO_ASSET_MANAGER_H

#include <map>
#include <string>

#include "CollapseDef.h"

#include "Assets/Asset.h"
#include "Assets/Sound2D.h"

using game::g_Log;

namespace asset
{
    class AL_AssetManager
    {
    public:
        ~AL_AssetManager();

        static AL_AssetManager& GetInstance();

        asset_id LoadAudioFromFile(const char* p_filename);

        AL_Sound2D* GetAudioByFilename(const char* p_filename);
        AL_Sound2D* GetAudioByID(const asset_id id);
        u_int       GetAudioCount() const;

        void Update();

    private:
        AL_AssetManager() : m_assetcount(0) {};
        AL_AssetManager(const AL_AssetManager&);

        std::map<asset_id, AL_Sound2D*> mp_audioAssets;
        u_int m_assetcount;
    };

    typedef std::map<asset_id, AL_Sound2D*>::iterator AL_asset_iterator;

    /// Globally accessible AudioAssetManager
    static AL_AssetManager& g_AudioAssets = AL_AssetManager::GetInstance();
}

#endif // AUDIO_ASSET_MANAGER_H

/// @}