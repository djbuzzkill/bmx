#version 420

//
// MARS TESSELLATION CONTROL PROGRAM
layout(vertices = 4) out;

in gl_PerVertex 
{

	vec4 gl_Position; 

} gl_in[]; 

// these pass thru
out gl_PerVertex
{

	vec4 gl_Position; 

} gl_out[]; 


in	vec2 txco[];
out vec2 texcoord[];

//
void main ()
{
	if (gl_InvocationID == 0) 
	{

	gl_TessLevelInner[0] = 2.2;
	gl_TessLevelInner[1] = 2.2;

	gl_TessLevelOuter[0] = 2.2;
	gl_TessLevelOuter[1] = 2.2;
	gl_TessLevelOuter[2] = 2.2;
	gl_TessLevelOuter[3] = 2.2;

	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; 
	texcoord[gl_InvocationID]			   = txco[gl_InvocationID]; 

}
