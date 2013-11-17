#pragma once
#include "Message.hpp"
#include "../Core/Queues.hpp"

namespace KE
{
	typedef KE::ThreadSafeQueue<KE::Message> MessageQueue;

}
