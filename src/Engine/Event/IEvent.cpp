#include "IEvent.hpp"

namespace KE
{
	IEvent::IEvent(const double p_TimeStamp)
		: m_TimeStamp(p_TimeStamp)
	{}

	IEvent::~IEvent(void)
	{}

	const double IEvent::GetTimeStamp(void) const
	{
		return m_TimeStamp;
	}

	const KE::EventType IEvent::GetType(void) const
	{
		return sk_EventType;
	}

} // KE ns