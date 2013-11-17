#pragma once
#include "../Engine/common.hpp"
#include "CITSEvents.hpp"
#include "../Engine/Entity/InputComponent.hpp"
#include "../Engine/Event/EventManager.hpp"

namespace CITS
{
namespace Input
{


	class InputController final
		: public KE::InputComponent
	{
	public:
		enum class ControllerMode
		{
			Freeflight,
			ThirdPerson,
			ObjectManipulation,
			ObjectMaterial,
		};

	private:
		ControllerMode	m_ControllerMode;

	public:
		InputController(KE::Entity_SmartPtr p_spEntity);
		virtual ~InputController(void);

		virtual void Update(const KE::Duration p_Elapsed) final;
		void HandleFreeflightModeInput(const KE::Duration p_Elapsed);
		void HandleObjectTranslationModeInput(const KE::Duration p_Elapsed);

		const bool EventDelegate(KE::Event_SmartPtr p_spEvent);
		const bool MouseMoveDelegate(KE::Event_SmartPtr p_spEvent);
		const bool KeyboardEventDelegate(KE::Event_SmartPtr p_spEvent);
	};

} // Input ns

} // CITS ns
