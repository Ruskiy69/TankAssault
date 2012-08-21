#include "Logging.h"

using gk::Logging;

Logging::Logging(const char* p_filename)
{
    time_t now;
    time(&now);

    m_file.open(p_filename, std::ios::out);
    if(!m_file.is_open())
        std::cerr << "[ERROR] Unable to open logging file.\n";

    m_file << "[INFO] Log initialized on " << ctime(&now);
    this->Flush();
}

Logging::~Logging()
{
    if(!m_file.is_open())
        return;

    time_t now;
    time(&now);

    this->Flush();
    m_log << "[INFO] Log closed on " << ctime(&now);
    this->Flush();

    m_file.close();
}

void Logging::Close()
{
    if(!m_file.is_open())
        return;

    time_t now;
    time(&now);

    this->Flush();
    m_log << "[INFO] Log closed on " << ctime(&now);
    this->Flush();

    m_file.close();
}

void Logging::Flush()
{
    // Don't write things with [DEBUG] tag in the release build.
#ifdef _DEBUG
    m_file << m_log.str();
#else
    if(m_log.str().find("[DEBUG]") == std::string::npos)
        m_file << m_log.str();
#endif // _DEBUG

    m_file.flush();
    m_log.str(std::string());
}

void Logging::ShowLastLog() const
{
    std::cout << m_log.str();
    if(m_log.str().back() != '\n')
        std::cout << std::endl;
}

std::string Logging::GetLastLog() const
{
    return m_log.str();
}
