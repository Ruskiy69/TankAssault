/**
 * @file
 *  Vertex shader for lighting.
 *  This shader does nothing but the standard transformation.
 *
 * @addtogroup Shaders
 * @{
 */

void main()
{
    gl_Position     = ftransform();
    gl_TexCoord[0]  = gl_MultiTexCoord0;
}

/** @} */