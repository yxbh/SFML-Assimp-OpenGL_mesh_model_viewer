#include "EventManager.hpp"
#include "../../Utility/System/SFML2_Time.hpp"

namespace KE
{
	EventManager::EventManager(void)
	{
		assert(KE::Event::s_pEventManager == nullptr);
		KE::Event::s_pEventManager = this;
	}

	EventManager::~EventManager(void)
	{
		m_Listeners.clear();
		m_EventQueue.clear();
	}

	bool EventManager::AddListener(const KE::EntityID p_EntityID, const KE::EventDelegate & p_rEventDelegate, const EventType p_CmdType)
	{
		if (p_CmdType == KE::Invalid_EventType)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "CommandManager : Invalid command type.");
			return false;
		}
		if (p_EntityID == KE::Invalid_EntityID)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "CommandManager : Invalid EntityID.");
			return false;
		}

		KE::ListenerMap & listeners = m_Listeners[p_CmdType]; // find or create entry.
		if (listeners.find(p_EntityID) != listeners.end())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "EventManager : double registration.");
			return false;
		}

		listeners.insert(std::make_pair(p_EntityID, p_rEventDelegate));
		return true;
	}

	bool EventManager::RemoveListener(const KE::EntityID p_EntityID, const EventType p_CmdType)
	{
		if (p_EntityID == KE::Invalid_EntityID)
			KE::Debug::print(KE::Debug::DBG_ERROR, "CommandManager : Invalid EntityID.");
		auto it = m_Listeners.find(p_CmdType);
		if (it == m_Listeners.end()) // no such event type registered.
			return false;

		KE::ListenerMap & listeners = it->second;
		std::size_t i = listeners.erase(p_EntityID);
		if (i == 0)
			return false;
		return true;
	}

	void EventManager::QueueEvent(KE::Event_SmartPtr p_spNewEvent)
	{
		m_ThreadSafeEventQueue.Push(p_spNewEvent);
	}

	bool EventManager::TriggerEventNow(KE::Event_SmartPtr p_Event)
	{
		assert(p_Event->GetType() != KE::Invalid_EventType);
		CommandListenersMap::iterator it = m_Listeners.find(p_Event->GetType());

		if (it == m_Listeners.end())
			return false;

		const ListenerMap & listeners = it->second;
		bool handled_event(false);
		for (auto & listener : listeners)
		{
			listener.second(p_Event);
			handled_event = true;
		}
		return handled_event;
	}

	bool EventManager::RemoveEvent(const EventType p_EventType, const bool p_RemoveAllSame)
	{
		bool remove_success(false);
		KE::CommandListenersMap::iterator listeners_it = m_Listeners.find(p_EventType);
		if (listeners_it == m_Listeners.end())
			return false;

		std::list<KE::Event_SmartPtr>::iterator it = m_EventQueue.begin();
		while (it != m_EventQueue.end())
		{
			auto it_for_delete = it; ++it; // erase() invalidates iterator. Make a copy and iterate original first.

			if (p_EventType != (*it_for_delete)->GetType()) // different type, move on to next.
				continue;
			
			m_EventQueue.erase(it_for_delete); // type match, so remove.
			remove_success = true;

			if (!p_RemoveAllSame)
				break;
		}
		return remove_success;
	}

	bool EventManager::Update(const KE::Duration p_Duration, const KE::Duration p_DurationLimit)
	{
		bool has_duraton_limit = true;
		if (p_DurationLimit == KE::Duration::Zero)
			has_duraton_limit = false;
		KE::Duration elapsed;
		KE::Timer timer; timer.Restart();

		KE::Event_SmartPtr moving_event_ptr;
		while (m_ThreadSafeEventQueue.Poll(moving_event_ptr))
		{
			m_EventQueue.push_back(moving_event_ptr);
		}

		KE::Event_SmartPtr event_ptr;
		while (!m_EventQueue.empty())
		{
			elapsed += KE::Duration::Microseconds(timer.ElapsedAsMicroseconds()); timer.Restart();
			if (elapsed >= p_Duration && has_duraton_limit) // if elapsed time if duration limit setted.
				break;

			event_ptr = m_EventQueue.front(); m_EventQueue.pop_front();

			auto listeners_it = m_Listeners.find(event_ptr->GetType());
			if (listeners_it == m_Listeners.end()) // no listeners registered for this type.
				continue;

			KE::ListenerMap & list = listeners_it->second;
			for (auto & listener : list)
				listener.second(event_ptr);	// call delegate
		}

		if (m_ThreadSafeEventQueue.Empty())
			return true;
		return false;
	}


namespace Event
{
	KE::EventManager * s_pEventManager = nullptr;

	KE::EventManager & Get(void)
	{
		assert(s_pEventManager != nullptr);
		return *s_pEventManager;
	}

} // Event ns

} // KE ns
