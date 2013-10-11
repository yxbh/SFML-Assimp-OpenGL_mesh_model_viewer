#include "Rectangle2D.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"

namespace object
{
	Rectangle2D::Rectangle2D(void)
		: KG::Mesh(KE::EntityIDGenerator::NewID(), KG::RenderPass::Dynamic)
		, m_TransformMatrix(1.0f)
	{
		const GLfloat vertex_data[6][3] = 
		{
			{-0.5f, -0.5f, 0.0f},
			{-0.5f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f},
			{0.5f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f},
			{-0.5f, 0.5f, 0.0f},
		};
		unsigned vao, vbo;
		unsigned buffer_size = sizeof vertex_data;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, &vertex_data, GL_STATIC_DRAW);

		// setup properties
		this->SetVAO(vao);
		this->SetRenderMode(KG::Mesh::RenderMode::Arrays);
		this->SetPrimitiveType(GL_TRIANGLES);
		this->SetFirstIndex(0); this->SetNumIndex(6);

		// setup shader program
		KG::ShaderResourceList shader_resource_list;
		shader_resource_list.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		shader_resource_list.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr shader_program(new KG::ShaderProgram());
		shader_program->SetGLHandle(KG::ShaderProgramFactory::MakeFromFiles(shader_resource_list));
		this->SetShaderProgram(shader_program);

		GLuint location = shader_program->Enable().GetAttributeLocation("vPosition");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	Rectangle2D::~Rectangle2D(void)
	{}


	void Rectangle2D::Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_rScene);
		static KE::Duration s_elapsed_time;
		s_elapsed_time += p_ElapsedTime;
		auto shader_program = this->GetShaderProgram();
	}
	

} // object ns
