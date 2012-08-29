/**
 * @file
 *  Fragment shader for lighting.
 *  Per-pixel point light lighting is done in this shader.
 *
 * @addtogroup Shaders
 * @{
 */

uniform sampler2D   tex;        // Active texture
uniform int         scr_height; // Screen height
uniform vec2        light_pos;  // Light position
uniform vec3        light_col;  // Light color
uniform vec3        light_att;  // Light attenuation

void main()
{
    vec2 pixel      = gl_FragCoord.xy;
    pixel.y         = scr_height - pixel.y;
    vec2 light_vec  = light_pos - pixel;
    float dist      = length(light_vec);
    
    float att       = 1.0 / ( light_att.x + 
                              light_att.y * dist + 
                              light_att.z * dist * dist);
    
    vec4 texel      = texture2D(tex, gl_TexCoord[0]) * gl_Color;
    gl_FragColor    = vec4(att, att, att, 1.0) * vec4(light_col, 1.0) * texel;
}

/** @} */