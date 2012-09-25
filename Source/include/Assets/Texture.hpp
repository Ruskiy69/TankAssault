/**
 * @file
 *	Declarations for the CTexture class.
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

#ifndef ASSETS__TEXTURE_HPP
#define ASSETS__TEXTURE_HPP

#include "Graphics/Graphics.hpp"
#include "Assets/Asset.hpp"

namespace asset
{
    class CTexture : public asset::CAsset
    {
    public:
    	CTexture() : m_texture(0) {}
    	virtual ~CTexture() { glDeleteTextures(1, &m_texture); }
    
        bool LoadFromFile(const char* pfilename);
        bool LoadFromSurface(SDL_Surface* pSurface);
        
        void Resize(const u_int w, const u_int h);

        GLuint GetTexture() const;
        GLint  GetW() const;
        GLint  GetH() const;

    private:
        math::CRect m_Size;
        GLuint      m_texture;
    };
}


#endif // ASSETS__TEXTURE_HPP

/// @}