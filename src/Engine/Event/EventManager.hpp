#pragma once
#include "../common.hpp"
#include "IEvent.hpp"
#include "EventQueue.hpp"
#include "../Entity/EntityID.hpp"

namespace KE
{
	typedef std::map<KE::EntityID, KE::EventDelegate>	ListenerMap;
	typedef std::map<KE::EventType, KE::ListenerMap>	CommandListenersMap;


	/*! \class EventManager
	*/
	class EventManager
	{
	private:
		KE::EventQueue					m_ThreadSafeEventQueue;
		std::list<KE::Event_SmartPtr>	m_EventQueue;
		KE::CommandListenersMap			m_Listeners;

	public:
		EventManager(void);
		~EventManager(void);

		/*! Register a listener/delegate to a specific event type. */
		bool AddListener(const KE::EventDelegate & p_rEventDelegate, const EventType p_CmdType);
		/*! Register a listener/delegate to a specific event type. */
		bool AddListener(const KE::EntityID p_EntityID, const KE::EventDelegate & p_rEventDelegate, const EventType p_CmdType);
		/*! Deregister a listener/delegate with the given EntityID from a specific event type. */
		bool RemoveListener(const KE::EntityID p_EntityID, const EventType p_CmdType);
		/*! Deregister a listener/delegate from a specific event type. */
		//bool RemoveListener(const KE::EventDelegate & p_rEventDelegate, const EventType p_CmdType);
		/*! Add an event into the Event queue. */
		void QueueEvent(KE::Event_SmartPtr p_spNewEvent);
		/*! Call all delegates listening to that event now. Return false if Event is not handled for whatever reason. */
		bool TriggerEventNow(KE::Event_SmartPtr p_Event);
		/*! Remove first instance of an event of a particular type. If true all event belong to the type will be removed. Does not remove event from the thread safe queue.*/
		bool RemoveEvent(const EventType p_EventType, const bool p_RemoveAllSame = false);

		/*! Fire off all Commands (With in limit, if one is set), returns true if all Commands were fired. */
		bool Update(const KE::Duration p_Duration, const KE::Duration p_DurationLimit = KE::Duration::Microseconds(0));

	private:

	}; // ICommandHandler class

	namespace Event
	{
		extern KE::EventManager * s_pEventManager;

		extern KE::EventManager & Get(void);
	}

} // KE ns
