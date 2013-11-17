#pragma once
#include "../common.hpp"
#include "../Message/IMessageHandler.hpp"
#include "../SystemEvent/ISystemEventHandler.hpp"

namespace KE
{

	typedef std::array<bool, 150>	KeyboardKeyStates;

	/*! \class MouseStates
	*/
	struct MouseStates
	{
		int		x, y, delta_x, delta_y;
		bool	LButton, RButton, MButton;
		bool	moved;
		MouseStates(void) : x(0), y(0), delta_x(0), delta_y(0), LButton(false), RButton(false), moved(false) {}
	};

	/*! \class SFML2_InputManager
	*/
	class SFML2_InputManager
		: public KE::ISystemEventHandler
		, public KE::IMessageHandler
	{
	private:
		KE::RenderDevice *		m_pAppWindow;
		KE::KeyboardKeyStates	m_KeyStates;
		KE::MouseStates			m_MouseStates;

		bool					m_ApplicationWindowInFocus;

	public:
		static const bool KeyPressed(const KE::Keyboard::Key p_Key);
		static SFML2_InputManager & GetInputManager(void);

		SFML2_InputManager(void);

		void RegisterAppWindow(KE::RenderDevice * const p_pAppWindow);

		void Init(void);
		void Update(const KE::Duration p_Duration);

		const KE::KeyboardKeyStates & GetKeyStates(void) const;
		const KE::MouseStates & GetMouseStates(void) const;

		void ResetCurosorToCenter(void);

	private:
		virtual void ProcessEvents(void) final;
		virtual void ProcessMessages(void) final;

		void UpdateKeyStates(void);
		void UpdateMouseStates(void);

		const KE::Message TranslateMouseEventToMessage(const KE::SystemEvent & p_rEvent);
		void TranslateSystemEventToEventAndQueue(const KE::SystemEvent & p_rSysEvent) const;

	}; // SFML2_InputManager class

	typedef SFML2_InputManager InputManager;

namespace Input
{
	extern SFML2_InputManager * s_pInputManager;
	extern const InputManager & Get(void);
	extern const bool KeyPressed(const KE::Keyboard::Key p_Key);
}

} // KE ns
