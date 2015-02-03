//
// MARS TESSELLATION EVALUATION PROGRAM
//
#version 420

layout(quads, equal_spacing, cw) in;

uniform mat4		mat_Model   ;                                               
uniform mat4		mat_View    ;                                               
uniform mat4		mat_ModelView;                                               
uniform mat4		mat_Proj    ;                                               
uniform float		heightScale;                                               
uniform sampler2D	heightMap;                                                  

in vec4				te_in_position[];
in vec2				te_in_txcrd[]; 

//quad interpol
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

void main()
{
   vec4 p0 = gl_TessCoord.x * te_in_position[0];
   vec4 p1 = gl_TessCoord.y * te_in_position[1];
   vec4 p2 = gl_TessCoord.z * te_in_position[2];
   vec4 p4 = gl_TessCoord.z * te_in_position[2];

   gl_Position = interpolate (
	   te_in_position[0], 
	   te_in_position[1], 
	   te_in_position[2], 
	   te_in_position[2]
	   );
}
