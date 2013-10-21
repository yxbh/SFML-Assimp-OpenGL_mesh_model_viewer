#include "Mesh.hpp"
#include "Texture.hpp"

namespace KG
{

	Mesh::Mesh(const KE::EntityID p_EntityID, const RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_HasPosVertices(false), m_HasFaces(false), m_HasNormals(false), m_HasTexCoords(false), m_HasColors(false)
		, m_VAO(0), m_VertexVBO(0), m_IndexVBO(0), m_NormalVBO(0), m_TexCoordVBO(0), m_ColorVBO(0), m_HasMaterial(false)
		, m_spTexture(nullptr), m_HasTexture(false)
		, m_LightBackFace(true), m_LoadedToGPU(false), m_Loaded(false)
		, m_RenderMode(RenderMode::Null), m_PrimitiveType(GL_TRIANGLES), m_IndexVarType(GL_UNSIGNED_INT)
		, m_FirstIndex(0), m_IndexCount(0), m_ElementCount(0), m_IndexOffset(0)
	{}

	Mesh::~Mesh(void)
	{
		glDeleteVertexArrays(1, &m_VAO);
		if (this->Has(Property::PosVertices))
			glDeleteBuffers(1, &m_VertexVBO);
		if (this->Has(Property::VertexIndices))
			glDeleteBuffers(1, &m_IndexVBO);
		if (this->Has(Property::Normals))
			glDeleteBuffers(1, &m_NormalVBO);
		if (this->Has(Property::TexCoords))
			glDeleteBuffers(1, &m_TexCoordVBO);
		if (this->Has(Property::Colors))
			glDeleteBuffers(1, &m_ColorVBO);
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
		return m_IndexCount;
	}

	const GLuint Mesh::GetNumElement(void) const
	{
		return m_ElementCount;
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

	const bool Mesh::Has(const Property p_Property) const
	{
		switch (p_Property)
		{
		case Property::PosVertices:		return m_HasPosVertices;
		case Property::Faces:			return m_HasFaces;
		case Property::VertexIndices:	return m_HasFaces;
		case Property::Normals:			return m_HasNormals;
		case Property::Textures:		return m_HasTexture;
		case Property::TexCoords:		return m_HasTexCoords;
		case Property::Colors:			return m_HasTexCoords;
		case Property::Material:		return m_HasMaterial;
		case Property::LightBackFace:	return m_LightBackFace;
		default:						return false;
		}
	}

	const bool Mesh::HasVertex(void) const
	{
		return m_HasPosVertices;
	}

	const bool Mesh::HasNormal(void) const
	{
		return m_HasNormals;
	}

	const bool Mesh::HasColor(void) const
	{
		return m_HasColors;
	}

	const bool Mesh::HasTexture(void) const
	{
		return m_HasTexture;
	}

	void Mesh::SetHasVertex(const bool p_Have)
	{
		m_HasPosVertices = p_Have;
	}

	void Mesh::SetHasNormal(const bool p_Have)
	{
		m_HasNormals = p_Have;
	}

	void Mesh::SetHasColor(const bool p_Have)
	{
		m_HasColors = p_Have;
	}

	void Mesh::SetHasTexture(const bool p_Have)
	{
		m_HasTexture = p_Have;
	}

	void Mesh::SetMaterial(const KG::Material p_NewMaterial)
	{
		m_Material = p_NewMaterial;
	}

	const GLuint Mesh::GetHandle(const Detail p_Detail) const
	{
		switch (p_Detail)
		{
		case Detail::VAO:			return m_VAO;
		case Detail::VertexVBO:		return m_VertexVBO;
		case Detail::IndexVBO:		return m_IndexVBO;
		case Detail::NormalVBO:		return m_NormalVBO;
		case Detail::TexCoordVBO:	return m_TexCoordVBO;
		case Detail::ColorVBO:		return m_ColorVBO;
		default:					return GLuint(0);
		}
	}

	void Mesh::Set(const Detail p_Detail, const GLuint p_Value)
	{
		switch (p_Detail)
		{
		case Detail::VAO:			m_VAO = p_Value;
		case Detail::VertexVBO:		m_VertexVBO = p_Value;
		case Detail::IndexVBO:		m_IndexVBO = p_Value;
		case Detail::NormalVBO:		m_NormalVBO = p_Value;
		case Detail::TexCoordVBO:	m_TexCoordVBO = p_Value;
		case Detail::ColorVBO:		m_ColorVBO = p_Value;
		default:					assert(false);
		}
	}

	const bool Mesh::Loaded(void) const
	{
		return m_Loaded;
	}

	const bool Mesh::BufferAll(void)
	{
		if (this->Buffered())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : already buffered everything.");
			return false;
		}

		// gen and bind VAO.
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		this->SetVAO(vao);
		KE::Debug::check_for_GL_error();

		// buffer pos vertices.
		if (this->m_HasPosVertices)
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			this->SetPosVBO(vbo);
			glBufferData
				(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->GetVertices().size(), this->GetVertices().data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
			KE::Debug::check_for_GL_error();
		}
		else
			return false;

		// buffer indices
		if (this->m_HasFaces)
		{
			this->SetRenderMode(RenderMode::Indexed);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
			this->SetIndexVBO(vbo);
			glBufferData
				(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->GetIndices().size(), this->GetIndices().data(), GL_STATIC_DRAW);
			KE::Debug::check_for_GL_error();
		}
		else
			this->SetRenderMode(RenderMode::Arrays);

		// normal
		if (this->m_HasNormals)
		{
			KE::Debug::print("Mesh : Normal buffering.");
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			this->SetNormalVBO(vbo);
			glBufferData
				(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->GetNormals().size(), this->GetNormals().data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
			KE::Debug::check_for_GL_error();
		}

		// color vertices
		if (this->m_HasColors)
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			this->SetColorVBO(vbo);
			glBufferData
				(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->GetColors().size(), this->GetColors().data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
			KE::Debug::check_for_GL_error();
		}

		// texture coords
		if (this->m_HasTexCoords)
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			this->SetTexCoordVBO(vbo);
			glBufferData
				(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->GetTexCoords().size(), this->GetTexCoords().data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
			KE::Debug::check_for_GL_error();
		}

		// material
		

		// TODO other bufferings.

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		KE::Debug::check_for_GL_error();

		return m_LoadedToGPU = true;
	}

	const bool Mesh::Buffered(void) const
	{
		return m_LoadedToGPU;
	}

	void Mesh::Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		this->KG::SceneNode::Update(p_rScene, p_Elapsed);
		static KE::Duration elapsed; elapsed += p_Elapsed;
		//this->
		//	//.SetScale(0.05, 0.05, 0.05)
		//	SetRotation
		//	(
		//		0//elapsed.AsSeconds() * 50
		//		, elapsed.AsSeconds() * 50
		//		, 0//elapsed.AsSeconds() * 100
		//	);
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

	void Mesh::SetRenderMode(const RenderMode p_RenderMode)
	{
		m_RenderMode = p_RenderMode;
	}

	void Mesh::SetPrimitiveType(const GLenum p_PrimitiveType)
	{
		m_PrimitiveType = p_PrimitiveType;
	}

	void Mesh::SetIndexType(const GLenum p_IndexType)
	{
		m_IndexVarType = p_IndexType;
	}

	void Mesh::SetFirstIndex(const GLuint p_FirstIndex)
	{
		m_FirstIndex = p_FirstIndex;
	}

	void Mesh::SetNumIndex(const GLuint p_IndexCount)
	{
		m_IndexCount = p_IndexCount;
	}

	void Mesh::SetNumElement(const GLuint p_ElementCount)
	{
		m_ElementCount = p_ElementCount;
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
			m_HasTexture = false;
			return;
		}
		else if (!p_spTexture->IsValid())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Mesh : invalid Texture.");
			m_HasTexture = false;
			return;
		}
		m_HasTexture = true;
		m_spTexture = p_spTexture;
	}

} // KG ns
