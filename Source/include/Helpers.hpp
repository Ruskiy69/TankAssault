/**
 * @file
 *  A collection of helper functions.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1
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
 * @addtogroup Helpers
 *   Contains a collection of helper functions for manipulation of string
 *   and other operations.
 **/
/// @{

#include <string>
#include <sstream>
#include <vector>

/// I can't find this in any C++ header.
typedef unsigned int u_int;

/// Anything that doesn't fit in the game-related namespaces
namespace gk
{
    int num_len(const int number);
    void        toupper(std::string& str);
    std::string toupper_ret(const std::string& str);
    std::string combine(const std::string& str1, const char* str2);
    std::string combine(const char* str2, const std::string& str1);
    std::vector<std::string> split(const std::string& str, char token);
}

/// @}