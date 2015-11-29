#version 420

                                                   
uniform sampler2D colorMap;                        
                                                   
in vec2     frag_txcd;                       
in float	diffuseIntensity; 
                                     
// out vec4          frag_color                    
void main ()                                       
{                          
	vec4 color = texture2D (colorMap, frag_txcd);                        
  gl_FragColor = max ( (diffuseIntensity * color) , (0.3 * color)); 
}                                                  
