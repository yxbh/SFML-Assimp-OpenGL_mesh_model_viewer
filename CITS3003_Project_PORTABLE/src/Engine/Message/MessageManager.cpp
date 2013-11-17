#include "MessageManager.hpp"

namespace KE
{
	std::shared_ptr<MessageManager> MessageManager::s_spMessageManager = nullptr;

	MessageManager & MessageManager::Create(void)
	{
		assert(s_spMessageManager==nullptr);
		s_spMessageManager =  std::shared_ptr<MessageManager>(new MessageManager());
		return *s_spMessageManager;
	}

	MessageManager & MessageManager::GetMessageManager(void)
	{
		assert(s_spMessageManager!=nullptr);
		return *s_spMessageManager;
	}

	void MessageManager::RegisterInputDevice(KE::RenderDevice * const p_rInputDevice)
	{
		m_pEngineInputDevice = p_rInputDevice; m_SystemsRegistered = m_SystemsRegistered|EngineSystemType::InputDevice;
	}
	
	void MessageManager::Register(KE::IMessageHandler * const p_pSystem, const EngineSystemType & p_rEngineSystemType)
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
			KE::Debug::print(Debug::msg_type::DBG_ERROR, "MessageManager : trying to register unknown system type."); break;
		}
	}

	void MessageManager::Update(const KE::Duration p_Duration)
	{
		KE_UNREFERENCED_PARAMETER(p_Duration);
		this->CollectMessage();
		this->DistributeMessages();
	}

	bool MessageManager::Ready(void) const
	{
		return (m_SystemsRegistered == EngineSystemType::All);
	}

	void MessageManager::AddMessage(const KE::Message & p_rMessage)
	{
		m_MessageQueue.Push(p_rMessage);
	}

	MessageManager::MessageManager(void)
		: m_SystemsRegistered(0)
		, m_pEngine(nullptr)
		, m_pEngineInputDevice(nullptr)
		, m_pEngineRenderSystem(nullptr)
		, m_pEngineGUISystem(nullptr)
		, m_pEngineRenderer(nullptr)
		, m_pEngineInputSystem(nullptr)
	{
	}

	void MessageManager::CollectMessage(void)
	{
		assert(this->Ready());
		KE::Message new_msg;
		while (m_pEngine->GetMessageOutbox().Poll(new_msg))
			m_MessageQueue.Push(new_msg);
		while (m_pEngineRenderSystem->GetMessageOutbox().Poll(new_msg))
			m_MessageQueue.Push(new_msg);
		while (m_pEngineGUISystem->GetMessageOutbox().Poll(new_msg))
			m_MessageQueue.Push(new_msg);
		while (m_pEngineRenderer->GetMessageOutbox().Poll(new_msg))
			m_MessageQueue.Push(new_msg);
		while (m_pEngineInputSystem->GetMessageOutbox().Poll(new_msg))
			m_MessageQueue.Push(new_msg);
	}

	void MessageManager::DistributeMessages(void)
	{
		KE::Message new_msg;
		while (m_MessageQueue.Poll(new_msg))
		{
			this->DistributeMessage(new_msg);
		}
	}

	void MessageManager::DistributeMessage(const KE::Message p_Message)
	{
		assert(Ready());
		switch (p_Message.type)
		{
		case KE::Message::MouseMoved:
			m_pEngineRenderSystem->GetMessageInbox().Push(p_Message);
			break;
		case KE::Message::Resized:
			m_pEngineGUISystem->GetMessageInbox().Push(p_Message);
			break;
		default: break;
		}
	}
} // KE ns
