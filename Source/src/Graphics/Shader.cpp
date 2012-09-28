#include "Graphics/Shader.hpp"

using gfx::CShader;
using game::g_Log;

CShader::CShader() : m_vshader(0), m_fshader(0), m_program(0),
    m_last_error(GL_NO_ERROR), mp_error_str(NULL) {}

CShader::~CShader()
{
    if(m_vshader > 0) glDeleteShader(m_vshader);
    if(m_fshader > 0) glDeleteShader(m_fshader);
    if(m_program > 0) glDeleteProgram(m_program);

    ShaderI_It i;
    for(i = m_iParams.begin();
        i != m_iParams.end();)
    {
        delete[] i->second->m_pparams;
        delete i->second;

        i = m_iParams.erase(i);
    }

    ShaderF_It j;
    for(j = m_fParams.begin();
        j != m_fParams.end();)
    {
        delete[] j->second->m_pparams;
        delete j->second;
        
        j = m_fParams.erase(j);
    }
}

bool CShader::LoadFromFile(const char* pvs_filename, 
    const char* pfs_filename)
{
    int length = 0;

    m_vshader = this->LoadShaderFile(pvs_filename, GL_VERTEX_SHADER);
    m_fshader = this->LoadShaderFile(pfs_filename, GL_FRAGMENT_SHADER);
    
    if(m_vshader <= 0 || m_fshader <= 0) return false;

    // Create program object and attach shader.
    m_program = glCreateProgram();
    glAttachShader(m_program, m_vshader);
    glAttachShader(m_program, m_fshader);

    // Link program
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &m_last_error);

    if(m_last_error == GL_FALSE)
    {
        // Get log length.
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_error_str != NULL)
            delete[] mp_error_str;

        mp_error_str = new char[length];
        glGetProgramInfoLog(m_program, length, &length, mp_error_str);
        glDeleteProgram(m_program);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to link '" << pvs_filename << "'.\n";
        g_Log << "[Error] OpenGL error: " << mp_error_str;
        return false;
    }

    m_vsfn = pvs_filename;
    m_fsfn = pfs_filename;

    return true;
}

bool CShader::LoadFromSource(const char* pvs_src, const char* pfs_src)
{
    return (this->LoadShaderSource(pvs_src, GL_VERTEX_SHADER) &&
        this->LoadShaderSource(pfs_src, GL_FRAGMENT_SHADER));
}

void CShader::PassVariableiv(const char* pvar_name, int pvalues[],
    u_int array_size, u_int array_count)
{
    ShaderI_It ifinder = m_iParams.find((char* const)pvar_name);
    
    if(ifinder != m_iParams.end())
    {
        delete[] ifinder->second->m_pparams;
        ifinder->second->m_pparams      = new int[array_size * array_count];
        ifinder->second->m_array_size   = array_size;
        ifinder->second->m_array_count  = array_count;

        for(u_int i = 0; i < array_size * array_count; ++i)
            ifinder->second->m_pparams[i] = pvalues[i];
    }
    
    gfx::ShaderParam<int>* pParam = new gfx::ShaderParam<int>;

    pParam->m_pparams       = new int[array_count * array_size];
    pParam->m_array_size    = array_size;
    pParam->m_array_count   = array_count;

    for(u_int i = 0; i < array_size * array_count; ++i)
        pParam->m_pparams[i] = pvalues[i];

    m_iParams[(char*)pvar_name] = pParam;
}

void CShader::PassVariablefv(const char* pvar_name, float pvalues[],
    u_int array_size, u_int array_count)
{
    ShaderF_It ffinder = m_fParams.find((char* const)pvar_name);

    if(ffinder != m_fParams.end())
    {
        delete[] ffinder->second->m_pparams;
        ffinder->second->m_pparams      = new float[array_size * array_count];
        ffinder->second->m_array_size   = array_size;
        ffinder->second->m_array_count  = array_count;

        for(u_int i = 0; i < array_size * array_count; ++i)
            ffinder->second->m_pparams[i] = pvalues[i];
    }
    
    gfx::ShaderParam<float>* pParam = new gfx::ShaderParam<float>;

    pParam->m_pparams       = new float[array_count * array_size];
    pParam->m_array_size    = array_size;
    pParam->m_array_count   = array_count;

    for(u_int i = 0; i < array_size * array_count; ++i)
        pParam->m_pparams[i] = pvalues[i];

    m_fParams[(char*)pvar_name] = pParam;
}

