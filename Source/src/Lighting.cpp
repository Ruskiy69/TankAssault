#include "Lighting.h"

using game::g_Log;
using gfx::GL_Lighting;

GL_Lighting::GL_Lighting() {}

bool GL_Lighting::Init()
{
    if(!m_FShader.LoadFromFile("Fragment.shader", GL_FRAGMENT_SHADER))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load shader 'Fragment.shader'.\n";
        g_Log << "[ERROR] Shader error: " << m_FShader.GetError();
        g_Log << ".\n";
        g_Log.ShowLastLog();
        return false;
    }
    if(!m_VShader.LoadFromFile("Vertex.shader", GL_VERTEX_SHADER))
    {
        g_Log.Flush();
        g_Log << "[ERROR] Failed to load shader 'Vertex.shader'.\n";
        g_Log << "[ERROR] Shader error: " << m_VShader.GetError();
        g_Log << ".\n";
        g_Log.ShowLastLog();
        return false;
    }

    return true;
}

gfx::GL_Shader& GL_Lighting::GetVShader()
{
    return m_VShader;
}

gfx::GL_Shader& GL_Lighting::GetFShader()
{
    return m_FShader;
}
