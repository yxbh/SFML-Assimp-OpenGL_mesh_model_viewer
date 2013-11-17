#pragma once
#include "../common.hpp"
#include "../Event/IEvent.hpp"

namespace KE
{
namespace Event
{
	enum KeyboardEventType
	{
		KeyboardPressed		= 204,
		KeyboardReleased	= 205,
	};

	/*! \class Key_Event
	*/
	class Key_Event
		: public KE::IEvent
	{
	public:
		static const KE::EventType sk_EventType = 200;
	protected:
		KE::SystemEvent		m_KeyboardEvent;


	public:
		Key_Event(const KE::SystemEvent & p_rSysEvent)
			: m_KeyboardEvent(p_rSysEvent)
		{	}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventType;
		}

		const KE::Keyboard::Key GetKey(void) const
		{
			return m_KeyboardEvent.key.code;
		}

		const bool HasAlt(void) const
		{
			return m_KeyboardEvent.key.alt;
		}

		const bool HasControl(void) const
		{
			return m_KeyboardEvent.key.control;
		}

		const bool HasShift(void) const
		{
			return m_KeyboardEvent.key.shift;
		}

		const bool HasSystem(void) const
		{
			return m_KeyboardEvent.key.system;
		}

	};

	/*! \class KeyboardPressed_Event
	*/
	class KeyboardPressed_Event
		: public Key_Event
	{
	public:
		static const KE::EventType sk_EventType = KeyboardPressed;

	public:
		KeyboardPressed_Event(const KE::SystemEvent & p_rSysEvent)
			: Key_Event(p_rSysEvent)
		{	}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventType;
		}
	};

	/*! \class KeyboardReleased_Event
	*/
	class KeyboardReleased_Event
		: public Key_Event
	{
	public:
		static const KE::EventType sk_EventType = KeyboardReleased;

	public:
		KeyboardReleased_Event(const KE::SystemEvent & p_rSysEvent)
			: Key_Event(p_rSysEvent)
		{	}

		virtual const KE::EventType GetType(void) const override
		{
			return sk_EventType;
		}
	};

} // Event
} // KE
