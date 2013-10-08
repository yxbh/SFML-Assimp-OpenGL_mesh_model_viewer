#include "Message.hpp"

namespace KE
{
	Message::Message(void)
		: type(Message::Type::Null)
	{
	}

	Message::Message(const KE::SystemEvent & p_rEvent)
	{
		switch (p_rEvent.type)
		{
		case Resized:
			this->type = Message::Type::Resized;
			this->size.origin_x = this->size.origin_y = 0;
			this->size.width = p_rEvent.size.width; this->size.height = p_rEvent.size.height;
			break;
		default:
			this->type = Message::Type::Null;
			break;
		}
	}

} // KE ns
