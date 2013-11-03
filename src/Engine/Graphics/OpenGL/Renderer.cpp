#include "Renderer.hpp"
#include "Scene.hpp"
#include "SceneNode.hpp"
#include "Asset/Mesh.hpp"
#include "Asset/Texture.hpp"
#include "Asset/Skeleton.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Shader/ShaderFactory.hpp"
#include "RenderSystem.hpp"
/* Event related. */
#include "../../Event/EventManager.hpp"
#include "Events.hpp"


namespace KG
{
	const double	Renderer::sk_DefaultAspectRatio = 1.7778;
	const unsigned	Renderer::sk_DefaultWidth = 720;
	const unsigned	Renderer::sk_DefaultHeight 
		= static_cast<int>((double)Renderer::sk_DefaultWidth/Renderer::sk_DefaultAspectRatio);	

	Renderer::Renderer(void)
		: m_CurrentAspectRatio(sk_DefaultAspectRatio)
		, m_CurrentWindowWidth(sk_DefaultWidth)
		, m_CurrentWindowHeight(sk_DefaultHeight)
		, m_CurrentViewportWidth(m_CurrentWindowWidth)
		, m_CurrentViewportHeight(m_CurrentWindowHeight)
		, m_pRenderDevice(nullptr)
		, m_RenderDeviceRegistered(false)
		, m_Initialized(false)
		, m_spShaderProgram(nullptr)
	{
		m_RenderMap.insert(std::make_pair(KG::RenderPass::Null, SceneNodeSPVector()));
		m_RenderMap.insert(std::make_pair(KG::RenderPass::NotRendered, SceneNodeSPVector()));
		m_RenderMap.insert(std::make_pair(KG::RenderPass::Dynamic, SceneNodeSPVector()));
		m_RenderMap.insert(std::make_pair(KG::RenderPass::Static, SceneNodeSPVector()));
		m_RenderMap.insert(std::make_pair(KG::RenderPass::Sky, SceneNodeSPVector()));
	}

	Renderer::~Renderer(void)
	{
		this->ShutDown();
	}

	void Renderer::Initalize(void)
	{
		if (!m_RenderDeviceRegistered) // temp until sf::Window is created in here.
			KE::Debug::print(KE::Debug::msg_type::DBG_ERROR, "Render device is not registered yet.");

		m_pRenderDevice->create
		(
			sf::VideoMode(m_CurrentWindowWidth, m_CurrentWindowHeight)
			, "K-Engine"
			, sf::Style::Default
			, sf::ContextSettings::ContextSettings(24, 8, 0, 3, 3)
		);

		glewExperimental = GL_TRUE;
		GLenum glew_init_status = glewInit();
		if (glew_init_status != GLEW_OK)
		{
			std::string fail_string("Failure GLEW intialization... \n ");
			fail_string += reinterpret_cast<const char*>(glewGetErrorString(glew_init_status));
			KE::Debug::print(fail_string);
			throw std::exception(fail_string.c_str());
		}
		else
		{
			m_Initialized = true;
			this->SetWindowSize(m_CurrentWindowWidth, m_CurrentWindowHeight);
			this->SetViewportSize(0,0, m_CurrentWindowWidth, m_CurrentWindowHeight);
		}

		/*!  TODO : TEMP CODE UNTIL SUMMER BREAK REFACTOR */
		/* setup renderer simple shader program. */
		KG::ShaderResourceList new_shader_rlist;
		new_shader_rlist.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
		new_shader_rlist.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
		KG::ShaderProgram_SmartPtr new_shader_program(new KG::ShaderProgram());
		new_shader_program->SetGLHandle(KG::ShaderProgramFactory::MakeFromFiles(new_shader_rlist));
		m_spShaderProgram = new_shader_program;
	}

	void Renderer::ShutDown(void)
	{
		if (m_pRenderDevice == nullptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Renderer : ShutDown(), m_pRenderDevice points to null.");
			return;
		}
	}

	void Renderer::RegisterRenderDevice(KE::RenderDevice * const p_pRenderDevice)
	{
		if (m_pRenderDevice != nullptr)
			KE::Debug::print(KE::Debug::msg_type::DBG_WARNING, "Renderer : RenderDevice already registered.");
		m_pRenderDevice = p_pRenderDevice; m_RenderDeviceRegistered = true;
	}

	void Renderer::Update(const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
		this->ProcessEvents();
		this->ProcessMessages();
	}

