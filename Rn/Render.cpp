#include "Render.h"


namespace Rn 
{

	// predefine vertex input
	//const std::string _vertex_attribute_names_[_VertexAttributeMaxCount_] = {
	//	"vert_Position",	// VA_Position = 0, 
	//	"vert_Normal",		// VA_Normal, 
	//	"vert_Tangent",		// VA_Tangent,
	//	"vert_TexCrd0",		// VA_TexCrd0, 
	//	"vert_TexCrd1",		// VA_TexCrd1, 
	//	"vert_BoneIndex",	// VA_BoneIndex, 
	//	"vert_BoneWeight",	// VA_BoneWeight, 
	//	}; 


}


/****************************************************************
 * 
 ***************************************************************/	
//int va_sz_comp[] = {
//	sizeof(vector3f), sizeof(vector3f),  sizeof(vector3f), 
//	sizeof(col4b),  sizeof(col4b), 
//	sizeof(vector2f), sizeof(vector2f),
//	sizeof(tuple4uc), sizeof(vector4f),
//};

// number of components
//const int va_num_comp[] = {
//	3, 3, 3,  
//	4, 4,  
//	2, 2,  
//	4, 4 
//	};
//

//
//const unsigned sizeof_vertex_attrib[] = {
//	sizeof(Ma::Vec3f),	//VA_Vertex = 0, 
//	sizeof(Ma::Vec3f),	//VA_Normal, 
//	sizeof(Ma::Vec3f),	//VA_Tangent,
//	sizeof(Ma::Vec2f),	//VA_Tex0Coord, 
//	sizeof(Ma::Vec2f),	//VA_Tex1Coord, 
//	sizeof(Ma::Col4ub), //VA_BoneIndex, 
//	sizeof(Ma::Vec4f)	//VA_BoneWeight, 
//	};


//const unsigned sizeof_index_type[] = {
//	sizeof(unsigned char), //Index8 = 0, 
//	sizeof(unsigned short), //Index16, 
//	sizeof(unsigned int) //Index32,
//	};

//
//bool ShaderProg::Build () 
//{ 
	//Debug ( "\nShaderProg::Build - %s\n", shaderName); 
	//Assert (progID == 0, "ShaderProg::Build () - progID not 0");

	//if (program_ID == 0) { 
	//	program_ID = glCreateProgram ();
	//}
	//else 
	//{
	//	// BreakAssert (GL_TRUE == glIsProgram (progID), "invalid shader program"); 
	//}

	//Assert (progID || glIsProgram (progID), "ShaderProg::Build () - invalid progID"); 
	//Assert (fragShader, ""); 
	//Assert (vertShader, ""); 
		
	// compile vert & frag
	//bool compileRes = ShaderProg::Compile(); 

	// bind our fixed positions
//for (size_t i = 0; i < vertAttribs.size (); i++) {
//	glBindAttribLocation (progID, vertAttribs[i]->AttribUsage(), vertAttribs[i]->Label());  	
//	BreakOnGLError_ ();
//	}

	// attach 
	//glAttachShader (progID, vert_Shader.GetID ());

	//glAttachShader (progID, frag_Shader.GetID ());
	
//	if (ShaderProg::Link ())  {
//for (size_t i = 0; i < vertAttribs.size (); i++)  {
//	vertAttribs[i]->SetLocation (glGetAttribLocation (progID, vertAttribs[i]->Label()));
//	if (vertAttribs[i]->Location() < 0) {
//		Debug ("\n===> Warning : Attribute %s not found", vertAttribs[i]->Label());				
//	}
//	else  {
//		Debug ("\n......Attribute %s at location %i", vertAttribs[i]->Label(), vertAttribs[i]->Location());				
//	}
//	}
		

		// these change for each shader, keep 

//for (int i = PredefinedVariableRangeBegin; i < PredefinedVariableRangeEnd; i++) {
//	char* dbgString = sgShaderVarPredef[i].Label();
//	preDefLocation[i] = glGetUniformLocation (progID, sgShaderVarPredef[i].Label());  
//	}

