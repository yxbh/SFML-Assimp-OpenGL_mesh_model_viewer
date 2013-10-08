#include "PhysicsComponent.hpp"

namespace KE
{
	PhysicsComponent::PhysicsComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::IEntityComponent(p_spEntity)
	{}

	void PhysicsComponent::Update(KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}

}
