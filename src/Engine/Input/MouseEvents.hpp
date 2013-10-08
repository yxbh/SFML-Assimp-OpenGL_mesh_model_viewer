#pragma once
#include "../common.hpp"
#include "../Event/IEvent.hpp"

namespace KE
{
namespace Event
{

	enum Type
	{
		MouseMoved = 201
	};

	class MouseMoved_Event
		: public KE::IEvent
	{
	public:
		static const KE::EventType sk_EventType = KE::Event::Type::MouseMoved;

	private:
		int m_PosX, m_PosY;
		int m_DeltaX, m_DeltaY;

	public:
		MouseMoved_Event(const int p_PosX, const int p_PosY, const int p_DeltaX, const int p_DeltaY)
			: m_PosX(p_PosX), m_PosY(p_PosY), m_DeltaX(p_DeltaX), m_DeltaY(p_DeltaY)
		{}

		const KE::EventType GetType(void) const
		{
			return sk_EventType;
		}

		const int GetPosX(void) const
		{
			return m_PosX;
		}

		const int GetPosY(void) const
		{
			return m_PosY;
		}

		const int GetDeltaX(void) const
		{
			return m_DeltaX;
		}

		const int GetDeltaY(void) const
		{
			return m_DeltaY;
		}
	};

}
} // KE ns
