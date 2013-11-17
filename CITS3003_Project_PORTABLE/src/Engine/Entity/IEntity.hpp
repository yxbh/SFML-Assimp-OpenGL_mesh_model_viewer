#pragma once
#include <climits>
#include "EntityID.hpp"

namespace KE
{

	/** \class Entity
	*/
	class IEntity
	{
	public:
		enum class type { ClientOnly, ServerOnly, ClientAndServer }; // ?

	private:
		static EntityID s_LatestUnusedID;
		
		EntityID m_ID;

	public:
		/*! return an unique integer value. (well it overflows after ULLONG_MAX. */
		static EntityID ProduceID(void) { return s_LatestUnusedID++; }
		

		/*! Ctor. Construct an Entity object with an ID automatically assigned. */
		IEntity(void) : m_ID(ProduceID()) {}
		virtual ~IEntity(void)
		{
			KE::EntityIDGenerator::Remove(m_ID);
		}

		/*! Ctor. Construct an Entity with a given ID. */
		explicit IEntity(const EntityID p_ID) : m_ID(p_ID) {}

		/*! Return this object's ID. */
		EntityID ID(void) const { return m_ID; }
		/*! Return this object's ID. */
		EntityID GetID(void) const { return m_ID; }
		/*! Set this object's ID to p_ID. */
		void SetID(const EntityID p_ID)
		{
			KE::EntityIDGenerator::Remove(m_ID);
			m_ID = p_ID;
		}
	
	}; // Entity class

} // KE ns
