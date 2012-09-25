/**
 * @file
 *  Error handling function definitions.
 *
 * @author George Kudrayvtsev
 * @version 1.0.1
 **/

#include "Errors.hpp"

/**
 * Handles errors.
 *
 * @param char* Error message
 * @param char* Error popup title (optional)
 * @param int   Quit after error popup or not
 **/
void gk::handle_error(const char* message, const char* title, const bool quit)
{
#ifdef _WIN32
    MessageBox(NULL, message, title, MB_ICONEXCLAMATION | MB_OK | MB_ICONERROR);
#else
    if(quit)
        fprintf(stderr, "[-] FATAL! %s", errorMessage);
        fprintf(stderr, "Press ENTER to exit...");
    else
        fprintf(stderr, "[-] A non fatal error occurred: %s", errorMessage);
        fprintf(stderr, "Press ENTER to continue...");
    gets();
#endif // _WIN32

    if(quit)
        exit(1);
}
