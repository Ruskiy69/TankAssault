/**
 * @file
 *  Definitions for various helper functions.
 *
 * @author  George Kudrayvtsev
 * @version 1.1
 **/

#include <sstream>

#include "Helpers.hpp"

/**
 * Counts the number of numbers in a number.
 * @param int number to count numbers of
 * @return Number of numbers in number.
 **/
int gk::num_len(const int number)
{
    int tmp     = number;
    int count   = 0;

    while(tmp > 0)
    {
        tmp = tmp / 10;
        count++;
    }
    return count;
}

/**
 * Convert a string to its uppercase equivalent.
 *  This directly modifies the original string argument.
 *  
 * @param str string to convert
 **/
void gk::toupper(std::string& str)
{
    for(u_int i = 0; i < str.length(); ++i)
        str[i] = ::toupper(str[i]);
}

/**
 * Convert a string to its uppercase equivalent.
 *  This returns a new string, converted, leaving the given one untouched.
 *  
 * @param std::string string to convert
 * @return Uppercase-converted string.
 **/
std::string gk::toupper_ret(const std::string& str)
{
    std::string tmp(str);
    for(u_int i = 0; i < str.length(); ++i)
        tmp[i] = ::toupper(str[i]);
    return tmp;
}

/**
 * Combine a C++ style string and a C string into one.
 *
 * @param std::string String one
 * @param char* String two
 * @return String one + string two.
 **/
std::string gk::combine(const std::string& str1, const char* pstr2)
{
    std::stringstream ss;
    ss << str1 << pstr2;
    return ss.str();
}

/**
 * Combine a C style string and a C++ string into one.
 *
 * @param char* String two
 * @param std::string String one
 * @return String one + string two.
 **/
std::string gk::combine(const char* pstr1, const std::string& str2)
{
    std::stringstream ss;
    ss << pstr1 << str2;
    return ss.str();
}

/**
 * Splits a string into individual parts at an special character.
 *
 * @param std::string string to split
 * @param char special character to split string with
 * @return Parts of the string split at special character.
 **/
std::vector<std::string> gk::split(const std::string& str, char token)
{
    std::string tmp(str);
    std::stringstream ss;
    std::vector<std::string> results;
    int index;

    while(true)
    {
        index = tmp.find(token);
        if(index == -1 || index == std::string::npos)
            break;

        results.push_back(tmp.substr(0, index));
        tmp = tmp.substr(index + 1, -1);
    }
    results.push_back(tmp);

    return results;
}
