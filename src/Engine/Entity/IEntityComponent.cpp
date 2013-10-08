#include "IEntityComponent.hpp"

namespace KE
{

	IEntityComponent::IEntityComponent(Entity_SmartPtr p_spEntity)
	{
		if (p_spEntity == nullptr) // unacceptable.
			assert(false);
		this->SetEntity(p_spEntity);
	}

	IEntityComponent::~IEntityComponent(void)
	{};

} // KE ns
