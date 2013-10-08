#include "Cube3D.hpp"

namespace object
{
	Cube3D::Cube3D(void)
		: KG::Mesh(KE::EntityIDGenerator::NewID(), KG::RenderPass::Dynamic)
	{
		// setup shader
		KG::ShaderResourceList new_shader_rlist;
		new_shader_rlist.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		new_shader_rlist.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr new_shader_program(new KG::ShaderProgram());
		new_shader_program->SetID(KG::ShaderProgramFactory::MakeFromFiles(new_shader_rlist));
		this->SetShaderProgram(new_shader_program);

		const float vertexData[] = {
			// vertices
			0.25f,  0.25f, 0.25f,		0.25f, -0.25f, 0.25f,		-0.25f,	0.25f, 0.25f,
			0.25f, -0.25f, 0.25f,		-0.25f, -0.25f, 0.25f,		-0.25f,  0.25f, 0.25f,
			0.25f,  0.25f, -0.25f,		-0.25f,  0.25f, -0.25f,		 0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,		-0.25f,  0.25f, -0.25f,		-0.25f, -0.25f, -0.25f,
			-0.25f,  0.25f, 0.25f,		-0.25f, -0.25f,  0.25f, 	-0.25f, -0.25f, -0.25f,
			-0.25f,  0.25f, 0.25f,		-0.25f, -0.25f, -0.25f,		-0.25f,  0.25f, -0.25f,
			0.25f,  0.25f,  0.25f,		0.25f, -0.25f, -0.25f,		 0.25f, -0.25f,  0.25f,
			0.25f,  0.25f,  0.25f,		0.25f,  0.25f, -0.25f,		 0.25f, -0.25f, -0.25f,
			0.25f,  0.25f, -0.25f,		0.25f,  0.25f,  0.25f,		-0.25f,  0.25f,  0.25f,
			0.25f,  0.25f, -0.25f,		-0.25f,  0.25f,  0.25f,		-0.25f,  0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,		-0.25f, -0.25f,  0.25f,		 0.25f, -0.25f,  0.25f,
			0.25f, -0.25f, -0.25f,		-0.25f, -0.25f, -0.25f,		-0.25f, -0.25f,  0.25f,
		};

		const GLfloat color_data[] = {
			// color data
			0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f, //
			0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.8f, 0.8f, 1.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.8f, 0.8f, 0.8f, 1.0f, // 0.8 -> 0.5
			0.8f, 0.8f, 0.8f, 1.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.8f, 0.8f, 0.8f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f, //
			0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,		0.5f, 0.5f, 0.0f, 1.0f,		0.5f, 0.5f, 0.0f, 1.0f, //
			0.5f, 0.5f, 0.0f, 1.0f,		0.5f, 0.5f, 0.0f, 1.0f,		0.5f, 0.5f, 0.0f, 1.0f, 
			1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, //
			1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f, //
			0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,
		};

		GLuint vertex_array_id, vertex_buffer_id, vertex_buffer_size, color_buffer_id, color_buffer_size;

		vertex_buffer_size = sizeof(vertexData);
		color_buffer_size = sizeof(color_data);

		glGenVertexArrays(1, &vertex_array_id);
		glBindVertexArray(vertex_array_id);

		// buffer vertex buffer
		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, &vertexData, GL_STATIC_DRAW);

		// config VertexAttrib
		GLuint location1 = new_shader_program->GetAttribLocation("vPosition");
		glEnableVertexAttribArray(location1);
		glVertexAttribPointer(location1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		// buffer color buffer
		glGenBuffers(1, &color_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, color_buffer_size, &color_data, GL_STATIC_DRAW);
		
		// config VertexAttrib
		GLuint location2 = new_shader_program->GetAttribLocation("vColor");
		new_shader_program->Enable().SetParameter<GLboolean>("HasColorVertices", GL_TRUE);
		this->SetHasColor(true);

		glEnableVertexAttribArray(location2); 
		glVertexAttribPointer(location2, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(location1); glDisableVertexAttribArray(location2);/**/

		this->SetVAO(vertex_array_id);
		this->SetNumIndex(36); this->SetFirstIndex(0);
		this->SetRenderMode(KG::Mesh::RenderMode::Arrays);
		this->SetPrimitiveType(GL_TRIANGLES);
	}

	Cube3D::~Cube3D(void)
	{}
	
	void Cube3D::Update(KG::Scene & p_rScene, const KE::Duration p_DeltaTime)
	{
		SceneNode::Update(p_rScene, p_DeltaTime);
		static KE::Duration elapsed; elapsed += p_DeltaTime;
		this->SetRotation
		(
			elapsed.AsSeconds() * 90
			, elapsed.AsSeconds() * 90
			, elapsed.AsSeconds() * 90
		);
	}

} // object ns