//
// MARS VERTEX PROGRAM
//

#version 420

// input assembly 
uniform mat4      mat_Model		;                                               
uniform mat4      mat_View		;                                               
uniform mat4      mat_ModelView	;                                               
uniform mat4      mat_Proj		;                                               

uniform sampler2D heightMap		;                                                  
uniform float     heightScale	;                                               

// vertex input 
in vec3      coord_EN;                                                        
in vec2      texCoord;                                                        
                                                                                       
// vertex output
out vec2 txrCrd;                                                   

void main ()                                                                  
{                                                                             
  
  float height_Coord   = heightScale * texture2D (heightMap, texCoord).r;   
  vec4  posi           = vec4 (coord_EN.x, height_Coord , coord_EN.y, 1.0);   

  // output

  //  tx cd
  txrCrd = texCoord;                                                    

  // h pos
  posi = mat_Model * posi;                                                   
  posi = mat_View * posi;                                                    
  gl_Position = mat_Proj * posi;
}                                                                             


