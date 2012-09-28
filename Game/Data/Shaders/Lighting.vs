#define MAX_LIGHTS 3
/**
 * @file
 *  Vertex shader for lighting.
 *  Per-pixel point light lighting is done in this shader pair.
 *  In this part, the vertex shader, the vector from the light to the vertex
 *  is calculated, and light properties like attenuation are calculated.
 *
 * @addtogroup Shaders
 * @{
 */
 
void main()
{
    gl_Position     = ftransform();
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    gl_FrontColor   = gl_Color;
}

/** @} */
