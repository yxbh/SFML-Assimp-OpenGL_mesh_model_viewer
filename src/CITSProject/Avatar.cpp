#include "Avatar.hpp"
#include "../Engine/Input/InputManager.hpp"
#include "../Engine/Entity/Entity.hpp"
#include "../Engine/Event/EventManager.hpp"
#include "CITSEvents.hpp"

namespace CITS
{
namespace Avatar
{
	InputComponent::InputComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::InputComponent(p_spEntity)
	{
		this->Init();
	}

	InputComponent::~InputComponent(void)
	{
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::TranslateUp);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::TranslateDown);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::StrafeForward);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::StrafeBackward);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::StrafeLeft);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::StrafeRight);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::LookUp);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::LookDown);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::LookLeft);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::LookRight);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::TiltLeft);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::TiltRight);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::IncreaseDistance);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::DecreaseDistance);
	}

	void InputComponent::Init(void)
	{
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::TranslateUp);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::TranslateDown);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeForward);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeBackward);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeLeft);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeRight);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeUp);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::StrafeDown);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::LookUp);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::LookDown);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::LookLeft);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::LookRight);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::TiltLeft);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::TiltRight);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::IncreaseDistance);
		KE::Event::Get().AddListener(this->GetEntity()->GetID()
			, std::bind(&InputComponent::MovementDelegate, this, std::placeholders::_1), CITS::DecreaseDistance);
	}

	void InputComponent::Update(const KE::Duration p_Elapsed)
	{
		KE_UNREFERENCED_PARAMETER(p_Elapsed);
	}

	bool InputComponent::MovementDelegate(KE::Event_SmartPtr p_spEvent)
	{
		// get ptr to camera.
		auto render_weak_ptr = this->GetEntity()->GetComponent<KE::RenderComponent>(1000);
		auto render_component = KE::Utility::MakeStrongPtr(render_weak_ptr);
		if (!render_component)
		{
			KE::Debug::print(KE::Debug::msg_type::DBG_ERROR, "aa aa");
			return false;
		}
		KG::SceneNode_SmartPtr scenenode_ptr = render_component->GetSceneNode();
		if (!scenenode_ptr)
		{
			KE::Debug::print(KE::Debug::msg_type::DBG_ERROR, "bb bb");
			return false;
		}
		auto camera_ptr = std::static_pointer_cast<KG::Camera>(scenenode_ptr);

		std::shared_ptr<CITS::Control_Event> event_ptr
			= std::static_pointer_cast<CITS::Control_Event>(p_spEvent);
		const double delta = event_ptr->GetDelta();
		switch (p_spEvent->GetType())
		{
		case CITS::EventType::StrafeForward:
			camera_ptr->StrafeForward(delta);
			break;
		case CITS::EventType::StrafeBackward:
			camera_ptr->StrafeBackward(delta);
			break;
		case CITS::EventType::StrafeLeft:
			camera_ptr->StrafeLeft(delta);
			break;
		case CITS::EventType::StrafeRight:
			camera_ptr->StrafeRight(delta);
			break;
		case CITS::EventType::StrafeUp:
			camera_ptr->StrafeUp(delta);
			break;
		case CITS::EventType::StrafeDown:
			camera_ptr->StrafeDown(delta);
			break;
		case CITS::EventType::TranslateUp:
			camera_ptr->OffsetPosition(0.0, delta, 0.0);
			break;
		case CITS::EventType::TranslateDown:
			camera_ptr->OffsetPosition(0.0, -delta, 0.0);
			break;
		case CITS::EventType::LookUp:
			camera_ptr->OffsetOrientation(delta, 0.0, 0.0);
			break;
		case CITS::EventType::LookDown:
			camera_ptr->OffsetOrientation(-delta, 0.0, 0.0);
			break;
		case CITS::EventType::LookLeft:
			camera_ptr->OffsetOrientation(0.0, delta, 0.0);
			break;
		case CITS::EventType::LookRight:
			camera_ptr->OffsetOrientation(0.0, -delta, 0.0);
			break;
		case CITS::EventType::TiltLeft:
			camera_ptr->OffsetOrientation(0.0, 0.0, delta);
			break;
		case CITS::EventType::TiltRight:
			camera_ptr->OffsetOrientation(0.0, 0.0, -delta);
			break;
		case CITS::EventType::IncreaseDistance:
			camera_ptr->OffsetTargetDistance(delta);
			break;
		case CITS::EventType::DecreaseDistance:
			camera_ptr->OffsetTargetDistance(-delta);
			break;
		}
		return true;
	}

	RenderComponent::RenderComponent(KE::Entity_SmartPtr p_spEntity)
		: KG::OGLEntityRenderComponent(p_spEntity)
	{}
	void RenderComponent::Init(void)
	{}

	void RenderComponent::Update(const KE::Duration p_Elapsed)
	{
		KE_UNREFERENCED_PARAMETER(p_Elapsed);

	}



	LogicComponent::LogicComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::LogicComponent(p_spEntity)
	{}

	void LogicComponent::Update(const KE::Duration p_Elapsed)
	{
		KE_UNREFERENCED_PARAMETER(p_Elapsed);
	}

}
}