#include "Assets/AudioAssetManager.h"

using asset::AL_AssetManager;
using game::g_Log;

AL_AssetManager::~AL_AssetManager()
{
    asset::AL_asset_iterator i;
    for(i = mp_audioAssets.begin();
        i != mp_audioAssets.end();
        /* no third */)
    {
        delete i->second;
        i = mp_audioAssets.erase(i);
    }

    mp_audioAssets.clear();
}

AL_AssetManager& AL_AssetManager::GetInstance()
{
    static AL_AssetManager g_ALAssets;
    return g_ALAssets;
}

asset::asset_id AL_AssetManager::LoadAudioFromFile(const char* p_filename)
{
    AL_asset_iterator i;
    for(i = mp_audioAssets.begin();
        i != mp_audioAssets.end();
        ++i)
    {
        if(i->second->GetFilename() == p_filename)
            return i->first;
    }

    g_Log.Flush();
    g_Log << "[INFO] Loading audio asset: " << p_filename << ".\n";

    AL_Sound2D* pAsset = new AL_Sound2D;
    if(!pAsset->LoadFromFile(p_filename))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load audio asset: " << p_filename;
        g_Log << "\n[ERROR] OpenAL error code: " << pAsset->GetLastError();
        g_Log << "\n[ERROR] ALUT error: ";
        g_Log << alutGetErrorString(alutGetError()) << ".\n";
        g_Log.ShowLastLog();
    }

    m_assetcount++;
    mp_audioAssets[pAsset->GetID()] = pAsset;
    return pAsset->GetID();
}

asset::AL_Sound2D* AL_AssetManager::GetAudioByFilename(const char* p_filename)
{
    for(AL_asset_iterator i = mp_audioAssets.begin();
        i != mp_audioAssets.end();
        ++i)
    {
        if(i->second->GetFilename() == p_filename)
            return i->second;
    }

    return NULL;
}

asset::AL_Sound2D* AL_AssetManager::GetAudioByID(const asset::asset_id id)
{
    asset::AL_asset_iterator i = mp_audioAssets.find(id);
    if(i == mp_audioAssets.end())
        return NULL;
    else
        return i->second;
}

void AL_AssetManager::Update()
{
    asset::AL_asset_iterator i = mp_audioAssets.begin();
    for(i = mp_audioAssets.begin();
        i != mp_audioAssets.end();
        ++i)
    {
        int state = i->second->GetAudioState();
        if(state == AL_STOPPED)
        {
            g_Log.Flush();
            g_Log << "[INFO] Unloading audio file: ";
            g_Log << i->second->GetFilename() << ".\n";
            g_Log.ShowLastLog();
            i->second->UnloadSource();
        }
    }
}

u_int AL_AssetManager::GetAudioCount() const
{
    return m_assetcount;
}
