/**
 * @file
 *  Declaration of the CAsset interface.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0.1
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

#ifndef ASSETS__ASSET_HPP
#define ASSETS__ASSET_HPP

#include <string>

#include "CollapseDef.hpp"

/**
 * Contains all asset related classes.
 *  Assets include fonts, textures, sound effects, etc. Asset managers
 *  are created to maintain and track these assets.
 **/
namespace asset
{
    typedef unsigned int asset_id;

    static unsigned long int ASSET_COUNT = 0;

    /// The interface for any game asset.
    class CAsset
    {
    public:
        CAsset();
        virtual ~CAsset();

        virtual bool LoadFromFile(const char* p_filename) = 0;
        virtual const char* GetFilename() const;
        virtual asset_id    GetID() const;

        bool IsLoaded() const;

        static inline u_int Hash(const char* pfilename, int size = 0)
        {
            unsigned int hash = 0;
            int c = 0;
            if(!size)
                while(c = *pfilename++) hash = ((hash << 5) + hash) ^ c;
            else
            {
                while(size > 0)
                {
                    c = *pfilename++;
                    hash = ((hash << 5) + hash) ^ c;
                    --size;
                }
            }                

            return hash;
        }

    protected:
        std::string m_filename;
        asset_id m_id;
        bool m_loaded;
    };
}

#endif // ASSETS__ASSET_HPP

/// @}