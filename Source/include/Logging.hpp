/**
 * @file
 *	Declarations for the CLogging class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
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
 * @addtogroup Game
 **/
/// @{

#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <ctime>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace gk
{
    /**
     * A singleton class that handles logging operations to a file
     * called LastRun.log.
     **/
    class CLogging
    {
    public:
        virtual ~CLogging();

        /**
         * Logs given data.
         *  Due to its templated nature, and the versatility of
         *  std::ostream, anything can be passed as an argument.
         *  The given data is written to an std::stringstream, and is only
         *  writted to the file when CLogging::Flush() is called.
         *
         * @param template<class T> Data to log
         * @return A reference to the CLogging class so that streaming
         *  can be grouped together.
         **/
        template<class T> CLogging& operator<< (T data);

        /**
         * Closes the logging file.
         *  A CLogging::Flush() operation is performed before closing.
         **/
        void Close();


        /**
         * Writes pending data to the file.
         *  The std::stringstream is written to the logging file, then
         *  cleared. Thus, calls to GetLastLog() will return a blank string. 
         *  
         * @see CLogging::GetLastLog()
         **/
        void Flush();
        void ShowLastLog() const;
        std::string GetLastLog() const;

        static CLogging& GetInstance()
        {
            static CLogging g_Log("LastRun.log");
            return g_Log;
        }

    private:
        CLogging(const char* p_filename);
        CLogging(const CLogging& Log);
        void operator= (const CLogging& Log);

        std::stringstream m_log;
        std::ofstream m_file;
    };

    template<class T> CLogging& CLogging::operator<< (T data)
    {
        m_log << data;
        return *this;
    }
}

#endif // LOGGING_HPP

/// @}