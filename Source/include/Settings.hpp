/**
 * @file
 *  Declarations for the CSettings class.
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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Errors.hpp"
#include "Helpers.hpp"

namespace game
{
    /**
     * Handles a settings file (typically Settings.ini), 
     * which is supposed to contain critical game values
     * such as sprite locations.
     **/
    class CSettings
    {
    public:
        ~CSettings();

        static CSettings& GetInstance();

        bool Load(const std::string& filename);

        std::string ChooseValueAt(const std::string& tag);
        std::string GetValueAt(const std::string& tag);

    private:
        CSettings(){}
        CSettings(const CSettings&);
        CSettings& operator= (const CSettings&);

        std::ifstream m_settingsfile;
    };
}

#endif // SETTINGS_HPP

/// @}