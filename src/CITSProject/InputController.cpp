#include "InputController.hpp"
#include "../Engine/Input/InputManager.hpp"
#include "../Engine/Input/MouseEvents.hpp"
#include "../Engine/Input/KeyboardEvents.hpp"
#include "../Engine/Entity/Entity.hpp"
#include "../Engine/Graphics/OpenGL_Graphics.hpp"
#include "../Engine/Graphics/OpenGL/Camera.hpp"

namespace CITS
{
namespace Input
{

	InputController::InputController(KE::Entity_SmartPtr p_spEntity)
		: KE::InputComponent(p_spEntity)
		, m_ControllerMode(ControllerMode::Freeflight)
	{
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::MouseMoveDelegate, this, std::placeholders::_1)
			, KE::Event::Type::MouseMoved
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::KeyboardEventDelegate, this, std::placeholders::_1)
			, KE::Event::KeyboardEventType::KeyboardPressed
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::KeyboardEventDelegate, this, std::placeholders::_1)
			, KE::Event::KeyboardEventType::KeyboardReleased
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetFreeFlightMode
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetThirdPersonMode
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetObjectManMode
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&InputController::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetObjectMatMode
		);
	}

	InputController::~InputController(void)
	{
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), KE::Event::Type::MouseMoved);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), KE::Event::KeyboardEventType::KeyboardPressed);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), KE::Event::KeyboardEventType::KeyboardReleased);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetFreeFlightMode);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetThirdPersonMode);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetObjectManMode);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetObjectMatMode);
	}

	void InputController::Update(const KE::Duration p_Elapsed)
	{
		if (KE::Input::KeyPressed(KE::Keyboard::Key::C))
		{
			KE::Debug::print("sending test events...");
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent())); KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new KE::TestEvent()));
		}
		switch (m_ControllerMode)
		{
		case ControllerMode::Freeflight:
			HandleFreeflightModeInput(p_Elapsed);
			break;
		case ControllerMode::ObjectManipulation:
			HandleObjectTranslationModeInput(p_Elapsed);
			break;
		default:
			KE::Debug::print(KE::Debug::DBG_ERROR, "InputController : unimplemented mode.");
		}
	}

	void InputController::HandleFreeflightModeInput(const KE::Duration p_Elapsed)
	{
		static KE::Duration cumulate_elapsed(KE::Duration::Zero); cumulate_elapsed += p_Elapsed;
		const double movement_factor = 0.001 * 0.05;
		const double move_delta = movement_factor * p_Elapsed.AsMicroseconds();
		const double orient_factor = 0.001 * 0.2;
		const double orient_delta = orient_factor * p_Elapsed.AsMicroseconds();

		if (KE::Input::KeyPressed(KE::Keyboard::Key::A))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeLeft_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::D))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeRight_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::W))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeForward_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::S))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeBackward_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::LShift))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeUp_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::LControl))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::StrafeDown_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::K))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::IncreaseDistance_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::M))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::DecreaseDistance_Event(move_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Left))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::LookLeft_Event(orient_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Right))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::LookRight_Event(orient_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Up))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::LookUp_Event(orient_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Down))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::LookDown_Event(orient_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Q))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::TiltLeft_Event(orient_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::E))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::TiltRight_Event(orient_delta)));
	}

	void InputController::HandleObjectTranslationModeInput(const KE::Duration p_Elapsed)
	{
		static KE::Duration cumulate_elapsed(KE::Duration::Zero); cumulate_elapsed += p_Elapsed;
		const double movement_factor = 0.001 * 0.01;
		const double move_delta = movement_factor * p_Elapsed.AsMicroseconds();
		const double orient_factor = 0.1 * 0.5;
		const double orient_delta = orient_factor * p_Elapsed.AsMicroseconds();
		const double rotate_factor = 0.001 * 0.25;
		const double rotate_delta = rotate_factor * p_Elapsed.AsMicroseconds();

		if (KE::Input::KeyPressed(KE::Keyboard::Key::Left))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::RollRight_Event(-rotate_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Right))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::RollRight_Event(rotate_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Up))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::RollUp_Event(-rotate_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Down))
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::RollUp_Event(rotate_delta)));
		if (KE::Input::KeyPressed(KE::Keyboard::Key::X))
		{
			KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewObjectRequest_Event())); // request random.
			KE::Debug::print("InputController : New object requested.");
		}
	}

	const bool InputController::EventDelegate(KE::Event_SmartPtr p_spEvent)
	{
		switch (p_spEvent->GetType())
		{
		case CITS::EventType::SetFreeFlightMode:
			m_ControllerMode = ControllerMode::Freeflight;
			KG::Graphics::Get().GetScene().GetCamera().SetCameraMode( KG::Camera::CamType::FreeFlight );
			KE::Debug::print("InputController : Freeflight mode activated.");
			break;
		case CITS::EventType::SetThirdPersonMode:

			m_ControllerMode = ControllerMode::Freeflight;
			KG::Graphics::Get().GetScene().GetCamera().SetCameraMode( KG::Camera::CamType::ThirdPerson );
			KG::Graphics::Get().GetScene().GetCamera().SetDistanceToTarget( 10.0 );
			KE::Debug::print("InputController : ThirdPerson mode activated.");
			break;
		case CITS::EventType::SetObjectManMode:
			m_ControllerMode = ControllerMode::ObjectManipulation;
			KE::Debug::print("InputController : ObjectManipulation mode activated.");
			break;
		case CITS::EventType::SetObjectMatMode:
			m_ControllerMode = ControllerMode::Freeflight;
			KE::Debug::print("InputController : ObjectMaterial mode activated.");
			break;
		}
		return true;
	}

	const bool InputController::MouseMoveDelegate(KE::Event_SmartPtr p_spEvent)
	{
		std::shared_ptr<KE::Event::MouseMoved_Event> event_ptr
			= std::static_pointer_cast<KE::Event::MouseMoved_Event>(p_spEvent);

		double delta_x, delta_y;
		delta_x = delta_y = 0;
		if (event_ptr->GetDeltaX() != 0 || event_ptr->GetDeltaY() != 0)
		{
			delta_x = event_ptr->GetDeltaX();
			delta_y = event_ptr->GetDeltaY();
		}
		else
			return false;

		switch (m_ControllerMode)
		{
		case ControllerMode::Freeflight:
			if (KE::Input::Get().GetMouseStates().RButton)
			{
				const double orient_factor = 0.6;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::LookRight_Event(orient_factor*double(delta_x))));
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::LookUp_Event(orient_factor*double(-delta_y))));
			}
			break;
		case ControllerMode::ThirdPerson:
			if (KE::Input::Get().GetMouseStates().RButton)
			{
				const double orient_factor = 0.6;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::LookRight_Event(orient_factor*double(delta_x))));
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::LookUp_Event(orient_factor*double(-delta_y))));
			}
		case ControllerMode::ObjectManipulation:
			if (KE::Input::Get().GetMouseStates().LButton)
			{
				const double movement_factor = 0.1;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::StrafeRightRelCam_Event(movement_factor*(double)delta_x)));
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::StrafeForeRelCam_Event(movement_factor*(double)-delta_y)));
			}
			if (KE::Input::Get().GetMouseStates().MButton)
			{
				const double movement_factor = 0.1;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::TranslateDeltaY_Event(movement_factor*(double)-delta_y)));
			}
			if (KE::Input::Get().GetMouseStates().RButton)
			{
				const double movement_factor = 0.05;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::UniformScale_Event(movement_factor*(double)-delta_y)));
			}
			if (KE::Input::KeyPressed(KE::Keyboard::Key::LShift))
			{
				const double rotate_factor = 4.0;
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::RollUp_Event(rotate_factor*delta_y)));
				KE::Event::Get()
					.QueueEvent(KE::Event_SmartPtr(new CITS::RollRight_Event(rotate_factor*delta_x)));
			}
			break;
		default:
			KE::Debug::print(KE::Debug::DBG_ERROR, "InputController : unimplemented mode.");
		}
		return true;
	}

	const bool InputController::KeyboardEventDelegate(KE::Event_SmartPtr p_spEvent)
	{
		std::shared_ptr<KE::Event::Key_Event> event_ptr
			= std::static_pointer_cast<KE::Event::Key_Event>(p_spEvent);
		if (!event_ptr)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "InputController : KeyboardEventDelegate failed.");
			return false;
		}

		if (event_ptr->GetType() == KE::Event::KeyboardReleased)
		{
			switch (event_ptr->GetKey())
			{
			case KE::Keyboard::Key::F1:
				m_ControllerMode = ControllerMode::Freeflight;
				KE::Debug::print("InputController : Freeflight mode activated.");
				break;
			case KE::Keyboard::Key::F2:
				m_ControllerMode = ControllerMode::ObjectManipulation;
				KE::Debug::print("InputController : ObjectManipulation mode activated.");
				break;
			case KE::Keyboard::Key::F3:
				m_ControllerMode = ControllerMode::ObjectMaterial;
				KE::Debug::print("InputController : ObjectMaterial mode activated.");
				break;
			case KE::Keyboard::Key::Z:
				if (m_ControllerMode == ControllerMode::ObjectManipulation)
				{
					KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewObjectRequest_Event())); // request random.
					KE::Debug::print("InputController : New object requested.");
				}
				break;
			case KE::Keyboard::Key::Tab:
				KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::ToggleControlPanelVisibility_Event())); // request random.
				break;
			default:
				break;
			}
		}
		return true;
	}

}
}