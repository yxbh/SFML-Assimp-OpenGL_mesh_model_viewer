#include "Mesh.hpp"
#include "Texture.hpp"
#include "Skeleton.hpp"

namespace KG
{

	Mesh::Mesh(const KE::EntityID p_EntityID, const RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_spSkeleton(nullptr)
		, m_VAO(0)
		, m_VertexVBO(0), m_IndexVBO(0), m_NormalVBO(0)
		, m_TexCoordVBO(0), m_ColorVBO(0)
		, m_BoneIDVBO(0), m_BoneWeightVBO(0)
		, m_spTexture(nullptr)
		, m_LightBackFace(true), m_LoadedToGPU(false), m_Loaded(false)
		, m_RenderMode(RenderMode::Null), m_PrimitiveType(GL_TRIANGLES), m_IndexVarType(GL_UNSIGNED_INT)
		, m_FirstIndex(0), m_ElementCount(0), m_IndexOffset(0)
	{}

	Mesh::~Mesh(void)
	{
		glDeleteVertexArrays(1, &m_VAO);
		if (this->HasVertices())
			glDeleteBuffers(1, &m_VertexVBO);
		if (this->HasVertexIndices())
			glDeleteBuffers(1, &m_IndexVBO);
		if (this->HasNormals())
			glDeleteBuffers(1, &m_NormalVBO);
		if (this->HasTextureCoords())
			glDeleteBuffers(1, &m_TexCoordVBO);
		if (this->HasVertexColors())
			glDeleteBuffers(1, &m_ColorVBO);
		if (this->HasSkeleton())
		{
			glDeleteBuffers(1, &m_BoneIDVBO);
			glDeleteBuffers(1, &m_BoneWeightVBO);
		}
	}

	const GLenum Mesh::GetPrimitiveType(void) const
	{
		return m_PrimitiveType;
	}

	const GLuint Mesh::GetVAO(void) const
	{
		return m_VAO;
	}

	const GLuint Mesh::GetVertexVBO(void) const
	{
		return m_VertexVBO;
	}

	const GLuint Mesh::GetIndexVBO(void) const
	{
		return m_IndexVBO;
	}

	const KG::Vec3fVector & Mesh::GetVertices(void) const
	{
		return m_PosVertices;
	}

	const std::vector<GLuint> & Mesh::GetIndices(void) const
	{
		return m_Indices;
	}

	const KG::Vec3fVector & Mesh::GetTexCoords(void) const
	{
		return m_TexCoordVertices;
	}

	const KG::Vec4fVector & Mesh::GetColors(void) const
	{
		return m_ColorVertices;
	}

	const KG::Vec3fVector & Mesh::GetNormals(void) const
	{
		return m_NormalVertices;
	}	

	const Mesh::RenderMode Mesh::GetRenderMode(void) const
	{
		return m_RenderMode;
	}

	const GLenum Mesh::GetIndexVarType(void) const
	{
		return m_IndexVarType;
	}

	const GLuint Mesh::GetFirstIndex(void) const
	{
		return m_FirstIndex;
	}

	const GLuint Mesh::GetNumIndex(void) const
	{
		return m_Indices.size();
	}

	const GLuint Mesh::GetNumElement(void) const
	{
		return m_Indices.size();
	}

	const GLuint Mesh::GetIndexOffset(void) const
	{
		return m_IndexOffset;
	}

	const KG::Material & Mesh::GetMaterial(void) const
	{
		return m_Material;
	}

	const KG::Texture_SmartPtr Mesh::GetTexture(void) const
	{
		return m_spTexture;
	}

	const KG::Skeleton_SmartPtr Mesh::GetSkeleton(void) const
	{
		return m_spSkeleton;
	}

	const bool Mesh::HasVertices(void) const
	{
		return !m_PosVertices.empty();
	}

	const bool Mesh::HasVertexIndices(void) const
	{
		return !m_Indices.empty();
	}

	const bool Mesh::HasNormals(void) const
	{
		return !m_NormalVertices.empty();
	}

	const bool Mesh::HasVertexColors(void) const
	{
		return !m_ColorVertices.empty();
	}

	const bool Mesh::HasTextureCoords(void) const
	{
		return !m_TexCoordVertices.empty();
	}

	const bool Mesh::HasTexture(void) const
	{
		return m_spTexture != nullptr;
	}

	const bool Mesh::HasMaterial(void) const
	{
		return true;
	}

	const bool Mesh::HasSkeleton(void) const
	{
		return m_spSkeleton != nullptr;
	}

	const bool Mesh::IsBackFaceLit(void) const
	{
		return m_LightBackFace;
	}

	void Mesh::SetMaterial(const KG::Material p_NewMaterial)
	{
		m_Material = p_NewMaterial;
	}

	const bool Mesh::Loaded(void) const
	{
		if (!m_Loaded)
			return false;
		bool loaded(true);
		for (KG::SceneNode_SmartPtr scenenode_sp : m_ChildSceneNodeList)
		{
			KG::Mesh_SmartPtr mesh_sp(std::static_pointer_cast<KG::Mesh>(scenenode_sp));
			if (mesh_sp)
				if (!mesh_sp->Loaded())
					loaded = false;
		}
		return loaded;
	}

	const bool Mesh::BufferAll(void)
	{
		if (!this->Loaded())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : Nothing loaded. Ignore call. ");
			return false; // immediately abort. The entire recursive operation will stop.
		}

		bool buffer_successful(true);
		if (this->Buffered())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : already buffered everything.");
		}
		else
		{
			// gen and bind VAO.
			GLuint vao, vbo;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			this->SetVAO(vao);
			KE::Debug::check_for_GL_error();

			// buffer pos vertices.
			if (this->HasVertices())
			{
				KE::Debug::print("Mesh::BufferAll : buffering position vertices.");
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				this->SetPosVBO(vbo);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(glm::vec3)*this->GetVertices().size()
					, this->GetVertices().data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
			}
			else
				return false;

			// buffer indices
			if (this->HasVertexIndices())
			{
				KE::Debug::print("Mesh::BufferAll : buffering vertex indices.");
				this->SetRenderMode(RenderMode::Indexed);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
				this->SetIndexVBO(vbo);
				glBufferData
				(
					GL_ELEMENT_ARRAY_BUFFER
					, sizeof(GLuint)*this->GetIndices().size()
					, this->GetIndices().data()
					, GL_STATIC_DRAW
				);
				KE::Debug::check_for_GL_error();
			}
			else
				this->SetRenderMode(RenderMode::Arrays);

			// normal
			if (this->HasNormals())
			{
				KE::Debug::print("Mesh::BufferAll : buffering vertex normals.");
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				this->SetNormalVBO(vbo);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(glm::vec3)*this->GetNormals().size()
					, this->GetNormals().data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
			}

			// color vertices
			if (this->HasVertexColors())
			{
				KE::Debug::print("Mesh::BufferAll : buffering vertex colors.");
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				this->SetColorVBO(vbo);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(glm::vec4)*this->GetColors().size()
					, this->GetColors().data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
			}

			// texture coords
			if (this->HasTextureCoords())
			{
				KE::Debug::print("Mesh::BufferAll : buffering vertex coordinates.");
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				this->SetTexCoordVBO(vbo);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(glm::vec3)*this->GetTexCoords().size()
					, this->GetTexCoords().data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
			}

			// material m_BoneWeightVBO
		
			// bone IDs
			if (this->HasSkeleton())
			{
				KE::Debug::print("Mesh::BufferAll : buffering bone data.");
				// bone IDs
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				this->SetBoneIDVBO(vbo);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(m_spSkeleton->bone_IDs[0]) * m_spSkeleton->bone_IDs.size()
					, m_spSkeleton->bone_IDs.data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(4);
				glVertexAttribIPointer(4, 4, GL_INT, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
				// bone weights
				glGenBuffers(1, &m_BoneWeightVBO);
				glBindBuffer(GL_ARRAY_BUFFER, m_BoneWeightVBO);
				glBufferData
				(
					GL_ARRAY_BUFFER
					, sizeof(m_spSkeleton->bone_weights[0]) * m_spSkeleton->bone_weights.size()
					, m_spSkeleton->bone_weights.data()
					, GL_STATIC_DRAW
				);
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
				KE::Debug::check_for_GL_error();
			}

			// unbind buffers
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			KE::Debug::check_for_GL_error();

			m_LoadedToGPU = true;
			buffer_successful = true;
		}

		// buffer all children if they're Mesh objects too.
		for (KG::SceneNode_SmartPtr scenenode_sp : this->GetChildSceneNodeList())
		{
			KG::Mesh_SmartPtr mesh_sp(std::static_pointer_cast<KG::Mesh>(scenenode_sp));
			if (mesh_sp)
				if (!mesh_sp->BufferAll())
					buffer_successful = false;
		}

		return buffer_successful;
	}

