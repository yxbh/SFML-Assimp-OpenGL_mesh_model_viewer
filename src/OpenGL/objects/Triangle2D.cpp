#include "Triangle2D.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"

namespace object
{
	Triangle::Triangle(void)
		: Mesh()
	{
		KG::ShaderResourceList shader_resource_list;
		shader_resource_list.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		shader_resource_list.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr new_shaderprogram(new KG::ShaderProgram());
		new_shaderprogram->SetGLHandle(KG::ShaderProgramFactory::MakeFromFiles(shader_resource_list));
		this->SetShaderProgram(new_shaderprogram);

		this->SetRenderPass(KG::RenderPass::Dynamic);

		this->SetRenderMode(KG::Mesh::RenderMode::Arrays);
		this->SetPrimitiveType(GL_TRIANGLES);
		this->SetFirstIndex(0); this->SetNumIndex(3);

		GLfloat vertices[3][3] =
		{
			// triangle
			{-0.4f, -0.4f, 0.0f},
			{0.4f, -0.4f, 0.0f},
			{0.0f, 0.5f, 0.0f},
		};

		GLfloat colors[3][4] = 
		{
			// colour
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f}
		};
		
		GLuint vertex_array_id, vertex_buffer, color_buffer;
		glGenVertexArrays(1, &vertex_array_id); this->SetVAO(vertex_array_id);
		glBindVertexArray(vertex_array_id);

		glGenBuffers(1, &vertex_buffer); this->SetPosVBO(vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		GLuint location = new_shaderprogram->GetAttributeLocation("vPosition");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glGenBuffers(1, &color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);
		location = new_shaderprogram->GetAttributeLocation("vColor");
		new_shaderprogram->Enable().SetParameter<GLboolean>("HasColorVertices", GL_TRUE);
		glEnableVertexAttribArray(location); 
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindVertexArray(0);
	}

	Triangle::~Triangle(void)
	{}

	void Triangle::Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_rScene);
		static KE::Duration elapsed; elapsed += p_ElapsedTime;
		this->SetOrientation
		(
			elapsed.AsSeconds() * 150
			, elapsed.AsSeconds() * 160
			, elapsed.AsSeconds() * 170
		)
		.SetPosition
		(
			sin(elapsed.AsSeconds() * KE::Math::DegreeToRadian(90))
			, cos(elapsed.AsSeconds() * KE::Math::DegreeToRadian(90))
			, 0.0
		);
	}

} // object ns
