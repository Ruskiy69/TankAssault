/**
 * @file
 *	Declarations for the CShader class.
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
 * @addtogroup Graphics
 **/
/// @{

#ifndef GRAPHICS__SHADER_HPP
#define GRAPHICS__SHADER_HPP

#include <map>

#include "Graphics/Graphics.hpp"

namespace gfx
{
    template<typename T>
    struct ShaderParam
    {
        T*      m_pparams;      // The parameters to pass to the shader
        u_int   m_array_size;   // The size of each parameter
        u_int   m_array_count;  // The amount of parameters
    };

    class CShader
    {
    public:
        CShader();
        ~CShader();

        bool LoadFromFile(const char* pvs_filename, 
            const char* pfs_filename);
        bool LoadFromSource(const char* pvs_src, const char* pfs_src);

        template<typename T>
        bool SetMacro(const char* pmacro_name, const T value);

        void PassVariableiv(const char* pvar_name, int pvalues[],
            u_int array_size, u_int array_count);
        void PassVariablefv(const char* pvar_name, float pvalues[],
            u_int array_size, u_int array_count);

        bool Link();
        bool Unlink();

        bool  IsLoaded() const;
        int   GetLocation(const char* pvar_name) const;
        char* GetError() const;

    private:
        GLuint  LoadShaderFile(const char* pfilename, const int shader_type);
        GLuint  LoadShaderSource(const char* psrc, const int shader_type);

        typedef std::map<char*, ShaderParam<int>*>::iterator    ShaderI_It;
        typedef std::map<char*, ShaderParam<float>*>::iterator  ShaderF_It;

        std::map<char*, ShaderParam<int>*>      m_iParams;
        std::map<char*, ShaderParam<float>*>    m_fParams;

        std::string m_vsfn, m_fsfn;

        GLuint  m_vshader, m_fshader;
        GLuint  m_program;
        GLint   m_last_error;
        GLchar* mp_error_str;
    };

    template<typename T>
    bool CShader::SetMacro(const char* pmacro_name, const T value)
    {
        std::stringstream   all_data;
        std::fstream        vshader;
        std::fstream        fshader;
        std::string         line;

        if(!this->IsLoaded())
            return false;

        vshader.open(m_vsfn, std::ios::in);
        fshader.open(m_fsfn, std::ios::in);

        // Read in the vertex shader's existing data.
        // Repeat process for fragment shader.
        std::string finder = "#define ";
        finder += pmacro_name;

        while(std::getline(vshader, line))
        {
            if(line.find(finder) == std::string::npos)
                all_data << line << '\n';
        }

        // Write the macro at the beginning
        vshader.close();
        vshader.open(m_vsfn, std::ios::out | std::ios::trunc);
        vshader << "#define " << pmacro_name << ' ' << value << "\n";
        vshader << all_data.str();
        vshader.close();

        // Clear stringstream
        all_data.str(std::string());

        // Repeat process for fragment shader.
        while(std::getline(fshader, line))
        {
            if(line.find(finder) == std::string::npos)
                all_data << line << '\n';
        }

        fshader.close();
        fshader.open(m_fsfn, std::ios::out | std::ios::trunc);
        fshader << "#define " << pmacro_name << ' ' << value << "\n";
        fshader << all_data.str();
        fshader.close();

        // Reload the OpenGL shader programs
        return this->LoadFromFile(m_vsfn.c_str(), m_fsfn.c_str());
    }
}

#endif // GRAPHICS__SHADER_HPP

/// @}
