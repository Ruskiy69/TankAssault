/**
 * @file
 *	Declaration for the CEntity class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.8.0
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
 * @addtogroup Objects
 **/
/// @{

#ifndef WORLD__OBJECTS__ENTITY_HPP
#define WORLD__OBJECTS__ENTITY_HPP

#include <cassert>

#include "SDL/SDL.h"

#include "Math/Math.hpp"
#include "Graphics/Graphics.hpp"
#include "Assets/Texture.hpp"

namespace obj
{
    class CEntity
    {
    public:
        // Ctor, dtor
        CEntity();
        virtual ~CEntity();

        // Loading methods
        virtual bool LoadFromFile(const char* pfilename);
        virtual bool LoadFromTexture(const asset::CTexture* pTexture);
        virtual bool LoadFromSurface(SDL_Surface* pSurface);
        virtual bool LoadFromEntity(CEntity* pCopy);

        void Move(const math::CVector2& Position);
        void Move(const float x, const float y);
        void Move_Rate(const math::CVector2& Rate);
        void Move_Rate(const float dx, const float dy);

        void Rotate(const float deg);
        void Rotate_Rate(const float ddeg);

        void ResizeTexture(const u_int w, const u_int h);
        void ResizeTexture(const math::CRect& NewSize);

        void Update();

        // Modifiers
        void SetBlending(bool flag);
        void SetRenderDimensions(const math::CRectf& Dimensions);
        void SetMovementRate(const math::CVector2& NewRate);
        void SetFilename(const char* pfilename);

        // Accessors
        const asset::CTexture& GetTexture() const;
        const math::CVector2& GetPosition() const;
        const math::CVector2& GetMovementRate() const;
        const math::CRectf& GetRenderDimensions() const;
        const std::string& GetFilename() const;
        float* GetVertices();
        float GetRotationAngle() const;
        float GetX() const;
        float GetY() const;
        u_int GetGLTexture() const;
        int GetW() const;
        int GetH() const;

    protected:
        // Member variables
        asset::CTexture m_Texture;
        math::CVector2  m_Position;
        math::CVector2  m_MovementRate;
        math::CRectf    m_RenderDimensions;
        float           m_vertices[4];
        float           m_rotation_angle;
        bool            m_useblending;
    };
}

namespace gfx
{
    bool FadeOut(const std::vector<obj::CEntity*> pentitiesToFade,
        const float fade_rate = 0.01f);
    bool FadeIn(const std::vector<obj::CEntity*> pentitiesToFade,
        const float fade_rate = 0.01f);

    bool FadeOut(obj::CEntity* p_ToFade, const float fade_rate = 0.01f);
    bool FadeIn(obj::CEntity* p_ToFade, const float fade_rate = 0.01f);
}

#endif // WORLD__OBJECTS__ENTITY_HPP

/// @}