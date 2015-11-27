#ifndef _Render_
#define _Render_

#include "VecMath.h"

namespace Rn 
{
	class Renderer; 

   union UniformValue { void* p; int i; };
   typedef std::map<std::string, int>           AttributeMap;
   typedef std::map<std::string, int>           UniformMap;
   typedef std::map<std::string, UniformValue>  UniformValueMap;
   typedef std::map<std::string, unsigned int>  ShaderTable;



   union vecptru {
      float*  f;
      double* d;

      Ma::Vec2f*  f2;
      Ma::Vec2d*  d2;

      Ma::Vec3f*  f3;
      Ma::Vec3d*  d3;

      Ma::Vec4f*  f4;
      Ma::Vec4d*  d4;

      Ma::Vec4i*  i4;
      Ma::Vec4ui* ui4;

      Ma::Vec4s*  s4;
      Ma::Vec4us* us4;

   };
   struct Shader_constant {};

	//
	// ProjectionState - load perspective GL projection 
	void ProjectionState (
		float fFoV, 
		float fAsp, 
		float fNear, 
		float fFar
		);
	
	//
	// ModelViewMatrix - build model view matrix
	Ma::Mat44f& ModelViewMatrix (
		Ma::Mat44f& out, 
		const Ma::Vec3f& mod_Pos, 
		const Ma::Quatf& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		); 

   //
   // ModelViewMatrix - build model view matrix
	Ma::Mat44f& ModelViewMatrix (
		Ma::Mat44f& out, 
		const Ma::Vec3f& mod_Pos, 
		const Ma::Mat33f& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		); 

	//
	// ModelViewState - load model GL model view matrix		
	void ModelViewState (
		const Ma::Vec3f& mod_Pos, 
		const Ma::Quatf& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		);

	//
	// ModelViewState - load model GL model view matrix		
	void ModelViewState (
		const Ma::Vec3f& mod_Pos, 
		const Ma::Mat33f& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		);

   //
   void Draw_AA_grid ( 
	   float		size, 
	   Ma::Quatf&	view_Rot, 
	   Ma::Vec3f&	view_Pos);


   void Draw_axes (
	   float size, 
      Ma::Matrix<4, 4, float>& mv);


   void Draw_line (
	   float             size, 
      const Ma::Vec3f&  col, 
      const Ma::Vec3f&  a, 
      const Ma::Vec3f&  b, 
	   Ma::Quatf&        view_Rot, 
      Ma::Vec3f&        view_Pos);


   void Draw_sphere (
      float size, 
      Ma::Vec3f& col3f, 
      Ma::Vec3f& model_Pos, 
	   Ma::Quatf& view_Rot, 
      Ma::Vec3f& view_Pos);


   void Draw_axes (
	   float size, 
	   Ma::Quatf& model_Rot, Ma::Vec3f& model_Pos, 
	   Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos
	   ); 

   Renderer*   Create_renderer (); 
   void        Destroy_renderer (Renderer*); 



	typedef std::list<std::string>		StringList; 
	typedef std::vector<std::string>	StringArray; 



	// abstract GL enums
	enum DataType { _Invalid_DataType = -1, 
		DT_Byte, 
		DT_UByte, 
		DT_Short, 
		DT_UShort, 
		DT_Int, 
		DT_UInt, 
		DT_Float, 
		DT_Double 
		}; 
		
	// predefine vertex input
	//enum VertexAttribute {
	//	VA_Position = 0, 
	//	VA_Normal, 
	//	VA_Tangent,
	//	VA_Tex0Coord, 
	//	VA_Tex1Coord, 
	//	VA_BoneIndex, 
	//	VA_BoneWeight, 
	//	//
	//	_VertexAttributeMaxCount_
	//	};

	//
	enum BufferUsage {
		BU_StreamDraw, 
		BU_StreamRead, 
		BU_StreamCopy, 
		BU_StaticDraw, 
		BU_StaticRead, 
		BU_StaticCopy,
		BU_DynamicDraw, 
		BU_DynamicRead, 
		BU_DynamicCopy
		};

	enum BufferAccess {
		BA_Read, 
		BA_Write, 
		BA_ReadWrite, 
		};

      //
//	struct IndexBuffer {
//		//struct Params {
//		//	Type		indexTyp;
//		//	BufferUsage usage;
//		//	size_t		sizeOf_Buff;
//		//	void*		source;
//		//	};
//		IndexBuffer (
//			DataType	indexTyp,
//			BufferUsage usage,
//			size_t		sizeOf_Buff,
//			void*		source
//			); 
//
//		~IndexBuffer (); 
//
//		unsigned GetID () const { 
//			return ID;
//			}
//
//		bool Lock (BufferAccess access, void** memPtr); 
//		bool Unlock (); 
//
//		unsigned ID; // GLuint ID;
//		// unsigned type; // GLenum type;
//		};
//
//
//	//
//	struct VertexBuffer {
//
//		//struct Params {
//		//	BufferUsage usage; 
//		//	size_t sizeOf_Buff;
//		//	void* source;	
//		//	};
//
//		inline unsigned GetID () const { return ID; } 
//
//		VertexBuffer (
//			BufferUsage	usage,
//			size_t		sizeOf_Buff,
//			void*		source
//			); 
//
//		~VertexBuffer (); 
//
//		bool Lock	(BufferAccess access, void** memPtr);
//		bool Unlock ();
//		//
//		unsigned	ID;		// GLuint ID;
//	};