//for (size_t i = 0; i < uniformVars.size(); i++) {
//	uniformVars[i]->SetLocation (glGetUniformLocation (progID, uniformVars[i]->Label()));
//	if (uniformVars[i]->Location () < 0)  {
//		Debug ( "\n===> Warning : Bind Uniform %s failed", uniformVars[i]->Label());				
//
//		if (uniformVars[i]->ReqMemSize ()) {
//			//uniformVars[i]->SetMemPtr (0);  
//			uniformVars[i]->SetMemPtr (GetShaderConstMem (this, uniformVars[i]->ReqMemSize ()));  
//		}
//	}
//	else {
//		Debug ( "\n......Uniform %s at location, %i", uniformVars[i]->Label(), uniformVars[i]->Location());
//
//		if (uniformVars[i]->ReqMemSize ())  {
//			uniformVars[i]->SetMemPtr (GetShaderConstMem (this, uniformVars[i]->ReqMemSize ()));  
//		}
//	}
//}
//		DB::AddShader (this);
//		return true;	
//	}
//	else {
//		// Assert (0, "\nLinkShaderProgram() failed\n");
//		return false;
//	}
//
//	return false; 
//}

/* *******************************************************
 * 
 *******************************************************/ 
//bool ShaderProg::Compile () { 
//	bool vertRes = vertShader->Compile ();
//	bool fragRes = fragShader->Compile ();
////	Assert (vertRes && fragRes, "ShaderProg::Compile () - Fail"); 
//	return (vertRes && fragRes);
//}

/* *******************************************************
 * 
 ********************************************************/
//bool ShaderProg::Link () 
//{ 
//	glLinkProgram (progID);
// 
//	GLint linkRes;
//	GLsizei bufflen;
//	char outputbuffer[1024];
//	glGetProgramiv (progID, GL_LINK_STATUS, &linkRes);
//	if (linkRes == GL_FALSE)
//	{
//		glGetProgramInfoLog (progID, 1024, &bufflen, outputbuffer);
//		//Debug ( "\nGL Info Log :  \n\n %s\n", outputbuffer);
//		//Assert (0, "ShaderProg::Link () - Failed");
//		return false;
//	}
//	return true;
//}

/* *******************************************************
 * 
 *******************************************************/ 
//static ShaderProg* sgCurrentShaderProg = 0;
//void ShaderProg::Bind () 
//{
//	if (sgCurrentShaderProg == this) 
//	{
//		return;
//	}
//	//
//// Assert (GL_TRUE == glIsProgram (progID) , "invalid program");  
//	//
//	glUseProgram (progID);

	// the predefs change location IDs
//for (int i = PredefinedVariableRangeBegin; i < PredefinedVariableRangeEnd; i++) 
//{
//	sgShaderVarPredef[i].SetLocation (preDefLocation[i]); 
//}

	// enable vertex attribute
//	for (size_t i = 0; i < vertAttribs.size(); i++)
//	{
//		glEnableVertexAttribArray (vertAttribs[i]->AttribUsage ()); 
//	}
	// clear prev user vars

//for (int i = UserVarRangeBegin; i < UserVarRangeEnd; i++) 
//{
//	gShaderVariableArray[i] = 0;
//}

	// and bind current ones 
//for (size_t i = 0; i < uniformVars.size(); i++)
//{
//	ConstVarSlot slot = uniformVars[i]->VariableSlot();
//
//	if (slot < UserVarRangeBegin)
//	{
//		int basfsda = 0; 
//		basfsda++; 
//	}
//
//	gShaderVariableArray[slot] = uniformVars[i];
//}

//}
//
//void ShaderProg::Unbind () 
//{
//	sgCurrentShaderProg = 0; 
//	glUseProgram (0); 
//}


////
//void SetVertexBuffer (VertexAttributeEnum va, VertexBuffer* vb) {
//
//	BreakOnGLError_();
//
//	if (vb) {
//		glBindBuffer (GL_ARRAY_BUFFER, vb->ID);
//		glVertexAttribPointer (va, va_num_comp[va], va_gl_type[va], GL_FALSE, 0, 0);
//		}
//
//	BreakOnGLError_();
//	}

