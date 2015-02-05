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
	//vec4 pos	= vec4 (attrib_position, 1); 
	//pos			= mat_Model * pos;                                                   
	//pos			= mat_View * pos; 
	//gl_Position = mat_Proj * pos; //gl_Position = vec4 (attrib_position, 1);
	//txco		= attrib_texcoord;                                                   

	gl_Position = vec4 (attrib_position, 1);
	txco		= attrib_texcoord;                                                   
}


