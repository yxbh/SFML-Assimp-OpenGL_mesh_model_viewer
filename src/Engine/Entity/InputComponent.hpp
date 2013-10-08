#pragma once
#include "IEntityComponent.hpp"

namespace KE
{
	/*! \class InputComponent
		A base class for an entity's input component.
		The component it self is an Entity.
	*/
	class InputComponent
		: public KE::IEntityComponent
	{
	private:
		static const KE::EntityComponentID s_ComponentID = 2000;
	public:
		InputComponent(KE::Entity_SmartPtr p_spEntity);
		virtual void Update(const KE::Duration p_ElapsedTime) override;
		virtual const KE::EntityComponentID GetID(void) const override;
	};

} // KE ns
