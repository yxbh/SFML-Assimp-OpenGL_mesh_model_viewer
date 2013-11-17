#pragma once
#include "../../common.hpp"
#include "../../GUI/GUISystem.hpp"
#include "../../Message/IMessageHandler.hpp"
#include "../../SystemEvent/ISystemEventHandler.hpp"
#include "Scene.hpp"
#include "Light/LightManager.hpp"

namespace KG
{
	class KG::Renderer;
	typedef std::shared_ptr<KG::Renderer> Renderer_SmartPtr;

	/*! \class RenderSystem
	*/
	class RenderSystem
		: public KE::IMessageHandler
		, public KE::ISystemEventHandler
	{
	private:
		KE::RenderDevice *	m_pAppWindow;

		KG::Scene					m_Scene;
		KG::Renderer_SmartPtr		m_spRenderer;
		KG::LightManager			m_LightManager;

		KE::GUISystem_SmartPtr		m_spGUIsystem;

	public:
		RenderSystem(KE::RenderDevice * const p_pRenderDevice);
		~RenderSystem(void);

		void RegisterAppWindow(KE::RenderDevice * p_pAppWindow);

		/*! */
		void Update(const KE::Duration m_DeltaTime);
		/*! Update entire scene. */
		void UpdateScene(const KE::Duration m_MicroSec);
		/*! Update GUI system. */
		void UpdateGUI(const KE::Duration m_DeltaTime);

		/*! TODO : TEMP METHODS UNTIL SUMMER BREAK REFACTOR */
		/*! setup light in shader program */
		void SetupLight(void);

		/*! setup lights, and views and etc. */
		void PreRender(void);
		/*! Render entire scene. */
		void RenderScene(void);
		/*! Render GUI. */
		void RenderGUI(void);

		KG::Scene & GetScene(void);
		KE::GUISystem & GetGUISystem(void);
		KG::Renderer & GetRenderer(void);
		KG::LightManager & GetLightManager(void);

		virtual void ProcessMessages(void) final;
		virtual void ProcessEvents(void) final;

	};

namespace Graphics
{
	extern KG::RenderSystem * g_pRenderSystem;

	extern KG::RenderSystem & Get(void);

} // Graphics ns
} // KG ns