	void Load_uniform_1ui	(unsigned loc, const unsigned* data); 
	void Load_uniform_2ui	(unsigned loc, const unsigned* data); 
	void Load_uniform_3ui	(unsigned loc, const unsigned* data); 
	void Load_uniform_4ui	(unsigned loc, const unsigned* data); 

	void Load_uniform_1i	(unsigned loc, const int* data); 
	void Load_uniform_2i	(unsigned loc, const int* data); 
	void Load_uniform_3i	(unsigned loc, const int* data); 
	void Load_uniform_4i	(unsigned loc, const int* data); 

	void Load_uniform_1f	(unsigned loc, const float* data); 
	void Load_uniform_2f	(unsigned loc, const float* data); 
	void Load_uniform_3f	(unsigned loc, const float* data); 
	void Load_uniform_4f	(unsigned loc, const float* data); 

	void Load_uniform_3x3f	(unsigned loc, bool transp, const float* data); 	
	void Load_uniform_4x4f	(unsigned loc, bool transp, const float* data); 


	//enum TextureBlend {
	//	TB_Add = 0, 
	//	TB_Mul, 
	//	TB_Mix, 
	//	TB_Sub, 
	//	};



	// Texture 
	struct Texture 
      { 
		enum Format {
			TF_UC = 0, 
			TF_2UC, 
			TF_3UC, 
			TF_4UC, // ... so far

			TF_1F, // ... so far
			TF_2F, // ... so far
			TF_3F, // ... so far
			TF_4F, // ... so far
			};

		Texture (); 
		~Texture (); 

		unsigned GetID () const { return ID; } 
		unsigned ID; //GLuint ID;
		};

	//Specify_1D_image 
	//Specify_3D_image 
	//Specify_Cube_image 

	//
	Texture& Load_2D_image (
		Texture&		txr, 
		int				lvl, 
		int				wd, 
		int				ht, 
		int				txr_Border, 
		Texture::Format int_Fmt, 
		Texture::Format src_Fmt, 
		const void* src
		); 

	//
	void Bind_texture2D_unit (
		Texture&	txr, 
		unsigned	iUnit, 
		bool		enable = true
		); 


	//
	//
	struct Sampler {

		enum Filter {
			SF_Nearest = 0, 
			SF_Linear, 
			SF_NearestMipmapNearest, 
			SF_LinearMipmapNearest, 
			SF_NearestMipmapLinear, 
			SF_LinearMipmapLinear, 
			};

		enum Wrap {
			SW_Clamp = 0,
			SW_Repeat,
			SW_MirroredRepeat,
			};

		Sampler (); 
		~Sampler (); 

		unsigned GetID () const { return ID; } 
		unsigned ID; //GLuint ID;
		}; 

	void Bind_sampler_unit (unsigned iUnit, Sampler& samp); 


	//
	void SetSamplerState2D (
		unsigned		stage,
		Sampler::Filter	minFilt,
		Sampler::Filter	magFilt, 
		Sampler::Wrap	uWrap,
		Sampler::Wrap	vWrap
		);


	// 
	struct Shader {

		typedef std::vector<std::string> Sources;

		enum Type { 
			INVALID_SHADER_TYPE = 0, 
			FragmentShader, 
			VertexShader,  
			};

		// FU@%TIONS
		Shader (Type type);
		~Shader ();
		//
		bool Compile (const Sources& filenames);
		//
		inline int GetID () { return shaderID; } 

		// VARS
		unsigned	shaderID;							/*[ gl generated ]*/ 
		Type		shaderType;						/*[ vert or frag ]*/ 
		}; 
	
	Shader::Type Get_shader_type (const Shader& sh); 

	//
	struct ShaderProg {


		ShaderProg ();

		bool Build (); 
		//bool Compile (); 
		//bool Link ();

		void Bind (); 
		void Unbind (); 

		// int preDefLocation[PredefinedVariableRangeEnd - PredefinedVariableRangeBegin]; 
		//
		unsigned attribFlags; 

		unsigned	program_ID;
		Shader		vert_Shader;
		Shader		frag_Shader;


		//int attribute_Binding[_VertexAttributeMaxCount_];

		AttributeMap	attrib_Map;
		UniformMap		unif_Map;
		//std::map<std::string, int> attribute_Map;
		//std::map<std::string, int> uniform_Map;

		//std::vector<ShaderConstValue*> uniformVars;
		//std::vector<VertexAttribDef*> vertAttribs;  
		};

	bool Build_shader_program (
		ShaderProg& prog, 
		const Shader::Sources& vertSources, 
		const Shader::Sources& fragSources
		); 

	bool Bind_shader_program  (
		ShaderProg& prog 
		);

	void Unbind_shader_program (
		); 

	//
	AttributeMap& Get_attribute_locations (
		AttributeMap&, 
		const ShaderProg& prog, 
		const StringArray& vars
		); 


	//
	UniformMap& Get_uniform_locations (
		UniformMap&,	
		const ShaderProg& prog,  
		const StringArray& vars
		); 

	//
//	void Bind_vertex_source (
//		unsigned			loc, 
//		DataType			dataType, 
//		size_t				numComps, 
//		const VertexBuffer&	vb,
//		bool				enable = true
//		); 

	//
//	void Bind_vertex_source (
//		VertexAttribute		va, 
//		DataType			dt, 
//		size_t				num_Comps, 
//		const VertexBuffer&	vb, 
//		const ShaderProg&	prog
//		); 

	//
//	void Bind_index_source (
//		const IndexBuffer& ib
//		);  
//
//	void Unbind_index_source (
//		);  

	//
	void BreakOnGLError_ (); 



} // namespace 
#endif