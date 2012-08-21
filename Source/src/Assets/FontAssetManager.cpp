#include "Assets/FontAssetManager.h"

using asset::FL_AssetManager;
using game::g_Log;

FL_AssetManager& FL_AssetManager::GetInstance()
{
    static FL_AssetManager g_ALAssets;
    return g_ALAssets;
}

asset::asset_id FL_AssetManager::LoadFontFromFile(
    const char* p_filename, const u_int size)
{
    FL_asset_iterator i;
    for(i = mp_fontAssets.begin();
        i != mp_fontAssets.end();
        ++i)
    {
        if(i->second->GetFilename() == p_filename)
            return i->first;
    }

    g_Log.Flush();
    g_Log << "[INFO] Loading font asset: " << p_filename << ".\n";

    FL_Font* pAsset = new FL_Font;
    if(!pAsset->LoadFromFile(p_filename, size))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load font asset: ";
        g_Log << p_filename << ".\n";
        g_Log << "[ERROR] TTF error: " << TTF_GetError() << ".\n";
        gk::handle_error(g_Log.GetLastLog().c_str());
    }

    m_assetcount++;
    mp_fontAssets[pAsset->GetID()] = pAsset;
    return pAsset->GetID();
}

asset::FL_Font* FL_AssetManager::GetFontByFilename(const char* p_filename)
{
    for(FL_asset_iterator i = mp_fontAssets.begin();
        i != mp_fontAssets.end();
        ++i)
    {
        if(i->second->GetFilename() == p_filename)
            return i->second;
    }

    return NULL;
}

asset::FL_Font* FL_AssetManager::GetFontByID(const asset::asset_id id)
{
    FL_asset_iterator i = mp_fontAssets.find(id);
    if(i == mp_fontAssets.end())
        return NULL;
    else
        return i->second;
}

u_int FL_AssetManager::GetFontCount() const
{
    return m_assetcount;
}

FL_AssetManager::~FL_AssetManager()
{
    asset::FL_asset_iterator i;
    for(i = mp_fontAssets.begin();
        i != mp_fontAssets.end();
        /* no third */)
    {
        delete i->second;
        i->second = NULL;
        i = mp_fontAssets.erase(i);
    }

    mp_fontAssets.clear();
}
