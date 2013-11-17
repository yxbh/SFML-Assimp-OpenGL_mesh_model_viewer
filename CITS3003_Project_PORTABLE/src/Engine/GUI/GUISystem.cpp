#include "GUISystem.hpp"

namespace KE
{
	GUISystem::GUISystem(void)
		: m_pRenderDevice(nullptr)
	{
		TestSetup();
	}
	GUISystem::~GUISystem(void)
	{
		m_SFGUIDesktop.RemoveAll();
		m_pRenderDevice = nullptr;
	}

	void GUISystem::Init(void)
	{
		// for delayed initialisation.
	}

	void GUISystem::RegisterRenderDevice(KE::RenderDevice * const p_pRenderDevice)
	{
		m_pRenderDevice = p_pRenderDevice;
	}

	void GUISystem::Update(const KE::Duration p_Duration)
	{
		ProcessEvents();
		ProcessMessages();
		
		m_SFGUIDesktop.Update(static_cast<float>(p_Duration.AsSeconds()));
	}

	void GUISystem::Render(void)
	{
		glDisable(GL_DEPTH_TEST);
		KE::Debug::check_for_GL_error();
		//assert(m_pRenderDevice!=nullptr);   // TODO : put it back and fix crash issue regarding engine exit.
		m_SFGUIContext.Display(*m_pRenderDevice);
	}

	sfg::Desktop & GUISystem::GetSFGDesktop(void)
	{
		return m_SFGUIDesktop;
	}

	const bool GUISystem::EventDelegate(const KE::Event_SmartPtr p_spEvent)
	{
		/*switch (p_spEvent->GetType())
		{
			case 
		}*/
		return false;
	}

	void GUISystem::ProcessEvents(void)
	{
		assert(m_pRenderDevice!=nullptr);
		KE::SystemEvent new_event;
		while (this->GetEventInbox().Poll(new_event))
		{
			m_SFGUIDesktop.HandleEvent(new_event);
		}
	}

	void GUISystem::ProcessMessages(void)
	{
		this->EmptyMessageInbox();
	}

} // KE ns
