//
#version 420


// MARS VERTEX PROGRAM
//

////
// input vertex 
in vec3						coord_EN		;                                                        
in vec2						texCoord		;                                                        

////
// uniform constants 
uniform mat4				mat_Model		;                                               
uniform mat4				mat_View		;                                               
uniform mat4				mat_ModelView	;                                               
uniform mat4				mat_Proj		;                                               
uniform sampler2D			heightMap		;                                                  
uniform float				heightScale		;                                               

////
// output vertex 
out vec2 txrCrd;                                                   
out vec4 gl_Position; 
////
// 
void main ()
{                                                                             
  float height_Coord   = heightScale * texture2D (heightMap, texCoord).r;   
  vec4  posi           = vec4 (coord_EN.x, height_Coord , coord_EN.y, 1.0);   

  // output
  txrCrd = texCoord;                                                    

  // h pos
  posi = mat_Model * posi;                                                   
  posi = mat_View * posi;                                                    
  gl_Position = mat_Proj * posi;
}                                                                             


