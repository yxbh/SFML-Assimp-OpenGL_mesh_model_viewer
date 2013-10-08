#pragma once
#include "IEvent.hpp"
#include "../Core/Queues.hpp"

namespace KE
{
	typedef KE::ThreadSafeQueue<Event_SmartPtr>	EventQueue;
	
} // KE ns
