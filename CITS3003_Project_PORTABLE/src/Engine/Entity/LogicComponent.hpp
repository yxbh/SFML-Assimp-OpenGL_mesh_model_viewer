#pragma once
#include "IEntityComponent.hpp"

namespace KE
{

	class LogicComponent
		: public KE::IEntityComponent
	{
	public:
		const static KE::EntityComponentID s_ComponentID = 3000;
	private:

	public:
		LogicComponent(KE::Entity_SmartPtr p_spEntity)
			: KE::IEntityComponent(p_spEntity)
		{}

		virtual const KE::EntityComponentID GetID(void) const
		{
			return s_ComponentID;
		}

	};

}
