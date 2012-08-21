/**
 * @file
 *  Declarations for the AL_Sound2D class.
 * 
 * @author George Kudrayvtsev
 * @version 1.1
 */

#include "Assets/Sound2D.h"

using asset::AL_Sound2D;
using game::g_Log;

AL_Sound2D::AL_Sound2D() : m_buffer(0), m_source(-1),
    m_lasterror(AL_NO_ERROR) {}

AL_Sound2D::~AL_Sound2D()
{
    if(s_sources[m_source] != 0)
    {
        alDeleteSources(1, &s_sources[m_source]);
        s_sources[m_source] = 0;
    }

    alDeleteBuffers(1, &m_buffer);
}

bool AL_Sound2D::InitializeOpenAL()
{
    static bool once = false;

    if(!once)
    {
        memset(s_sources, 0, sizeof s_sources);
        alutInit(NULL, NULL);
        if(alutGetError() != ALUT_ERROR_NO_ERROR)
            return false;
        alGetError();
        once = true;
    }

    return true;
}

void AL_Sound2D::GetAvailableSource()
{
    // First, check if the current available source is
    // in range.
    if(s_available < 0 || s_available >= 256)
        s_available = 0;

    // Now, check if the current available one is.. well..
    // available.
    if(s_sources[s_available] == 0)
        return;

    // Then, check if the source immediately
    // following the current one is okay.
    if(s_available < 255 && s_sources[s_available + 1])
    {
        s_available++;
        return;
    }

    // Now, iterate through the sources array to find 
    // the next unused one.
    for(size_t i = 0; i < sizeof s_sources; ++i)
    {
        if(s_sources[i] == 0)
        {
            s_available = i;
            return;
        }
    }

    // There is nothing available.
    s_available = -1;
}

bool AL_Sound2D::LoadFromAudio(AL_Sound2D* const p_Copy)
{
    p_Copy->IncrementReferenceCount();
    m_buffer    = p_Copy->GetBuffer();
    m_filename  = p_Copy->GetFilename();
    m_refcount  = p_Copy->GetReferenceCount();
    m_source    = -1;

    return true;
}

