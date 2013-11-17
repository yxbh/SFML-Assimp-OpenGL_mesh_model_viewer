#pragma once
#include "../common.hpp"
#include "EntityID.hpp"

namespace KE
{
	class Entity;
	typedef std::shared_ptr<KE::Entity> Entity_SmartPtr;

	/** class \class EntityManager
		Manages lifetime of Entities including their components.
	*/
	class EntityManager
	{
	private:
		std::map<KE::EntityID, Entity_SmartPtr> m_EntityCollection;

	public:
		EntityManager(void);
		~EntityManager(void);

		/*! */
		void UpdateAll(const KE::Duration p_ElapsedTime);
		void UpdateCommands(const KE::Duration p_ElapsedTime);

		/*! Add entity into collection. */
		void Add(Entity_SmartPtr p_spEntity);
		/*! */
		KE::Entity_SmartPtr GetEntityByID(const KE::EntityID p_EntityID);
		/*! Remove entity with p_ID. */
		void Erase(const KE::EntityID p_EntityID);
		/*! Remove all entities in the collection. */
		void EraseAll(void);
		/*! Remove all entities in the collection. */
		void Clear(void);

	}; // EntityManager class.

	extern KE::EntityManager *	g_pEntityManager;
	extern KE::EntityManager & GetEntityManager(void); // TODO : remove this after CITS projects.

} // KE ns
