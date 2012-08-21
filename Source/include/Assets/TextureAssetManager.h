/**
 * @file
 *	Declarations for the GL_AssetManager interface.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Assets
 */
/// @{

#ifndef TEXTURE_ASSET_MANAGER_H
#define TEXTURE_ASSET_MANAGER_H

#include <map>
#include <string>

#include "CollapseDef.h"

#include "Assets/Asset.h"
#include "Assets/Entity.h"

using game::g_Log;

namespace asset
{
    class GL_AssetManager
    {
    public:
        ~GL_AssetManager();

        static GL_AssetManager& GetInstance();

        asset_id LoadEntityFromFile(const char* p_filename);
        asset_id LoadEntityFromSurface(SDL_Surface* const p_Surface);
        bool UnloadEntityByID(const asset_id id);

        GL_Entity*  GetEntityByID(const asset_id id);
        u_int       GetEntityCount() const;

        void Update();

    private:
        GL_AssetManager() : m_assetcount(0) {}
        GL_AssetManager(const GL_AssetManager&);

        std::map<asset_id, GL_Entity*> mp_assetPool;
        std::map<asset_id, GL_Entity*> mp_allAssets;

        u_int m_assetcount;
    };

    typedef std::map<asset_id, GL_Entity*>::iterator GL_asset_iterator;

    /// Globally accessible TextureAssetManager
    static GL_AssetManager& g_TextureAssets = GL_AssetManager::GetInstance();
}

#endif // TEXTURE_MANAGER_H

/// @}