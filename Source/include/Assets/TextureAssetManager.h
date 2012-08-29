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
        template<typename T>
        asset_id LoadEntityFromFile(const char* p_filename);
        asset_id LoadEntityFromSurface(SDL_Surface* const p_Surface);
        asset_id LoadEntityFromTexture(const u_int texture,
            const math::ML_Rect& Dimensions);
        bool UnloadEntityByID(const asset_id id);

        void        SetDefaultShader(gfx::GL_Shader* p_VShader, 
            gfx::GL_Shader* p_FShader);
        GL_Entity*  GetEntityByID(const asset_id id);
        u_int       GetEntityCount() const;

        void Update();

    private:
        GL_AssetManager() : mp_VShader(NULL), mp_FShader(NULL),
            m_assetcount(0) {}
        GL_AssetManager(const GL_AssetManager&);

        gfx::GL_Shader* mp_VShader;
        gfx::GL_Shader* mp_FShader;

        std::map<asset_id, GL_Entity*> mp_assetPool;
        std::map<asset_id, GL_Entity*> mp_allAssets;

        u_int m_assetcount;
    };

    typedef std::map<asset_id, GL_Entity*>::iterator GL_asset_iterator;

    template<typename T>
    asset_id GL_AssetManager::LoadEntityFromFile(const char* p_filename)
    {
        GL_asset_iterator finder;

        // Figure out if the requested asset exists in the asset pool.
        for(finder = mp_assetPool.begin();
            finder != mp_assetPool.end();
            ++finder)
        {
            if(finder->second->GetFilename() == p_filename)
            {
                // If it exists, make a copy, add to all assets,
                // and return it.
                T* p_Tmp = new T(mp_VShader, mp_FShader);
                p_Tmp->LoadFromEntity(finder->second);
                mp_allAssets[p_Tmp->GetID()] = (GL_Entity*)p_Tmp;
                m_assetcount++;
                return p_Tmp->GetID();
            }
        }

        // If if doesn't exist, add it to the pool, 
        // add it to all assets, and return it.

        // Do some logging operations.
        g_Log.Flush();
        g_Log << "[INFO] Loading texture asset: " << p_filename << ".\n";

        T* p_ToPool = new T(mp_VShader, mp_FShader);
        if(!p_ToPool->LoadFromFile(p_filename))
        {
            g_Log.Flush();
            g_Log << "[ERROR] Failed to load texture asset: ";
            g_Log << p_filename << ".\n";
            g_Log << "[ERROR] OpenGL error: " << gluGetString(glGetError());
            g_Log << "\n[ERROR] SDL error: " << SDL_GetError() << ".\n";
            gk::handle_error(g_Log.GetLastLog().c_str());
        }

        mp_assetPool[p_ToPool->GetID()] = (GL_Entity*)p_ToPool;

        T* p_ToAll = new T(mp_VShader, mp_FShader);
        if(!p_ToAll->LoadFromEntity(p_ToPool))
        {
            g_Log.Flush();
            g_Log << "[ERROR] Failed to create texture from pixel data!\n";
            g_Log << "[ERROR] SDL error: " << SDL_GetError() << ".\n";
            gk::handle_error(g_Log.GetLastLog().c_str());
        }
        mp_allAssets[p_ToAll->GetID()] = (GL_Entity*)p_ToAll;
        m_assetcount++;
        return p_ToAll->GetID();
    }

    /// Globally accessible TextureAssetManager
    static GL_AssetManager& g_TextureAssets = GL_AssetManager::GetInstance();
}

#endif // TEXTURE_MANAGER_H

/// @}