#include "InputManager.hpp"
#include "../Event/EventManager.hpp"
#include "MouseEvents.hpp"
#include "KeyboardEvents.hpp"

namespace KE
{
	SFML2_InputManager * KE::Input::s_pInputManager = nullptr;

	const bool SFML2_InputManager::KeyPressed(const KE::Keyboard::Key p_Key)
	{
		assert(KE::Input::s_pInputManager != nullptr);
		if (KE::Input::s_pInputManager->m_ApplicationWindowInFocus)
			return KE::Input::s_pInputManager->GetKeyStates().at(p_Key);
		return false;
	}

	SFML2_InputManager & SFML2_InputManager::GetInputManager(void)
	{
		assert(KE::Input::s_pInputManager != nullptr);
		return *KE::Input::s_pInputManager;
	}


	SFML2_InputManager::SFML2_InputManager(void)
		: m_ApplicationWindowInFocus(true)
		, m_pAppWindow(nullptr)
	{}

	void SFML2_InputManager::RegisterAppWindow(KE::RenderDevice * const p_pAppWindow)
	{
		m_pAppWindow = p_pAppWindow;
	}

	void SFML2_InputManager::Init(void)
	{
		m_KeyStates.fill(false);
		assert(m_pAppWindow != nullptr);
		sf::Vector2i pos =  sf::Mouse::getPosition(*m_pAppWindow);
		m_MouseStates.x = pos.x; m_MouseStates.y = pos.y;

		assert(KE::Input::s_pInputManager == nullptr);
		KE::Input::s_pInputManager = this;
	}

	void SFML2_InputManager::Update(const KE::Duration p_Duration)
	{
		KE_UNREFERENCED_PARAMETER(p_Duration);
		this->ProcessEvents(); this->ProcessMessages();

		m_KeyStates.fill(false);

		if (!m_ApplicationWindowInFocus) return;

		UpdateKeyStates(); UpdateMouseStates();
	}

	const KE::KeyboardKeyStates & SFML2_InputManager::GetKeyStates(void) const
	{
		return m_KeyStates;
	}

	const KE::MouseStates & SFML2_InputManager::GetMouseStates(void) const
	{
		return m_MouseStates;
	}

	void SFML2_InputManager::ResetCurosorToCenter(void)
	{
		sf::Mouse::setPosition(sf::Vector2i(m_pAppWindow->getSize().x/2, m_pAppWindow->getSize().y/2), *m_pAppWindow);
	}

	void SFML2_InputManager::ProcessEvents(void)
	{
		KE::SystemEvent new_event;
		while (this->GetEventInbox().Poll(new_event))
		{
			switch (new_event.type)
			{
			case KE::SystemEvent::LostFocus:
				m_ApplicationWindowInFocus = false;
				break;
			case KE::SystemEvent::GainedFocus:
				m_ApplicationWindowInFocus = true;
				break;
			default: break;
			}
			
			if (m_ApplicationWindowInFocus) // process events only if window in focus. otherwise discard.
			{
				switch (new_event.type)
				{
				case KE::SystemEvent::MouseMoved:
					{
					KE::Message msg = this->TranslateMouseEventToMessage(new_event);
					this->GetMessageOutbox().Push(msg);
					break;
					}
				case KE::SystemEvent::KeyPressed:
				case KE::SystemEvent::KeyReleased:
					this->TranslateSystemEventToEventAndQueue(new_event);
					break;
				default:break;
				}
			}
		}
	}

	void SFML2_InputManager::ProcessMessages(void)
	{
		this->EmptyMessageInbox(); // ignore and empty all for now.
	}

