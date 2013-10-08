#pragma once
#include "IEntityComponent.hpp"
#include "../../Utility/Time.hpp"
#include "../../Utility/dbg.hpp"

namespace KE
{
	class Entity;

	/** \class EntityPhysicsComponent
		The physics component of an Entity in the physics world.
	*/
	class PhysicsComponent
		: public IEntityComponent
	{
	private:

	public:
		PhysicsComponent(KE::Entity_SmartPtr p_spEntity);
		virtual void Update(const KE::Duration p_ElapsedTime) override;

	}; // EntityPhysicsComponent class

} // KE ns
