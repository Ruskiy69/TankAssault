/**
 * @file
 *	Declarations for the GL_Lighting class.
 *
 * @author  George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Graphics
 */
/// @{

#ifndef LIGHTING_H
#define LIGHTING_H

#include "Errors.h"
#include "CollapseDef.h"
#include "Shader.h"

namespace gfx
{
    class GL_Lighting
    {
    public:
        GL_Lighting();
        bool Init();

        GL_Shader& GetVShader();
        GL_Shader& GetFShader();

    private:
        GL_Shader m_VShader;
        GL_Shader m_FShader;
    };
}

#endif // LIGHTING_H

/// @}