bool CShader::Link()
{
    if(!this->IsLoaded()) return false;

    // Iterators for passing parameters.
    ShaderI_It i;
    ShaderF_It f;

    glUseProgram(m_program);

    // Pass integer parameters to the shader.
    for(i = m_iParams.begin(); i != m_iParams.end(); ++i)
    {
        int loc = this->GetLocation(i->first);

        switch(i->second->m_array_size)
        {
        case 1:
            glUniform1iv(loc,
                i->second->m_array_count,
                i->second->m_pparams); break;

        case 2:
            glUniform2iv(loc,
                i->second->m_array_count,
                i->second->m_pparams); break;

        case 3:
            glUniform3iv(loc,
                i->second->m_array_count,
                i->second->m_pparams); break;

        case 4:
            glUniform4iv(loc,
                i->second->m_array_count,
                i->second->m_pparams); break;
        }
    }

    // Pass float parameters to the shader.
    for(f = m_fParams.begin(); f != m_fParams.end(); ++f)
    {
        int loc = this->GetLocation(f->first);

        switch(f->second->m_array_size)
        {
        case 1:
            glUniform1fv(loc,
                f->second->m_array_count,
                f->second->m_pparams); break;

        case 2:
            glUniform2fv(loc,
                f->second->m_array_count,
                f->second->m_pparams); break;

        case 3:
            glUniform3fv(loc,
                f->second->m_array_count,
                f->second->m_pparams); break;

        case 4:
            glUniform4fv(loc,
                f->second->m_array_count,
                f->second->m_pparams); break;
        }
    }

    return ((m_last_error = glGetError()) == GL_NO_ERROR);
}

bool CShader::Unlink()
{
    glUseProgram(0);
    return ((m_last_error = glGetError()) == GL_NO_ERROR);
}

bool CShader::IsLoaded() const
{
    return (m_program > 0 && m_vshader > 0 && m_fshader > 0);
}

int CShader::GetLocation(const char* pvar_name) const
{
    return glGetUniformLocation(m_program, pvar_name);
}

char* CShader::GetError() const
{
    return mp_error_str;
}

GLuint CShader::LoadShaderFile(const char* pfilename, const int shader_type)
{
    g_Log.Flush();
    g_Log << "[INFO] Loading shader: " << pfilename << ".\n";

    std::string source, line;
    GLint length;

    // Load shader file.
    std::ifstream file;
    file.open(pfilename);

    if(!file.is_open())
    {
        mp_error_str = "File does not exist";
        return 0;
    }

    while(std::getline(file, line))
    {
        source += line;
        source += '\n';
    }
    file.close();

    const char* p_src = source.c_str();

    // Create and compile shader.
    GLuint shader = glCreateShader(shader_type);
    length = source.length();
    glShaderSource(shader, 1, &p_src, &length);
    glCompileShader(shader);

    // Check for errors.
    glGetShaderiv(shader, GL_COMPILE_STATUS, &m_last_error);

    if(m_last_error == GL_FALSE)
    {
        // Get log length.
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_error_str != NULL)
            delete[] mp_error_str;

        mp_error_str = new char[length];
        glGetShaderInfoLog(shader, length, &length, mp_error_str);
        glDeleteShader(shader);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to compile '" << pfilename << "'.\n";
        g_Log << "[Error] OpenGL error: " << mp_error_str;
        return 0;
    }

    return shader;
}

GLuint CShader::LoadShaderSource(const char* psrc, const int shader_type)
{
    g_Log.Flush();
    g_Log << "[INFO] Loading shader from source.\n";

    GLint length;

    // Create and compile shader.
    GLuint shader = glCreateShader(shader_type);
    length = strlen(psrc);
    glShaderSource(shader, 1, &psrc, &length);
    glCompileShader(shader);

    // Check for errors.
    glGetShaderiv(shader, GL_COMPILE_STATUS, &m_last_error);

    if(m_last_error == GL_FALSE)
    {
        // Get log length.
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_error_str != NULL)
            delete[] mp_error_str;

        mp_error_str = new char[length];
        glGetShaderInfoLog(shader, length, &length, mp_error_str);
        glDeleteShader(shader);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to compile shader from source.\n";
        g_Log << "[Error] OpenGL error: " << mp_error_str;
        return 0;
    }

    return shader;
}
