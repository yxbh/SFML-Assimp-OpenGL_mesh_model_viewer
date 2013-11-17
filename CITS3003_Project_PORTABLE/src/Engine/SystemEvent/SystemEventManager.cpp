#include "SystemEventManager.hpp"

namespace KE
{
	SystemEventManager::SystemEventManager(void)
		: m_SystemsRegistered(EngineSystemType::Null)
		, m_pEngineInputDevice(nullptr)
		, m_pEngineInputSystem(nullptr)
		, m_pEngineRenderSystem(nullptr)
		, m_pEngineRenderer(nullptr)
		, m_WindowInFocus(true)
	{}

	void SystemEventManager::RegisterInputDevice(KE::RenderDevice * const p_rInputDevice)
	{
		m_pEngineInputDevice = p_rInputDevice; m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::InputDevice;
	}

	void SystemEventManager::Register(KE::ISystemEventHandler * const p_pSystem, const EngineSystemType & p_rEngineSystemType)
	{
		assert(p_pSystem != nullptr);
		switch (p_rEngineSystemType)
		{
		case EngineSystemType::EngineBackBone:
			m_pEngine = p_pSystem;
			m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::EngineBackBone;
			break;
		case EngineSystemType::RenderSystem:
			m_pEngineRenderSystem = p_pSystem;
			m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::RenderSystem;
			break;
		case EngineSystemType::GUISystem:
			m_pEngineGUISystem = p_pSystem;
			m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::GUISystem;
			break;
		case EngineSystemType::Renderer:
			m_pEngineRenderer = p_pSystem;
			m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::Renderer;
			break;
		case EngineSystemType::InputSystem:
			m_pEngineInputSystem = p_pSystem;
			m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::InputSystem;
			break;
		default:
			KE::Debug::print(Debug::msg_type::DBG_ERROR, "EventManager : trying to register unknown system type."); break;
		}
	}

	void SystemEventManager::Update(const KE::Duration p_Duration)
	{
		KE_UNREFERENCED_PARAMETER(p_Duration);
		ProcessEvents();
	}

	bool SystemEventManager::Ready(void) const
	{
		return (m_SystemsRegistered == EngineSystemType::All);
	}

	void SystemEventManager::ProcessEvents(void)
	{
		assert(Ready());
		sf::Event new_event;
		while (m_pEngineInputDevice->pollEvent(new_event))
			this->DistributeEvent(new_event);
	}

	void SystemEventManager::DistributeEvent(const KE::SystemEvent & p_NewEvent)
	{
		if(m_WindowInFocus)
		{
			m_pEngineGUISystem->GetEventInbox().Push(p_NewEvent);

			switch (p_NewEvent.type)
			{
			case KE::SystemEvent::Closed:
				m_pEngine->GetEventInbox().Push(p_NewEvent);
				break;
			case KE::SystemEvent::Resized:
				m_pEngineRenderer->GetEventInbox().Push(p_NewEvent);
				break;
			case KE::SystemEvent::GainedFocus:
			case KE::SystemEvent::LostFocus:
			case KE::SystemEvent::TextEntered:
			case KE::SystemEvent::KeyPressed:
			case KE::SystemEvent::KeyReleased:
			case KE::SystemEvent::MouseWheelMoved:
			case KE::SystemEvent::MouseButtonPressed:
			case KE::SystemEvent::MouseButtonReleased:
			case KE::SystemEvent::MouseMoved:
			case KE::SystemEvent::MouseEntered:
			case KE::SystemEvent::MouseLeft:
				m_pEngineInputSystem->GetEventInbox().Push(p_NewEvent);
				m_pEngineRenderSystem->GetEventInbox().Push(p_NewEvent);
				break;
			}
		}
	}

} // KE ns