//
// CreateVertexBuffer 
//bool CreateVertexBuffer (VertexBuffer::Params& params, VertexBuffer** vb) 
//{ 
//	*vb = 0; 
//	*vb = new VertexBuffer (
//		params.attr, 
//		params.usage, 
//		params.numElems, 
//		params.source
//		); 
//	return vb ? true : false;
//}


//const int va_gl_type[] = {
//	GL_FLOAT,  GL_FLOAT, GL_FLOAT, 
//	GL_BYTE, GL_BYTE, 
//	GL_FLOAT,  GL_FLOAT, 
//	GL_BYTE,  GL_FLOAT,
//
//	GL_INT
//	};
//
//bool CreateIndexBuffer (IndexBuffer::Params& params, IndexBuffer** ib) {
//	*ib = 0; 
//	*ib = new IndexBuffer (params.indexTyp, params.usage, params.numElems, params.source); 
//	return (*ib) ? true : false; 
//	}

//
//void SetIndexBuffer (IndexBuffer& ib) {
//	//BreakAssert (ib, "SetIndexBuffer() : invalid pointer\n");
//	//
//	//sgCurrentIndexBuffer = ib;
//	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ib.ID);
//	}



//
//Texture2D::Texture2D (const Texture2D::Params& params)
//: fmt (params.format), wd (params.width), ht (params.height), ID (0)
//{
//	glGenTextures (1, &ID);
//
//	glBindTexture (GL_TEXTURE_2D, ID);
//
//	glTexImage2D (
//		GL_TEXTURE_2D, 
//		0, 
//		gl_intrnal_format[fmt], 
//		wd, ht, 0, 
//		gl_texture_format[fmt], 
//		gl_data_type[fmt],
//		params.source 
//		);
//
//	BreakOnGLError_();
//}
//
////
//Texture2D::Texture2D (Texture::Format format, unsigned width, unsigned height, const void* source) 
//	: fmt (format), wd (width), ht (height), ID (0)
//{
//	glGenTextures (1, &ID);
//
//	glBindTexture (GL_TEXTURE_2D, ID);
//
//	glTexImage2D (
//		GL_TEXTURE_2D, 
//		0, 
//		gl_intrnal_format[format], 
//		width, height, 0, 
//		gl_texture_format[format], 
//		gl_data_type[format],
//		source 
//	);
//
//	BreakOnGLError_();
//} 

////
//void BindIndexBuffer (IndexBuffer* ib) {
//	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ib ? ib->ID : 0);
//	}
//
////
//void BindVertexBuffer (VertexBuffer* vb) {
//	glBindBuffer (GL_ARRAY_BUFFER, vb->ID);
//}
//



//
//void SetTextureStage2D (unsigned stage, Texture2D* txr) {
//	//Assert (sgCurrentShaderProgram, "");
//	//Assert(stage < MAX_SAMPLER_UNITS, "sampler stage out of range");
//	//Assert(GL_TRUE == glIsTexture (txr->ID), "not a texture?");
//
//	glActiveTexture (GL_TEXTURE0 + stage);
//	glEnable (GL_TEXTURE_2D);
//	glBindTexture (GL_TEXTURE_2D, txr->ID); 
//
//	BreakOnGLError_();
//
//	}
////


// void 
// render_grid (const glm::fvec3& ctr,  float size)
// {
//    int   num_tiles   = 64; 
//    float gridstep    = size / static_cast<float> (num_tiles); 
// 
//    glm::fvec3 startPos (ctr.x - size * 0.5f, 0.0f,  ctr.y - size * 0.5f); 
// 
//    glBegin (GL_LINES); 
//    for (int iY = 0; iY <= num_tiles; iY++) 
//    {
//       glVertex3f (startPos.x        , startPos.y, startPos.z + gridstep * iY); 
//       glVertex3f (startPos.x + size , startPos.y, startPos.z + gridstep * iY); 
//    }
// 
//    for (int iX = 0; iX <= num_tiles; iX++) 
//    {
//       glVertex3f (startPos.x + gridstep * iX, startPos.y, startPos.z); 
//       glVertex3f (startPos.x + gridstep * iX, startPos.y, startPos.z + size); 
//    }
// 
//    glEnd (); 
// }

