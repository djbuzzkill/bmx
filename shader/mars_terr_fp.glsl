#version 420


//
// MARS FRAGMENT PROGRAM
                                                   
const vec3 mars_color = vec3 (1.0, 0.8, 0.6);

uniform sampler2D colorMap;                        
in vec2           frag_txcd;                       
out vec4          frag_color;                    

void main ()                                       
{
	frag_color.rgb	= texture2D (colorMap, frag_txcd).r * mars_color;
	frag_color.a	= 1.0; 
}                                                  
