/**
 * @file
 *  Fragment shader for lighting.
 *  Per-pixel point light lighting is done in this shader.
 *
 * @addtogroup Shaders
 * @{
 */

#define MAX_LIGHTS  1
 
uniform sampler2D   tex;                    // Active texture
uniform int         scr_height;             // Screen height
uniform vec2        light_pos[MAX_LIGHTS];  // Light position
uniform vec3        light_col[MAX_LIGHTS];  // Light color
uniform vec3        light_att[MAX_LIGHTS];  // Light attenuation

void main()
{
    vec2 pixel      = gl_FragCoord.xy;
    pixel.y         = scr_height - pixel.y;
    
    vec4 lights;
    
    for(int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec2 light_vec  = light_pos[i]  - pixel;
        float dist      = length(light_vec);
    
        float att       = 1.0 / ( light_att[i].x + 
                                  light_att[i].y * dist + 
                                  light_att[i].z * dist * dist);

        lights         += vec4(light_col[i], 1.0) * vec4(att, att, att, 1.0);
    }
    
    vec4 texel      = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
    gl_FragColor    = texel * lights;
}

/** @} */