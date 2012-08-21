/**
 * @file
 *	Declarations for the Logging class.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Game
 */
/// @{

#ifndef LOGGING_H
#define LOGGING_H

#include <ctime>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace gk
{
    class Logging
    {
    public:
        virtual ~Logging();

        template<class T> Logging& operator<< (T data);

        void Close();
        void Flush();
        void ShowLastLog() const;
        std::string GetLastLog() const;

        static Logging& GetInstance()
        {
            static Logging g_Log("LastRun.log");
            return g_Log;
        }

    private:
        Logging(const char* p_filename);
        Logging(const Logging& Log);
        void operator= (const Logging& Log);

        std::stringstream m_log;
        std::ofstream m_file;
    };

    template<class T> Logging& Logging::operator<< (T data)
    {
        m_log << data;
        return *this;
    }
}

#endif // LOGGING_H

/// @}