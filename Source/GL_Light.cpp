#include "GL_Light.h"

GL_Light::GL_Light(const GLint type)
{
    this->type = type;
}

void GL_Light::SetPosition(const GLfloat x,
        const GLfloat y,
        const GLfloat z)
{
    this->Position[0] = x;
    this->Position[1] = y;
    this->Position[2] = z;
    this->Position[3] = 1.0f;

    glLightfv(GL_LIGHT1, GL_POSITION, this->Position);
}

void GL_Light::SetIntensity(const GLfloat r,
        const GLfloat g,
        const GLfloat b,
        const GLfloat a)
{
    this->Intensity[0] = r;
    this->Intensity[1] = g;
    this->Intensity[2] = b;
    this->Intensity[3] = a;

    glLightfv(GL_LIGHT1, this->type, this->Intensity);
}

void GL_Light::SetToggle(bool flag)
{
    bool value = GL_Light::ToggleLighting();

    if(value != flag)
        GL_Light::ToggleLighting();
}

bool GL_Light::ToggleLighting()
{
    static bool toggle = true;
    toggle = !toggle;

    if(toggle)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    return toggle;
}