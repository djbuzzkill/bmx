#version 420

//
// MARS TESSELLATION EVALUATION PROGRAM

layout(quads, equal_spacing, cw) in;

uniform mat4		mat_Model   ;                                               
uniform mat4		mat_View    ;                                               
uniform mat4		mat_ModelView;                                               
uniform mat4		mat_Proj    ;                                               
uniform float		heightScale;                                               
uniform sampler2D	heightMap;                                                  

in vec4				te_in_position[];
in vec2				texcoord[]; 


out vec4 gl_Position; 
out vec2 texCoord; 
//
//
vec2 interpolate2 (in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

//quad interpol
vec4 interpolate4(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}


//
//
void main()
{
   texCoord = interpolate2 (
	   texcoord[0],
	   texcoord[1],
	   texcoord[2],
	   texcoord[3]
	   ); 

	vec4 pos = interpolate4 (
	   te_in_position[0], 
	   te_in_position[1], 
	   te_in_position[2], 
	   te_in_position[3]
	   );

	pos.z = heightScale * texture2D (heightMap, texCoord).r;

	pos = mat_Model * pos;                                                   
	pos = mat_View * pos;                                                    
	gl_Position = mat_Proj * pos;
}				

