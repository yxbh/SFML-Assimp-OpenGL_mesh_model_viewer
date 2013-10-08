#include "RenderSystem.hpp"
#include "Renderer.hpp"

// TEMP
#include "../../Input/InputManager.hpp"
#include "Shader/ShaderProgram.hpp"
// /TEMP

namespace KG
{
	RenderSystem::RenderSystem(KE::RenderDevice * const p_pRenderDevice)
		: m_pAppWindow(nullptr)
		, m_spRenderer(new KG::Renderer)
		, m_spGUIsystem(KE::GUISystem_SmartPtr(new KE::GUISystem()))
	{
		assert(KG::Graphics::g_pRenderSystem == nullptr);
		KG::Graphics::g_pRenderSystem = this;
		m_spRenderer->RegisterRenderDevice(p_pRenderDevice);
		this->RegisterAppWindow(p_pRenderDevice);
		m_spGUIsystem->RegisterRenderDevice(p_pRenderDevice);
		m_Scene.SetRenderer(m_spRenderer);
	}

	RenderSystem::~RenderSystem(void)
	{
		m_pAppWindow->close();
	}

	void RenderSystem::RegisterAppWindow(KE::RenderDevice * p_pAppWindow)
	{
		m_pAppWindow = p_pAppWindow;
	}

	void RenderSystem::Update(const KE::Duration m_DeltaTime)
	{
		this->ProcessEvents();
		this->ProcessMessages();

		m_LightManager.Update(m_DeltaTime);
		m_spRenderer->Update(m_DeltaTime);
	}

	void RenderSystem::UpdateScene(const KE::Duration m_MicroSec)
	{
		m_Scene.UpdateAll(m_MicroSec);
	}

	void RenderSystem::UpdateGUI(const KE::Duration m_DeltaTime)
	{
		m_spGUIsystem->Update(m_DeltaTime);
	}

	void RenderSystem::SetupLight(void)
	{
		m_LightManager.SetupLights(m_spRenderer->m_spShaderProgram);
	}

	void RenderSystem::PreRender(void)
	{
		// prepare renderer,
		m_spRenderer->PreRender();

		m_spRenderer->m_spShaderProgram->Enable();

		// view: camera position.
		glm::vec3 camera_pos(m_Scene.GetCamera().GetPositionVec());
		m_spRenderer->m_spShaderProgram->SetParameter<glm::vec3>("CameraPosition", camera_pos);

		// TODO TEMP HERE UNTIL SUMMER BREAK REFACTOR.
		m_LightManager.SetupLights(m_spRenderer->m_spShaderProgram);
	}

	void RenderSystem::RenderScene(void)
	{
		/* Enable depth drawing. */
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);	// dont draw if it's less or equal in depth.
		glEnable(GL_DEPTH_CLAMP);
		glDepthRange(0.0f, 1.0f);
		glFrontFace(GL_CCW); // define winding order of front face. GL convention : CCW.
		/* culling. */
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CW);
		/* Check for error.  */
		KE::Debug::check_for_GL_error();

		this->PreRender();
		KE::Debug::check_for_GL_error();
		m_Scene.RenderAll();

		m_spRenderer->DoRenderPasses();
		m_spRenderer->PostRender();
	}

	void RenderSystem::RenderGUI(void)
	{
		m_spGUIsystem->Render();
	}

	KG::Scene & RenderSystem::GetScene(void)
	{
		return m_Scene;
	}

	KG::Renderer & RenderSystem::GetRenderer(void)
	{
		if (m_spRenderer == nullptr)
			assert(false);
		return *m_spRenderer;
	}

	KE::GUISystem & RenderSystem::GetGUISystem(void)
	{
		return *m_spGUIsystem;
	}

	KG::LightManager & RenderSystem::GetLightManager(void)
	{
		return m_LightManager;
	}

	void RenderSystem::ProcessMessages(void)
	{
		this->EmptyMessageInbox();
	}

	void RenderSystem::ProcessEvents(void)
	{
		this->EmptyEventInbox();
	}

namespace Graphics
{
	KG::RenderSystem * g_pRenderSystem = nullptr;

	KG::RenderSystem & Get(void)
	{
		assert(g_pRenderSystem != nullptr);
		return *g_pRenderSystem;
	}

} // Graphics ns
}