	void Renderer::Clear(const float p_R, const float p_G, const float p_B, const float p_A) const
	{
		glClearColor(p_R, p_G, p_B, p_A);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::PreRender(void)
	{
		
		for (auto & list : m_RenderMap)
			list.second.reserve(300);
	}

	void Renderer::Render(KG::Scene & p_rScene, KG::SceneNode * const p_pSceneNode)
	{
		m_RenderMap[p_pSceneNode->GetRenderPass()]
			//.push_back(std::make_pair(p_pSceneNode, p_rScene.GetMatrixStack().Top()));
			.push_back(p_pSceneNode);
	}

	void Renderer::RenderPassMesh(void)
	{
		for (auto & node_list_pair : m_RenderMap)
		{
		for (auto & node_ptr : node_list_pair.second)
		{
			KG::Mesh * const mesh_ptr(static_cast<KG::Mesh*const>(node_ptr)); // crash if not mesh. TODO : proper type thing. maybe add a NodeType method into SceneNode.

			if (mesh_ptr->GetRenderPass() == KG::RenderPass::Null)
				KE::Debug::print(KE::Debug::DBG_ERROR, "Renderer : RenderPass not set (default to 'null').");
			if (mesh_ptr->GetRenderPass() == KG::RenderPass::NotRendered)
				return;
		
			// mvp matrix
			KG::Scene & scene_ref(KG::Graphics::Get().GetScene());
			const glm::dmat4 camera_view_matrix(scene_ref.GetCamera().GetViewProjectionMatrixd());
			const glm::dmat4 & model_matrix(node_ptr->GetCurrentToWorldTransform());
			auto shader_program_ptr = m_spShaderProgram;
			shader_program_ptr->Use(); // already called in PreRender(). but will leave it here for now.
			shader_program_ptr->SetParameter<glm::mat4>("mvpMatrix", glm::mat4(camera_view_matrix*model_matrix));

			// setup color or texture
			if (mesh_ptr->HasVertexColors())
				shader_program_ptr->SetParameter<GLboolean>("HasColorVertices", GL_TRUE);
			else if (mesh_ptr->HasTexture())
			{
				shader_program_ptr->SetParameter<GLboolean>("HasTexCoord2D", GL_TRUE);
				shader_program_ptr->SetParameter<GLfloat>("TexCoordMultiplier", static_cast<float>(mesh_ptr->GetTexture()->m_TexCoordMultiplier));
			}
			else
			{
				shader_program_ptr->SetParameter<GLboolean>("HasColorVertices", GL_FALSE);
				shader_program_ptr->SetParameter<GLboolean>("HasTexCoord2D", GL_FALSE);
			}

			// texture
			if (mesh_ptr->HasTexture())
			{
				mesh_ptr->GetTexture()->Bind(GL_TEXTURE0);
				shader_program_ptr->SetParameter<GLboolean>("HasTexCoord2D", GL_TRUE);
				shader_program_ptr->SetParameter<GLint>("Tex2D", 0);
			}
			else
			{
				shader_program_ptr->SetParameter<GLboolean>("HasTexCoord2D", GL_FALSE);
			}

			// material
			const KG::Material & mat(mesh_ptr->GetMaterial());
			if (mesh_ptr->HasMaterial()) // this is always true ATM (02/11/2013).
			{
				shader_program_ptr->SetParameter<glm::vec3>("material.Emissive", mat.Emissive);
				shader_program_ptr->SetParameter<glm::vec3>("material.Ambient", mat.Ambient);
				shader_program_ptr->SetParameter<glm::vec3>("material.Diffuse", mat.Diffuse);
				shader_program_ptr->SetParameter<glm::vec3>("material.Specular", mat.Specular);
				shader_program_ptr->SetParameter<GLfloat>("material.Shininess", mat.Shininess);
			}

			// model
			shader_program_ptr->SetParameter<glm::mat4>("modelMatrix", glm::mat4(model_matrix));
			shader_program_ptr->SetParameter<glm::mat3>("ModelRotationMat3", glm::mat3(glm::dmat3(mesh_ptr->GetOrientationMat())));// glm::mat3(glm::transpose(glm::inverse(glm::dmat3(model_matrix)))));
			// light
			if (mesh_ptr->IsBackFaceLit())
				shader_program_ptr->SetParameter<GLboolean>("LightBackFace", GL_TRUE);
			else
				shader_program_ptr->SetParameter<GLboolean>("LightBackFace", GL_FALSE);
			if (mesh_ptr->HasNormals())
				shader_program_ptr->SetParameter<GLboolean>("HasNormals", GL_TRUE);
			else
				shader_program_ptr->SetParameter<GLboolean>("HasNormals", GL_FALSE);
			KE::Debug::check_for_GL_error();

			// animation
			if (mesh_ptr->HasSkeleton())
			{
				shader_program_ptr->SetParameter<GLboolean>("HasBones", GL_TRUE);
				shader_program_ptr->SetParameter("BoneTranforms[0]", mesh_ptr->GetSkeleton()->final_transforms);
			}
			else
			{
				shader_program_ptr->SetParameter<GLboolean>("HasBones", GL_FALSE);
			}

			if (mesh_ptr->GetVAO() != 0)
			{
				glBindVertexArray(mesh_ptr->GetVAO());
				if (mesh_ptr->GetRenderMode() == KG::Mesh::RenderMode::Arrays)
				{
					glDrawArrays(mesh_ptr->GetGLPrimitiveType(), mesh_ptr->GetFirstIndex(), mesh_ptr->GetNumIndex());
				}
				else if (mesh_ptr->GetRenderMode() == KG::Mesh::RenderMode::Indexed)
				{
					glDrawElements(mesh_ptr->GetGLPrimitiveType(), mesh_ptr->GetNumElement()
						, mesh_ptr->GetIndexVarType(), (const GLvoid*)(mesh_ptr->GetIndexOffset()));
				}
				else
				{
					KE::Debug::print(KE::Debug::DBG_ERROR, "Renderer : render mode is not set (default to null).");
				}
				glBindVertexArray(0);
			}
			else
			{
				//KE::Debug::print(KE::Debug::DBG_ERROR, "Renderer : VAO id is 0.");			
			}
			KE::Debug::check_for_GL_error();

			shader_program_ptr->Disable();
		}
		}
	}

	void Renderer::RenderPassShadow(void)
	{

	}

	void Renderer::PostRender(void)
	{
		for (auto & list : m_RenderMap)
			list.second.clear();
	}

	void Renderer::DoRenderPasses(void)
	{
		this->RenderPassShadow();
		this->RenderPassMesh();
	}

	void Renderer::Display(void) const
	{
		m_pRenderDevice->display();
	}

	const double Renderer::GetAspectRatio(void) const
	{
		return m_CurrentAspectRatio;
	}

	const size Renderer::GetSize(void) const
	{
		return size(m_CurrentWindowWidth, m_CurrentWindowHeight, m_CurrentViewportWidth, m_CurrentViewportHeight);
	}

	const std::string Renderer::GetAPIVersion(void) const
	{
		if (!Avaliable()) return "N/A";
		sf::ContextSettings OpenGL_context_setting = m_pRenderDevice->getSettings();
		std::string info_string = "OpenGL version = Major: " + std::to_string(OpenGL_context_setting.majorVersion);
		info_string += "  Minor: " + std::to_string(OpenGL_context_setting.minorVersion);
		return info_string;
	}

	void Renderer::SetAspectRatio(const double p_NewAspectRatio)
	{
		m_CurrentAspectRatio = p_NewAspectRatio;
	}

	void Renderer::SetViewportSize(GLint p_X,GLint p_Y, GLsizei p_Width, GLsizei p_Height)
	{
		glViewport(p_X, p_Y, p_Width, p_Height);
	}

	void Renderer::SetWindowSize(const unsigned p_NewWindowWidth, const unsigned p_NewWindowHeight)
	{
		if (Avaliable()) m_pRenderDevice->setSize(sf::Vector2u(p_NewWindowWidth, p_NewWindowHeight));
	}

	void Renderer::SetActive(const bool p_Active)
	{
		if (Avaliable()) m_pRenderDevice->setActive(p_Active);
	}

	bool Renderer::Avaliable(void) const
	{
		if (m_Initialized && m_RenderDeviceRegistered)
			return true;
		return false;
	}

	void Renderer::ProcessMessages(void)
	{
		this->EmptyMessageInbox();
	}

	void Renderer::ProcessEvents(void)
	{
		KE::SystemEvent new_event;
		while (this->GetEventInbox().Poll(new_event))
		{
			switch(new_event.type)
			{
			case KE::SystemEvent::Resized:
				{
					int width = new_event.size.width; int height = new_event.size.height;
					m_CurrentWindowWidth = width; m_CurrentWindowHeight = height;
					KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KG::Event::WindowResize_Event(width, height)));
					glViewport(0,0, GLsizei(width), GLsizei(height));
					KE::Debug::print("Renderer : window resized.");
					KE::Debug::print("new size: width = "+std::to_string(width)+" height = "+std::to_string(height));
					break;
				}
			}
		}
	}

}
