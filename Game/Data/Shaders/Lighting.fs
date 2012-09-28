#define MAX_LIGHTS 10

/**
 * @file
 *  Fragment shader for lighting.
 *  Per-pixel point light lighting is done in this shader.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Shaders
 * @{
 */
 
uniform sampler2D   tex;                    // Active texture
uniform int         scr_height;             // Screen height
uniform vec2        light_pos[MAX_LIGHTS];  // Light position
uniform vec3        light_col[MAX_LIGHTS];  // Light color
uniform vec3        light_att[MAX_LIGHTS];  // Light attenuation
uniform float       light_brt[MAX_LIGHTS];  // Light brightness

void main()
{
    vec2 pixel      = gl_FragCoord.xy;
    pixel.y         = scr_height - pixel.y;
    
    vec3 lights;
    
    for(int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec2 light_vec  = light_pos[i]  - pixel;
        float dist      = length(light_vec);
    
        float att       = 1.0 / ( light_att[i].x + 
                                 (light_att[i].y * dist) + 
                                 (light_att[i].z * dist * dist));

        lights         += light_col[i] * att * light_brt[i];
    }
    
    vec4 texel      = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
    gl_FragColor    = texel * vec4(lights, 1.0);
}

/** @} */
