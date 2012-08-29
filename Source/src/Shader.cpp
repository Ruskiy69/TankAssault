#include "Shader.h"

using gfx::GL_Shader;
using game::g_Log;

GL_Shader::GL_Shader() : m_shader(0), m_lasterror(GL_NO_ERROR),
    mp_lasterror(NULL), m_loaded(false) {}

GL_Shader::~GL_Shader()
{
    if(m_shader > 0)
    {
        glDeleteShader(m_shader);
    }
}

bool GL_Shader::LoadFromFile(const char* p_filename, const int type)
{
    g_Log.Flush();
    g_Log << "[INFO] Loading shader: " << p_filename << ".\n";

    std::string source, line;
    GLint length;

    // Load shader file.
    std::ifstream file;
    file.open(p_filename);

    if(!file.is_open())
    {
        mp_lasterror = "File does not exist";
        return false;
    }

    while(std::getline(file, line))
    {
        source += line;
        source += '\n';
    }
    file.close();

    const char* p_src = source.c_str();

    // Create and compile shader.
    m_shader = glCreateShader(type);
    length = source.length();
    glShaderSource(m_shader, 1, &p_src, &length);
    glCompileShader(m_shader);

    // Check for errors.
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &m_lasterror);

    if(m_lasterror == GL_FALSE)
    {
        // Get log length.
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_lasterror != NULL)
            delete[] mp_lasterror;

        mp_lasterror = new char[length];
        glGetShaderInfoLog(m_shader, length, &length, mp_lasterror);
        glDeleteShader(m_shader);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to compile '" << p_filename << "'.\n";
        g_Log << "[Error] OpenGL error: " << mp_lasterror;
        return false;
    }

    // Create program object and attach shader.
    m_program = glCreateProgram();
    glAttachShader(m_program, m_shader);

    // Link program
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &m_lasterror);

    if(m_lasterror == GL_FALSE)
    {
        // Get log length.
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_lasterror != NULL)
            delete[] mp_lasterror;

        mp_lasterror = new char[length];
        glGetProgramInfoLog(m_program, length, &length, mp_lasterror);
        glDeleteProgram(m_program);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to link '" << p_filename << "'.\n";
        g_Log << "[Error] OpenGL error: " << mp_lasterror;
        return false;
    }

    m_loaded = true;
    return true;
}

bool GL_Shader::Link()
{
    glUseProgram(m_program);
    return (glGetError() == GL_NO_ERROR);
}

bool GL_Shader::Unlink()
{
    glUseProgram(0);
    return (glGetError() == GL_NO_ERROR);
}

bool GL_Shader::LoadFromSource(const char* p_src, const int type)
{
    g_Log.Flush();
    g_Log << "[INFO] Loading shader from raw source.\n";

    GLint length;

    // Create and compile shader.
    m_shader = glCreateShader(type);
    length = strlen(p_src);
    glShaderSource(m_shader, 1, &p_src, &length);
    glCompileShader(m_shader);

    // Check for errors.
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &m_lasterror);
    if(m_lasterror == GL_FALSE)
    {
        glDeleteShader(m_shader);

        // Get log length.
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_lasterror != NULL)
            delete[] mp_lasterror;

        mp_lasterror = new char[length];
        glGetShaderInfoLog(m_shader, length, &length, mp_lasterror);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to compile raw shader.\n";
        g_Log << "[Error] OpenGL error: " << mp_lasterror;
        return false;
    }

    // Create program object and attach shader.
    m_program = glCreateProgram();
    glAttachShader(m_program, m_shader);

    // Link program
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &m_lasterror);
    if(m_lasterror == GL_FALSE)
    {
        glDeleteProgram(m_program);

        // Get log length.
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

        // Get log details.
        if(mp_lasterror != NULL)
            delete[] mp_lasterror;

        mp_lasterror = new char[length];
        glGetProgramInfoLog(m_shader, length, &length, mp_lasterror);

        g_Log.Flush();
        g_Log << "[ERROR] Failed to link raw shader.\n";
        g_Log << "[Error] OpenGL error: " << mp_lasterror;
        return false;
    }

    m_loaded = true;
    return true;
}

char* GL_Shader::GetError() const
{
    return mp_lasterror;
}

int GL_Shader::GetLocation(const char* param)
{
    return glGetUniformLocation(m_program, param);
}

bool GL_Shader::IsLoaded() const
{
    return m_loaded;
}