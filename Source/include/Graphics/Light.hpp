/**
 * @file
 *	Declarations for the CLight class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
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
 * @addtogroup Graphics
 */
/// @{

#ifndef GRAPHICS__LIGHT_HPP
#define GRAPHICS__LIGHT_HPP

#include "Math/Math.hpp"
#include "Graphics/Graphics.hpp"

namespace gfx
{
    class CLight
    {
    public:
        CLight();
        ~CLight(){}

        void Adjust(const float dx, const float dy);
        void Adjust(const math::CVector2& Rate);

        void SetPosition(const math::CVector2& Pos);
        void SetPosition(const float x, const float y);
        void SetBrightness(const float value);
        void SetAttenuation(const math::CVector3& Att);
        void SetAttenuation(const float c, const float l, const float q);
        void SetColor(const gfx::Color& Color);
        void SetColor(const float r, const float g, const float b);

        float* GetPosition();
        float* GetAttenuation();
        float* GetColor();
        float  GetBrightness();

    private:
        float m_position[2];
        float m_color[3];
        float m_attenuation[3];
        float m_brightness;
    };
}

#endif // GRAPHICS__LIGHT_HPP

/// @}