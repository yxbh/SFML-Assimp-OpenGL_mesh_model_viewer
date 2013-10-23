#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"
#include "../SceneNode.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Light/Material.hpp"

namespace KG
{	
	class Texture;
	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;
	typedef std::weak_ptr<KG::Texture>		Texture_WeakPtr;

	/*! \class Mesh
	*/
	class Mesh
		: public KG::SceneNode
	{
		friend class MeshLoader;

	public:
		enum class RenderMode
		{
			Null,
			Arrays,
			Indexed
		};

		enum class Property
		{
			PosVertices,
			Faces,
			VertexIndices,
			Normals,
			Textures,
			TexCoords,
			Colors,
			Material,
			LightBackFace,
		};

		enum class Detail
		{
			VAO,
			VertexVBO,
			IndexVBO,
			NormalVBO,
			TexCoordVBO,
			ColorVBO,
		};

	private:
		KG::Vec3fVector			m_PosVertices;
		KG::Vec1uVector			m_Indices;
		KG::Vec3fVector			m_NormalVertices;
		KG::Vec3fVector			m_TexCoordVertices;
		KG::Vec4fVector			m_ColorVertices;
		KG::Material			m_Material;
		KG::Vec4iVector			m_BoneIDs;
		KG::Vec4fVector			m_BoneWeights;
		/* mesh properties. */
		bool					m_HasPosVertices;
		bool					m_HasFaces;			// vertex indices.
		bool					m_HasNormals;
		bool					m_HasTexCoords;
		bool					m_HasColors;		// color vertices
		bool					m_HasMaterial;
		bool					m_HasBones;
		unsigned				m_NumBones;
		//bool					m_HasAnimation;
		bool					m_LightBackFace;	// light with only ambient if false.
		bool					m_LoadedToGPU;		// loaded into GPU memory.
		bool					m_Loaded;			// loaded into RAM.
		/* render specifics. VAO and VBO's */
		GLuint					m_VAO;
		GLuint					m_VertexVBO;		// vertex data buffer
		GLuint					m_IndexVBO;			// index data buffer
		GLuint					m_NormalVBO;
		GLuint					m_TexCoordVBO;
		GLuint					m_ColorVBO;
		GLuint					m_BoneIDVBO;
		/* texture related. */
		KG::Texture_SmartPtr	m_spTexture;
		bool					m_HasTexture;
		/* OpenGL draw call related. */
		RenderMode				m_RenderMode;			// glDrawArrays or glDrawElements
		GLenum					m_PrimitiveType;		// e.g. GL_TRIANGLES etc
		GLenum					m_IndexVarType;			// for glDrawElements
		GLuint					m_FirstIndex;
		GLuint					m_IndexCount;			// 
		GLuint					m_ElementCount;			// number of 'indexes' to draw
		GLuint					m_IndexOffset;			// offset in bytes
		// e.g. glDrawArrays(m_PrimitiveType, m_FirstIndex, m_IndexCount)
		// e.g. glDrawElements(m_PrimitiveType, m_ElementCount, m_IndexVarType, m_IndexOffset)

	public:
		Mesh(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::Static);
		virtual ~Mesh(void);

		const GLenum GetPrimitiveType(void) const;
		const GLuint GetVAO(void) const;
		const GLuint GetVertexVBO(void) const;
		const GLuint GetIndexVBO(void) const;
		const KG::Vec3fVector & GetVertices(void) const;
		const std::vector<GLuint> & GetIndices(void) const;
		const KG::Vec3fVector & GetTexCoords(void) const;
		const KG::Vec4fVector & GetColors(void) const;
		const KG::Vec3fVector & GetNormals(void) const;
		const Mesh::RenderMode GetRenderMode(void) const;
		const GLenum GetIndexVarType(void) const;
		const GLuint GetFirstIndex(void) const;
		const GLuint GetNumIndex(void) const;
		const GLuint GetNumElement(void) const;
		const GLuint GetIndexOffset(void) const;
		const KG::Material & GetMaterial(void) const;
		const KG::Texture_SmartPtr GetTexture(void) const;

		const bool Has(const Property p_Property) const;
		const bool HasVertex(void) const;
		const bool HasNormal(void) const;
		const bool HasColor(void) const;
		const bool HasTexture(void) const;
		const bool HasBones(void) const;
		void SetHasVertex(const bool p_Have);
		void SetHasNormal(const bool p_Have);
		void SetHasColor(const bool p_Have);
		void SetHasTexture(const bool p_Have);
		void SetMaterial(const KG::Material p_NewMaterial);

		/*! return the requested internal handle. */
		const GLuint GetHandle(const Detail p_Detail) const;
		void Set(const Detail p_Detail, const GLuint p_Value);

		/*! Check and see if this Mesh is empty. */
		const bool Loaded(void) const;
		/*! Buffer everything to GPU and set ID's for all VAO and VBO's. */
		virtual const bool BufferAll(void);
		/*! Check to see if mesh is already buffered to the GPU memory. */
		const bool Buffered(void) const;

		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed) override;

	//protected:
		void SetVAO(const GLuint p_VAO);
		void SetPosVBO(const GLuint p_VBO);
		void SetIndexVBO(const GLuint p_VBO);
		void SetNormalVBO(const GLuint p_VBO);
		void SetTexCoordVBO(const GLuint p_VBO);
		void SetColorVBO(const GLuint p_VBO);
		void SetBoneIDVBO(const GLuint p_VBO);
		void SetRenderMode(const RenderMode p_RenderMode);
		void SetPrimitiveType(const GLenum p_PrimitiveType);
		void SetIndexType(const GLenum p_IndexType);
		void SetFirstIndex(const GLuint p_FirstIndex);
		void SetNumIndex(const GLuint p_IndexCount);
		void SetNumElement(const GLuint p_ElementCount);
		void SetIndexOffset(const GLuint p_IndexOffset);
		void SetLightBackFace(const bool p_LightBackFace);
		virtual void SetTexture(KG::Texture_SmartPtr p_spTexture);
	};

	typedef std::shared_ptr<KG::Mesh> Mesh_SmartPtr;
	typedef std::weak_ptr<KG::Mesh> Mesh_WeakPtr;
	typedef std::vector<KG::Mesh_SmartPtr>	Mesh_SP_List;
} // KG ns