	void SFML2_InputManager::UpdateKeyStates(void)
	{
		m_KeyStates[KE::Keyboard::Key::A]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::A);
		m_KeyStates[KE::Keyboard::Key::B]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::B);
		m_KeyStates[KE::Keyboard::Key::C]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::C);
		m_KeyStates[KE::Keyboard::Key::D]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::D);
		m_KeyStates[KE::Keyboard::Key::E]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::E);
		m_KeyStates[KE::Keyboard::Key::D]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::D);
		m_KeyStates[KE::Keyboard::Key::E]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::E);
		m_KeyStates[KE::Keyboard::Key::F]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F);
		m_KeyStates[KE::Keyboard::Key::G]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::G);
		m_KeyStates[KE::Keyboard::Key::H]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::H);
		m_KeyStates[KE::Keyboard::Key::I]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::I);
		m_KeyStates[KE::Keyboard::Key::J]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::J);
		m_KeyStates[KE::Keyboard::Key::K]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::K);
		m_KeyStates[KE::Keyboard::Key::L]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::L);
		m_KeyStates[KE::Keyboard::Key::M]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::M);
		m_KeyStates[KE::Keyboard::Key::N]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::N);
		m_KeyStates[KE::Keyboard::Key::O]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::O);
		m_KeyStates[KE::Keyboard::Key::P]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::P);
		m_KeyStates[KE::Keyboard::Key::Q]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Q);
		m_KeyStates[KE::Keyboard::Key::R]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::R);
		m_KeyStates[KE::Keyboard::Key::S]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::S);
		m_KeyStates[KE::Keyboard::Key::T]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::T);
		m_KeyStates[KE::Keyboard::Key::U]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::U);
		m_KeyStates[KE::Keyboard::Key::V]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::V);
		m_KeyStates[KE::Keyboard::Key::W]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::W);
		m_KeyStates[KE::Keyboard::Key::X]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::X);
		m_KeyStates[KE::Keyboard::Key::Y]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Y);
		m_KeyStates[KE::Keyboard::Key::Z]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Z);
		m_KeyStates[KE::Keyboard::Key::Num0]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num0);
		m_KeyStates[KE::Keyboard::Key::Num1]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num1);
		m_KeyStates[KE::Keyboard::Key::Num2]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num2);
		m_KeyStates[KE::Keyboard::Key::Num3]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num3);
		m_KeyStates[KE::Keyboard::Key::Num4]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num4);
		m_KeyStates[KE::Keyboard::Key::Num5]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num5);
		m_KeyStates[KE::Keyboard::Key::Num6]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num6);
		m_KeyStates[KE::Keyboard::Key::Num7]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num7);
		m_KeyStates[KE::Keyboard::Key::Num8]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num8);
		m_KeyStates[KE::Keyboard::Key::Num9]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Num9);
		m_KeyStates[KE::Keyboard::Key::Escape]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Escape);
		m_KeyStates[KE::Keyboard::Key::LControl]	= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::LControl);
		m_KeyStates[KE::Keyboard::Key::LShift]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::LShift);
		m_KeyStates[KE::Keyboard::Key::LAlt]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::LAlt);

		m_KeyStates[KE::Keyboard::Key::Tilde]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Tilde);
		m_KeyStates[KE::Keyboard::Key::Space]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Space);	
		m_KeyStates[KE::Keyboard::Key::Return]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Return);
		m_KeyStates[KE::Keyboard::Key::BackSpace]	= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::BackSpace);
		m_KeyStates[KE::Keyboard::Key::Tab]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Tab);

		m_KeyStates[KE::Keyboard::Key::Left]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Left);
		m_KeyStates[KE::Keyboard::Key::Right]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Right);
		m_KeyStates[KE::Keyboard::Key::Up]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Up);
		m_KeyStates[KE::Keyboard::Key::Down]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Down);
		m_KeyStates[KE::Keyboard::Key::Numpad0]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad0);
		m_KeyStates[KE::Keyboard::Key::Numpad1]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad1);
		m_KeyStates[KE::Keyboard::Key::Numpad2]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad2);
		m_KeyStates[KE::Keyboard::Key::Numpad3]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad3);
		m_KeyStates[KE::Keyboard::Key::Numpad4]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad4);
		m_KeyStates[KE::Keyboard::Key::Numpad5]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad5);
		m_KeyStates[KE::Keyboard::Key::Numpad6]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad6);
		m_KeyStates[KE::Keyboard::Key::Numpad7]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad7);
		m_KeyStates[KE::Keyboard::Key::Numpad8]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad8);
		m_KeyStates[KE::Keyboard::Key::Numpad9]		= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::Numpad9);
		m_KeyStates[KE::Keyboard::Key::F1]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F1);
		m_KeyStates[KE::Keyboard::Key::F2]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F2);
		m_KeyStates[KE::Keyboard::Key::F3]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F3);
		m_KeyStates[KE::Keyboard::Key::F4]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F4);
		m_KeyStates[KE::Keyboard::Key::F5]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F5);
		m_KeyStates[KE::Keyboard::Key::F6]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F6);
		m_KeyStates[KE::Keyboard::Key::F7]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F7);
		m_KeyStates[KE::Keyboard::Key::F8]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F8);
		m_KeyStates[KE::Keyboard::Key::F9]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F9);
		m_KeyStates[KE::Keyboard::Key::F10]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F10);
		m_KeyStates[KE::Keyboard::Key::F11]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F11);
		m_KeyStates[KE::Keyboard::Key::F12]			= sf::Keyboard::isKeyPressed(KE::Keyboard::Key::F12);
		
	}

	void SFML2_InputManager::UpdateMouseStates(void)
	{
		m_MouseStates.LButton = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		m_MouseStates.RButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		m_MouseStates.MButton = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(*m_pAppWindow);
		KE::MouseStates mouse_states = this->GetMouseStates();
		if (mouse_pos.x == mouse_states.x && mouse_pos.y == mouse_states.y)
			m_MouseStates.moved = false;
		else
		{
			m_MouseStates.moved = true;
			m_MouseStates.delta_x = mouse_pos.x - m_MouseStates.x;
			m_MouseStates.delta_y = mouse_pos.y - m_MouseStates.y;
			m_MouseStates.x = mouse_pos.x;
			m_MouseStates.y = mouse_pos.y;

			// send and tell EventManager to trigger the Event now.
			KE::Event::Get().TriggerEventNow
			(
				KE::Event_SmartPtr
				(
					new KE::Event::MouseMoved_Event(m_MouseStates.x, m_MouseStates.y
										, m_MouseStates.delta_x, m_MouseStates.delta_y))
			);
		}
	}

	const KE::Message SFML2_InputManager::TranslateMouseEventToMessage(const KE::SystemEvent & p_rEvent)
	{
		KE::Message new_msg; new_msg.type = KE::Message::MouseMoved;
		new_msg.mouse.x = p_rEvent.mouseMove.x;
		new_msg.mouse.y = p_rEvent.mouseMove.y;
		new_msg.mouse.delta_x = p_rEvent.mouseMove.x - m_MouseStates.x;
		new_msg.mouse.delta_y = p_rEvent.mouseMove.y - m_MouseStates.y;
		m_MouseStates.x = p_rEvent.mouseMove.x;
		m_MouseStates.y = p_rEvent.mouseMove.y;
		return new_msg;
	}

	void SFML2_InputManager::TranslateSystemEventToEventAndQueue(const KE::SystemEvent & p_rSysEvent) const
	{
		KE::Event_SmartPtr new_event;
		if (p_rSysEvent.type == KE::SystemEvent::KeyPressed)
			new_event = KE::Event_SmartPtr(new KE::Event::KeyboardPressed_Event(p_rSysEvent));
		else if (p_rSysEvent.type == KE::SystemEvent::KeyReleased)
			new_event = KE::Event_SmartPtr(new KE::Event::KeyboardReleased_Event(p_rSysEvent));
		KE::Event::Get().QueueEvent(new_event);
	}

namespace Input
{
	const SFML2_InputManager & Get(void)
	{
		assert(KE::Input::s_pInputManager != nullptr);
		return *KE::Input::s_pInputManager;
	}

	const bool KeyPressed(const KE::Keyboard::Key p_Key)
	{
		return KE::InputManager::GetInputManager().KeyPressed(p_Key);
	}

}

} // KE ns
