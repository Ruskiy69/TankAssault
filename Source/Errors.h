/****************************************
 *             ERRORS.H                 *
 *                                      *
 * Simplified error handling. Simply    *
 * show a MessageBox (Windows) with a   *
 * custom error message and caption,    *
 * or print to stderr (*Nix).           *
 *                                      *
 ***************************************/

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
    void HandleError(const char* message, const char* title = "Error", const bool quit = true);
}

#endif