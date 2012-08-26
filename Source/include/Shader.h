/**
 * @file
 *	Declarations of the GL_Shader class.
 *
 * @author  George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Graphics
 */
/// @{

#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <string>

#include "GL/glew.h"
#include "GL/GLU.h"
#include "GL/GL.h"

#include "CollapseDef.h"

namespace gfx
{
    class GL_Shader
    {
    public:
        GL_Shader();
        ~GL_Shader();

        bool LoadFromFile(const char* p_filename, const int type);
        bool LoadFromSource(const char* p_src, const int type);
        bool Link();
        bool Unlink();
        int  GetLocation(const char* param);

        char* GetError() const;

    private:
        GLuint m_shader, m_program;
        GLint m_lasterror;
        GLchar* mp_lasterror;
    };
}

#endif // SHADER_H

/// @}