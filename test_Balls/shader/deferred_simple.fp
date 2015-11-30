
#version 420
                                           
uniform sampler2D albedoMap;                        
                                                   
in vec3           frag_posi; 
in vec3           frag_norm; 
in vec2           frag_txcd;                       
in float          frag_depth; 
                                                   
// out vec4          frag;

void main ()                                       
{
	gl_FragColor.rgb = texture2D (albedoMap, frag_txcd).rrr; 
	gl_FragColor.a = 1.0; 
}                                                  
