#pragma once
#include "../common.hpp"

namespace KE
{

	/*! \class EventType
	*/
	typedef unsigned long EventType;
	static const EventType Invalid_EventType = 0;

	/*! \class Event
		Base class for a Command object.
		Each derived class needs to define their own unique static EventType.
	*/
	class IEvent
	{
	public:
		static const EventType sk_EventType = Invalid_EventType;

	private:
		double m_TimeStamp;

	public:
		explicit IEvent(const double p_TimeStamp = 0.0);
		virtual ~IEvent(void);

		const double GetTimeStamp(void) const;
		virtual const KE::EventType GetType(void) const;

		//void operator =(KE::IEvent & p_Event);
	};

	typedef std::shared_ptr<KE::IEvent>					Event_SmartPtr;
	typedef std::function<bool(Event_SmartPtr)>			EventDelegate;


	class TestEvent
		: public KE::IEvent
	{
	public:
	private:
		static const EventType sk_EventType = 66666666;
	public:
		virtual const KE::EventType GetType(void) const
		{
			return sk_EventType;
		}

	};

} // KE ns
