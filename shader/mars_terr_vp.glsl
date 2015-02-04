#version 420


// MARS VERTEX PROGRAM
//

////
// input vertex 
layout(location=0) in vec3 attrib_position;                                                        
layout(location=1) in vec2 attrib_texcoord;                                                        

////
// uniform constants 
                                             

////
// output vertex 
out vec2 txco;                                                   
out vec4 gl_Position; 


// 
void main ()
{
	txco		= attrib_texcoord;                                                   
	gl_Position = vec4 (attrib_position, 1);
}


