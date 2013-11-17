#pragma once
#include "../../common.hpp"
#include "../../Event/IEvent.hpp"

namespace KG
{
namespace Event
{

	enum Type
	{
		Null			= 100,
		Resize			= 101,
		
	};

	class WindowResize_Event
		: public KE::IEvent
	{
	private:
		static const KE::EventType sk_EventType = KG::Event::Type::Resize;
		glm::ivec2	m_Size; // x = width, y = height,

	public:
		WindowResize_Event(const int p_Width, const int p_Height)
			: m_Size(p_Width, p_Height)
		{}

		const glm::ivec2 GetSize(void) const
		{
			return m_Size;
		}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventType;
		}

	};

}
} // KG ns
