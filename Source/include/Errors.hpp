/**
 * @file
 *  Simplified error handling. Shows a MessageBox (Windows) with a
 *  custom error message and caption, or print to stderr (*Nix).
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0.1
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
 **/
/// @{

#ifndef ERRORS_H
#define ERRORS_H

#ifdef __cplusplus
 #include <cstdlib>
#else
 #include <stdlib.h>
#endif // __cplusplus

#ifdef _WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <Windows.h>
#else
 #ifdef __cplusplus
  #include <cstdio>
 #else 
  #include <stdio.h>
 #endif // __cplusplus
#endif // _WIN32

namespace gk
{
    void handle_error(
        const char* message,            // Message that pops up
        const char* title = "Error",    // Window title
        const bool quit = true);        // Quit after showing?
}

#endif // ERRORS_HPP

/// @}