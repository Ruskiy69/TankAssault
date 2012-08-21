/**
 * @file
 *  A collection of helper functions.
 *
 * @author George Kudrayvtsev
 * @version 1.1
 *
 * @addtogroup Helpers
 *   Contains a collection of helper functions for manipulation of string
 *   and other operations.
 */
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