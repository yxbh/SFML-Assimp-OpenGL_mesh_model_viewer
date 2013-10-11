#include "Triangle2D2.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"
#include "../../Engine/Graphics/OpenGL/Asset/Mesh.hpp"

namespace object
{
	Triangle2D::Triangle2D(void)
		: Mesh()
	{
		KG::ShaderResourceList shader_resource_list;
		shader_resource_list.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		shader_resource_list.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr new_shaderprogram(new KG::ShaderProgram());
		new_shaderprogram->SetGLHandle(KG::ShaderProgramFactory::MakeFromFiles(shader_resource_list));
		this->SetShaderProgram(new_shaderprogram);

		this->SetRenderPass(KG::RenderPass::Dynamic);
		this->SetRenderMode(KG::Mesh::RenderMode::Indexed);
		this->SetIndexOffset(0);
		this->SetNumElement(3);
		this->SetFirstIndex(0);
		this->SetIndexType(GL_UNSIGNED_INT);
		this->SetPrimitiveType(GL_TRIANGLES);

		KG::Vec3fVector vertices;
		vertices.push_back(KG::Vec3f(-0.4f, -0.4f, 0.0f));
		vertices.push_back(KG::Vec3f(0.4f, -0.4f, 0.0f));
		vertices.push_back(KG::Vec3f(0.0f, 0.5f, 0.0f));

		std::vector<GLuint> indices;
		indices.push_back(0);	indices.push_back(1);	indices.push_back(2);
		
		// gen and bind VAO.
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		this->SetVAO(vao);
		KE::Debug::check_for_GL_error();

		// buffer pos vertices.
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		this->SetPosVBO(vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(KG::Vec3f)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
		KE::Debug::check_for_GL_error();

		// buffer indices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		this->SetIndexVBO(vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
		KE::Debug::check_for_GL_error();

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		KE::Debug::check_for_GL_error();
	}

	Triangle2D::~Triangle2D(void)
	{}

	void Triangle2D::Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_rScene);
		static KE::Duration elapsed; elapsed += p_ElapsedTime;
		this->SetOrientation
		(
			elapsed.AsSeconds() * 200
			,elapsed.AsSeconds() * 200
			, elapsed.AsSeconds() * 200
		)
		.SetPosition
		(
			sin(elapsed.AsSeconds() * KE::Math::DegreeToRadian(100))
			, cos(elapsed.AsSeconds() * KE::Math::DegreeToRadian(100))
			, 0.0
		);
	}
} // object ns
