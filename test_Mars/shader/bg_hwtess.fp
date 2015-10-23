#version 420
                                                   
uniform sampler2D colorMap;                        
//uniform sampler2D normalMap;

                                                      
in  vec2          otxcd;                       
out vec4          frag;

   const vec3 kMarsCol = vec3(1.0, 0.8, 0.7);                     

void main ()                                       
{
	frag = vec4 (texture2D (colorMap, otxcd).r * kMarsCol , 1.0); 
}                                                  
