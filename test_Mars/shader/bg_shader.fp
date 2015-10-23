
                                                   
uniform sampler2D colorMap;                        
                                                   
in vec2           frag_txcd;                       
                                                   
// out vec4          frag_color                    
void main ()                                       
{

	gl_FragColor.rgb = texture2D (colorMap, frag_txcd).rrr; 
	gl_FragColor.a = 1.0; 
}                                                  
