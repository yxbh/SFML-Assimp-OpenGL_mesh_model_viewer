#pragma once
#include "Message.hpp"
#include "IMessageHandler.hpp"

namespace KE
{
	

	/*! \class MessageHandler
		This class uses the singleton design pattern. There will only be one MessageHandler.
	*/
	class MessageManager
	{
	public:
		enum EngineSystemType
		{
			Null				= 0,
			EngineBackBone		= 1 << 0,	
			InputDevice			= 1 << 1,
			RenderSystem		= 1 << 2,
			GUISystem			= 1 << 3,
			Renderer			= 1 << 4,
			InputSystem			= 1 << 5,
			All					= EngineBackBone|InputDevice|RenderSystem|GUISystem|Renderer|InputSystem
		};

	private:
		static std::shared_ptr<MessageManager> s_spMessageManager;

		KE::MessageQueue		m_MessageQueue;

		int16_t					m_SystemsRegistered;
		KE::RenderDevice *		m_pEngineInputDevice;
		KE::IMessageHandler *	m_pEngine;
		KE::IMessageHandler *	m_pEngineRenderSystem;
		KE::IMessageHandler *	m_pEngineGUISystem;
		KE::IMessageHandler *	m_pEngineRenderer;
		KE::IMessageHandler *	m_pEngineInputSystem;

	public:
		static MessageManager & Create(void);
		static MessageManager & GetMessageManager(void);
		
		void RegisterInputDevice(KE::RenderDevice * const p_pInputDevice);
		/*! Register a system to receive handle KE::Messages. */
		void Register(KE::IMessageHandler * const p_pSystem, const EngineSystemType & p_rEngineSystemType);

		void Update(const KE::Duration p_Duration);
		/*! Check whether all required systems are registered. */
		bool Ready(void) const;

		void AddMessage(const KE::Message & p_rMessage);
		

	private:
		MessageManager(void);
		/*! process all events and pass relevant events to each engine sub system. */
		void CollectMessage(void);
		void DistributeMessages(void);
		void DistributeMessage(const KE::Message p_Message);
	}; // MessageHandler class

} // KE ns
