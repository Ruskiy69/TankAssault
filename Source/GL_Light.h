/****************************************
 *             GL_LIGHT.H               *
 *                                      *
 * A lighting class to easily create,   *
 * move, and modify the various types   *
 * of OpenGL lights, such as ambient    *
 * or diffusive.                        *
 *                                      *
 ***************************************/

#ifndef GL_LIGHT_H
#define GL_LIGHT_H

#include "Errors.h"
#include "SDL_GL_Base.h"

class GL_Light
{
public:
    GL_Light(const GLint type);

    void SetPosition(const GLfloat x,
        const GLfloat y,
        const GLfloat z);

    void SetIntensity(const GLfloat r,
        const GLfloat g,
        const GLfloat b,
        const GLfloat a = 1.0f);

    void SetToggle(bool value);

    static bool ToggleLighting();

private:
    GLfloat Intensity[4];
    GLfloat Position[4];

    bool toggle;
    int  type;
};

#endif // GL_LIGHT_H