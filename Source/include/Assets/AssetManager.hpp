/**
 * @file
 *	Declarations for the CAssetManager class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     0.1
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

#ifndef ASSETS__ASSET_MANAGER_HPP
#define ASSETS__ASSET_MANAGER_HPP

#include "CollapseDef.hpp"
#include "Assets/Asset.hpp"

namespace asset
{
    using game::g_Log;

    class CAssetManager
    {
    public:
        virtual ~CAssetManager();

        static CAsset* Find(const char* pfilename)
        {
            // Comparing hashes is much quicker than comparing strings.
            asset::asset_id searcher = CAsset::Hash(pfilename);
            for(size_t i = 0; i < CAssetManager::mp_allAssets.size(); ++i)
                if(searcher == CAsset::Hash(
                    CAssetManager::mp_allAssets[i]->GetFilename()))
                    return CAssetManager::mp_allAssets[i];

            // Specified asset wasn't found.
            return NULL;
        }
        static CAsset* Find(const asset::asset_id uid)
        {
            for(size_t i = 0; i < mp_allAssets.size(); ++i)
                if(uid == CAssetManager::mp_allAssets[i]->GetID())
                    return CAssetManager::mp_allAssets[i];

            return NULL;
        }

        template<typename T>
        static T* Create(const char* pfilename)
        {
            T* pTest = (T*)CAssetManager::Find(pfilename);
            if(pTest != NULL)
            {
                g_Log.Flush();
                g_Log << "[INFO] Retrieving asset: " << pfilename << ".\n";
                g_Log.ShowLastLog();
                return pTest;
            }
            else
            {
                g_Log.Flush();
                g_Log << "[INFO] Creating asset: " << pfilename << ".\n";
                g_Log.ShowLastLog();

                T* pLatest = new T;
                if(pLatest->LoadFromFile(pfilename))
                {
                    CAssetManager::mp_allAssets.push_back(pLatest);
                    return (T*)CAssetManager::mp_allAssets.back();
                }
                else
                {
                    g_Log.Flush();
                    g_Log << "[ERROR] Failed to load asset: " << pfilename << ".\n";
                    gk::handle_error(g_Log.GetLastLog().c_str());
                    return NULL;
                }
            }
        }

        static inline u_int GetAssetCount()
        {
            return CAssetManager::mp_allAssets.size();
        }

    private:
        CAssetManager();
        static std::vector<CAsset*> mp_allAssets;
    };
}

#endif // ASSETS__ASSET_MANAGER_HPP

/// @}