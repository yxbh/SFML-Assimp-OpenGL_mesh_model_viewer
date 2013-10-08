#include "Ground.hpp"

namespace CITS
{
namespace Ground
{

	Mesh::Mesh(void)
	{
		// setup shader program
		KG::ShaderResourceList shader_resource_list;
		shader_resource_list.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		shader_resource_list.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr shader_program(new KG::ShaderProgram());
		shader_program->SetID(KG::ShaderProgramFactory::MakeFromFiles(shader_resource_list));
		this->SetShaderProgram(shader_program);

		const GLfloat vertex_data[6][3] =
		{
			{0.0f, -2.0f, 0.0f},
			{25.0f, -2.0f, 25.f},
			{25.0f, -2.0f, -25.f},
			{-25.0f, -2.0f, -25.f},
			{-25.0f, -2.0f, 25.f},
			{25.0f, -2.0f, 25.f},
		};

		const GLfloat color_data[6][4] =
		{
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
		};

		GLuint vao, vbo, vbo2;
		unsigned buffer_size = sizeof vertex_data;
		unsigned color_buffer_size = sizeof color_data;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, &vertex_data, GL_STATIC_DRAW);

		GLuint location = shader_program->Enable().GetAttribLocation("vPosition");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &vbo2);
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, color_buffer_size, &color_data, GL_STATIC_DRAW);

		GLuint location2 = shader_program->Enable().GetAttribLocation("vColor");
		shader_program->SetParameter<GLboolean>("HasColorVertices", GL_TRUE);
		glEnableVertexAttribArray(location2);
		glVertexAttribPointer(location2, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		// setup properties
		this->SetRenderPass(KG::RenderPass::Static);
		this->SetVAO(vao);
		this->SetColorVBO(vbo2);
		this->SetPosVBO(vbo);
		this->SetRenderMode(Mesh::RenderMode::Arrays);
		this->SetPrimitiveType(GL_TRIANGLE_FAN);
		this->SetFirstIndex(0);
		this->SetNumIndex(6);
		this->SetLightBackFace(true);
	}

	Mesh::~Mesh(void)
	{
	}

	void Mesh::Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		//KE::Debug::print("hello");
		KE_UNREFERENCED_PARAMETER(p_rScene); KE_UNREFERENCED_PARAMETER(p_Elapsed);
	}

} // Ground ns
} // CITS ns
