
                                                   
uniform sampler2D colorMap;                        
                                                   
in vec2           frag_txcd;                       
                                                   
// out vec4          frag_color                    
void main ()                                       
{                                                  
  gl_FragColor = texture2D (colorMap, frag_txcd); 
}                                                  
