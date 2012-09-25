/**
 * @file
 *	Function definitions for various calculations and conversions.
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
 * @addtogroup Math
 **/
/// @{

#ifndef MATH__MATH_DEF_HPP
#define MATH__MATH_DEF_HPP

#include <cmath>
#include "Errors.hpp"

/// Couldn't find it in any default headers
typedef unsigned int u_int;

/// Contains all math-related data.
namespace math
{
    /// Well-known constant. Special thanks to Windows calculator.
    static const float PI = 3.14159265358979323846f;

    float rad(const float x);
    float deg(const float x);
    float distance(const float x1, const float y1,
        const float x2, const float y2,
        bool do_sqrt = true);
}

#endif // MATH__MATH_DEF_HPP

/// @}