#include "Graphics/Light.hpp"

using gfx::CLight;

CLight::CLight()
{
    m_position[0] = m_position[1] = 0;
    m_color[0] = m_color[1] = m_color[2] = 1;
    
    // Some default values for attenuation.
    // These make a small point light that illuminates a decent amount.
    m_attenuation[0] = 0.05f;
    m_attenuation[1] = 0.01f;
    m_attenuation[2] = 0.0f;

    m_brightness = 0.1f;
}

void CLight::Adjust(const math::CVector2& Rate)
{
    m_position[0] += Rate.x;
    m_position[1] += Rate.y;
}

void CLight::Adjust(const float dx, const float dy)
{
    m_position[0] += dx;
    m_position[1] += dy;
}

void CLight::SetPosition(const math::CVector2& Pos)
{
    m_position[0] = Pos.x;
    m_position[1] = Pos.y;
}

void CLight::SetPosition(const float x, const float y)
{
    m_position[0] = x;
    m_position[1] = y;
}

void CLight::SetBrightness(const float val)
{
    m_brightness = val;
}

void CLight::SetAttenuation(const float c, const float l, const float q)
{
    m_attenuation[0] = c;
    m_attenuation[1] = l;
    m_attenuation[2] = q;
}

void CLight::SetColor(const float r, const float g, const float b)
{
    m_color[0] = 255.0f / r;
    m_color[1] = 255.0f / g;
    m_color[2] = 255.0f / b;
}

void CLight::SetColor(const gfx::Color& Color)
{
    m_color[0] = 255.0f / Color.r;
    m_color[1] = 255.0f / Color.g;
    m_color[2] = 255.0f / Color.b;
}

float* CLight::GetPosition()
{
    return m_position;
}

float* CLight::GetAttenuation()
{
    return m_attenuation;
}

float* CLight::GetColor()
{
    return m_color;
}

float CLight::GetBrightness()
{
    return m_brightness;
}