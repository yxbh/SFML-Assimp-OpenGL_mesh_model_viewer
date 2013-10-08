#pragma once
#include "SystemEventQueue.hpp"

namespace KE
{
	/*! \class ISystemEventHandler
		Inherit from this class to receive and send KE::Event.
	*/
	class ISystemEventHandler
	{
		friend class SystemEventManager;

	private:
		KE::SystemEventQueue	m_EventInbox, m_EventOutbox;
	protected:
		void PushEventToInbox(const KE::SystemEvent & p_Msg)
		{
			m_EventInbox.Push(p_Msg);
		}

		void PushEventToOutbox(const KE::SystemEvent & p_Msg)
		{
			m_EventOutbox.Push(p_Msg);
		}

		KE::SystemEventQueue & GetEventInbox(void)
		{
			return m_EventInbox;
		}

		KE::SystemEventQueue & GetEventOutbox(void)
		{
			return m_EventOutbox;
		}

		void EmptyEventInbox(void)
		{
			KE::SystemEvent new_event;
			while (this->GetEventInbox().Poll(new_event)) {}
		}

		void EmptyEventOutbox(void)
		{
			KE::SystemEvent new_event;
			while (this->GetEventOutbox().Poll(new_event)) {}
		}

		virtual void ProcessEvents(void)
		{
			this->EmptyEventInbox();
		}

	};

} // KE ns