bool AL_Sound2D::LoadFromFile(const char* p_filename)
{
    /// Buffer size for .ogg decoding (32 bytes).
    static const int BUFFER_SIZE = 32768;

    // Variables for libvorbis decoding.
    vorbis_info*        p_Info              = NULL; // 
    OggVorbis_File      ogg_file;                   // Information about the file.
    std::vector<char>   buffer;                     // .ogg data
    char                array[BUFFER_SIZE];         // Temporary data
    int                 bit_stream;
    int                 bytes_read;                 // Bytes read on each call
    int                 format, freq;               // .ogg information
    int                 endian              = 0;    // 0 is little endian, 1 is big endian
    FILE*               p_File              = NULL; // Raw .ogg file

    m_lasterror = AL_NO_ERROR;

    // Check if there's already something loaded.
    if(m_buffer != 0)
    {
        alDeleteBuffers(1, &m_buffer);
    }
    if(m_source != -1)
    {
        if(s_sources[m_source] != 0)
            this->UnloadSource();
    }

    // Check for a valid filename.
    if(p_filename == NULL)
    {
        m_lasterror = AL_INVALID_NAME;
        return false;
    }

    // Determine if the given file is .ogg or not.
    // Open the .ogg file in binary-read mode.
    p_File = fopen(p_filename, "rb");
    if(p_File == NULL)
    {
        m_lasterror = AL_INVALID_NAME;
        return false;
    }

    // Initialize OggVorbis_File structure and check for valid ogg
    if(ov_open_callbacks(p_File, &ogg_file, NULL, 0, OV_CALLBACKS_DEFAULT) < 0)
    {
        ov_clear(&ogg_file);
        fclose(p_File);

        // The file isn't .ogg, so try and load it as a .wav.
        return this->LoadFromFile_WAV(p_filename);
    }

    // Get information from the file.
    p_Info = ov_info(&ogg_file, -1);

    if(p_Info->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
    freq = p_Info->rate;

    // Decode the data
    do
    {
        // Read 32 bytes into array.
        bytes_read = ov_read(&ogg_file, array,
            BUFFER_SIZE, endian, 2, 1, &bit_stream);

        // Insert to all data array.
        buffer.insert(buffer.end(), array, array + bytes_read);
    }
    while(bytes_read > 0);

    // Clean up memory
    ov_clear(&ogg_file);

    // Generate OpenAL audio buffers from raw OGG data.
    alGenBuffers(1, &m_buffer);
    if((m_lasterror = alGetError()) != AL_NO_ERROR)
        return false;

    alBufferData(m_buffer, format, &buffer[0], buffer.size(), freq);
    if((m_lasterror = alGetError()) != AL_NO_ERROR)
        return false;

    m_filename  = p_filename;
    m_loaded    = true;

    return true;
}

bool AL_Sound2D::UnloadSource()
{
    if(m_source == -1)
        return false;

    alDeleteSources(1, &s_sources[m_source]);
    s_sources[m_source] = 0;

    return ((m_lasterror = alGetError()) == AL_NO_ERROR);
}

bool AL_Sound2D::Play()
{
    if(this->GetAudioState() == AL_PLAYING)
        return true;

    AL_Sound2D::GetAvailableSource();
    if(s_available == -1)
    {
        m_lasterror = AL_OUT_OF_MEMORY;
        return false;
    }

    ALuint source = 0;
    alGenSources(1, &s_sources[s_available]);
    if((m_lasterror = alGetError()) != AL_NO_ERROR)
        return false;
    m_source = s_available;

    alSourcei(s_sources[m_source], AL_BUFFER, m_buffer);
    if((m_lasterror = alGetError()) != AL_NO_ERROR)
        return false;

    alSourcePlay(s_sources[m_source]);

    g_Log.Flush();
    g_Log << "[DEBUG] Playing " << m_filename << ".\n";
    g_Log.ShowLastLog();

    return true;
}

bool AL_Sound2D::Pause()
{
    if(!m_loaded || m_source == -1)
        return false;

    if(this->GetAudioState() == AL_PAUSED)
        return true;

#ifdef _DEBUG
    std::cout << "Paused " << m_filename << ".\n";
#endif // _DEBUG
    alSourcePause(s_sources[m_source]);

    return true;
}

bool AL_Sound2D::Stop()
{
    if(!m_loaded || m_source == -1)
        return false;

    if(this->GetAudioState() == AL_STOPPED)
        return true;

#ifdef _DEBUG
    std::cout << "Stopped " << m_filename << ".\n";
#endif // _DEBUG
    alSourceStop(s_sources[m_source]);
    return true;
}

void AL_Sound2D::SetPosition(const math::ML_Vector2& Pos)
{
    if(m_source != -1)
        alSource3f(s_sources[m_source], AL_POSITION, Pos.x, Pos.y, 0.0f);
}

void AL_Sound2D::SetVelocity(const math::ML_Vector2& Vel)
{
    if(m_source != -1)
        alSource3f(s_sources[m_source], AL_VELOCITY, Vel.x, Vel.y, 0.0f);
}

void AL_Sound2D::SetDirection(const math::ML_Vector2& Dir)
{
    if(m_source != -1)
        alSource3f(s_sources[m_source], AL_DIRECTION, Dir.x, Dir.y, 0.0f);
}

int AL_Sound2D::GetAudioState() const
{
    if(m_source == -1)
        return AL_INVALID_OPERATION;

    int state;
    alGetSourcei(s_sources[m_source], AL_SOURCE_STATE, &state);
    alGetError();
    return state;
}

ALenum AL_Sound2D::GetLastError() const
{
    return m_lasterror;
}

ALuint AL_Sound2D::GetBuffer() const
{
    return m_buffer;
}

ALuint AL_Sound2D::GetSourceIndex() const
{
    return m_source;
}

ALuint AL_Sound2D::GetSource() const
{
    if(m_source != -1)
        return s_sources[m_source];
    else
        return -1;
}

bool AL_Sound2D::LoadFromFile_WAV(const char* p_filename)
{
    alGenBuffers(1, &m_buffer);
    if((m_lasterror = alGetError()) != AL_NO_ERROR)
        return false;

    m_buffer = alutCreateBufferFromFile(p_filename);
    if(m_buffer == AL_NONE || ((m_lasterror = alutGetError()) != AL_NO_ERROR))   
    {
        alDeleteBuffers(1, &m_buffer);
        return false;
    }

    m_filename  = p_filename;
    m_loaded    = true;

    return true;
}