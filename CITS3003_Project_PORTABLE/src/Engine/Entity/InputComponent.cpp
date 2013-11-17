#include "InputComponent.hpp"

namespace KE
{
	InputComponent::InputComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::IEntityComponent(p_spEntity)
	{	}

	void InputComponent::Update(const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}

	const KE::EntityComponentID InputComponent::GetID(void) const
	{
		return s_ComponentID;
	}

} // KE ns
