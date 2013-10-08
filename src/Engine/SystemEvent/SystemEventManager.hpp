#pragma once
#include "../common.hpp"
#include "ISystemEventHandler.hpp"
#include "SystemEventQueue.hpp"

namespace KE
{
	/*! \class SystemEventManager
		Handles and distribute/redistrbutes engine and system events.
	*/
	class SystemEventManager
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
		int16_t							m_SystemsRegistered;
		KE::RenderDevice *				m_pEngineInputDevice;
		KE::ISystemEventHandler *		m_pEngine;
		KE::ISystemEventHandler *		m_pEngineInputSystem;
		KE::ISystemEventHandler *		m_pEngineGUISystem;
		KE::ISystemEventHandler *		m_pEngineRenderSystem;
		KE::ISystemEventHandler *		m_pEngineRenderer;

		bool		m_WindowInFocus;

	public:
		SystemEventManager(void);

		void RegisterInputDevice(KE::RenderDevice * const p_pInputDevice);
		void Register(KE::ISystemEventHandler * const p_pSystem, const EngineSystemType & p_rEngineSystemType);

		void Update(const KE::Duration p_Duration);
		
		/*! Check whether all required systems are registered. */
		bool Ready(void) const;

	private:
		/*! process all events. */
		void ProcessEvents(void);
		/*! Distribute event to relevant system/s. */
		void DistributeEvent(const KE::SystemEvent & p_NewEvent);
	};

} // KE ns