	const bool Mesh::Buffered(void) const
	{
		return m_LoadedToGPU;
	}

	void Mesh::Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		this->KG::SceneNode::Update(p_rScene, p_Elapsed);
		
		if (this->HasSkeleton())
			m_spSkeleton->ComputePose(p_Elapsed);

	}

	void Mesh::SetVAO(const GLuint p_VAO)
	{
		m_VAO = p_VAO;
	}

	void Mesh::SetPosVBO(const GLuint p_VBO)
	{
		m_VertexVBO = p_VBO;
	}

	void Mesh::SetIndexVBO(const GLuint p_VBO)
	{
		m_IndexVBO = p_VBO;
	}

	void Mesh::SetNormalVBO(const GLuint p_VBO)
	{
		m_NormalVBO = p_VBO;
	}

	void Mesh::SetTexCoordVBO(const GLuint p_VBO)
	{
		m_TexCoordVBO = p_VBO;
	}
	
	void Mesh::SetColorVBO(const GLuint p_VBO)
	{
		m_ColorVBO = p_VBO;
	}

	void Mesh::SetBoneIDVBO(const GLuint p_VBO)
	{
		m_BoneIDVBO = p_VBO;
	}

	void Mesh::SetRenderMode(const RenderMode p_RenderMode)
	{
		m_RenderMode = p_RenderMode;
	}

	void Mesh::SetPrimitiveType(const KG::PrimitiveType p_PrimitiveType)
	{
		switch (p_PrimitiveType)
		{
		case KG::PrimitiveType::Points:
			m_PrimitiveType = GL_POINTS;
			break;
		case KG::PrimitiveType::Lines:
			m_PrimitiveType = GL_LINES;
			break;
		case KG::PrimitiveType::LineStrip:
			m_PrimitiveType = GL_LINE_STRIP;
			break;
		case KG::PrimitiveType::Triangles:
			m_PrimitiveType = GL_TRIANGLES;
			break;
		case KG::PrimitiveType::TriangleFan:
			m_PrimitiveType = GL_TRIANGLE_FAN;
			break;
		case KG::PrimitiveType::TriangleStrip:
			m_PrimitiveType = GL_TRIANGLE_STRIP;
			break;
		};
	}

	void Mesh::SetIndexType(const GLenum p_IndexType)
	{
		m_IndexVarType = p_IndexType;
	}

	void Mesh::SetFirstIndex(const GLuint p_FirstIndex)
	{
		m_FirstIndex = p_FirstIndex;
	}

	void Mesh::SetIndexOffset(const GLuint p_IndexOffset)
	{
		m_IndexOffset = p_IndexOffset;
	}

	void Mesh::SetLightBackFace(const bool p_LightBackFace)
	{
		m_LightBackFace = p_LightBackFace;
	}

	void Mesh::SetTexture(KG::Texture_SmartPtr p_spTexture)
	{
		if (p_spTexture == nullptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : applying null Texture.");
			return;
		}
		else if (!p_spTexture->IsValid())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : invalid Texture.");
			return;
		}
		m_spTexture = p_spTexture;
	}

	void Mesh::SetSkeleton(KG::Skeleton_SmartPtr p_spSkeleton)
	{
		m_spSkeleton = p_spSkeleton;
	}

} // KG ns
