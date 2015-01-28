//
// MARS TESSELLATION CONTROL PROGRAM
#version 420

layout(vertices = 4) out;
in vec3 tc_in_Pos[]; 
in vec2	tc_in_TxrCrd[];

// these pass thru
out vec3 tc_out_Position[];
out vec2 tc_out_TxrCrd[];

uniform mat4      mat_Model   ;                                               
uniform mat4      mat_View    ;                                               
uniform mat4      mat_ModelView;                                               
uniform mat4      mat_Proj    ;                                               

uniform float     height_Scale;                                               
uniform sampler2D heightMap;                                                  

// we gonna pass these in
// uniform float tessLevelInner;
// uniform float tessLevelOuter;

void main ()
{
	gl_TessLevelInner[0] = 2.2;
	gl_TessLevelInner[1] = 2.2;

	gl_TessLevelOuter[0] = 2.2;
	gl_TessLevelOuter[1] = 2.2;
	gl_TessLevelOuter[2] = 2.2;
	gl_TessLevelOuter[3] = 2.2;

	tc_out_Position[gl_InvocationID]	= tc_in_Pos[gl_InvocationID];
	tc_out_TxrCrd[gl_InvocationID]		= tc_in_TxrCrd[gl_InvocationID];
}
