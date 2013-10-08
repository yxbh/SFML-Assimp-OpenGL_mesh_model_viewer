#pragma once
#include "../common.hpp"
#include "IEntity.hpp"
#include "EntityComponentID.hpp"

namespace KE
{
	class IEntityComponent;
	typedef std::shared_ptr<KE::IEntityComponent> EntityComponent_SmartPtr; // strong pointer
	typedef std::weak_ptr<KE::IEntityComponent> EntityComponent_WeakPtr;
	typedef std::map<KE::EntityComponentID, EntityComponent_SmartPtr> EntityComponentMap;
	typedef std::string EntityType;

	/** \class Entity
		A container and manager class for EntityComponents. Also an abstract representation of an entity.
		
		This class is not meant to be inhertied.

		Entity holds strong reference to it's Components and the components also hold strong references to
		the entity. There Destory() must be called otherwise the Entity cannnot be destoryed, and the components
		won't be able to destory themselves either.

		This class is modeled after GCC(4th ed)'s Actor class, and so the principles are similar.
	*/
	class Entity
		: public KE::IEntity
	{
	friend class EntityFactory;

	private:
		EntityType				m_Type;
		EntityComponentMap		m_Components;

	public:
		/*! */
		Entity(const KE::EntityID = KE::EntityIDGenerator::NewID());
		~Entity(void);

		/*! Adds a componenet. This function must be called when the Entity is passed to say EntityComponent. */
		void AddComponent(KE::EntityComponent_SmartPtr p_spEntityComponent);
		void Destory(void);

		/*! */
		virtual void UpdateAll(const KE::Duration p_ElapsedTime);
		
		const KE::EntityType GetType(void) const;
		/*! */
		template <class ComponentType>
		std::weak_ptr<ComponentType> GetComponent(const KE::EntityComponentID p_ID)
		{
			auto it = m_Components.find(p_ID);
			if (it == m_Components.end()) // does not have
				return std::weak_ptr<ComponentType>(); // return nullptr
			EntityComponent_SmartPtr component_base_handle(it->second);
			std::shared_ptr<ComponentType> component = std::static_pointer_cast<ComponentType>(component_base_handle); // cast to derived component class
			return std::weak_ptr<ComponentType>(component); // return weak pointer version.
		}


	private:
		void SetType(const KE::EntityType & p_rEntityType);
	}; // Entity class

	typedef std::shared_ptr<KE::Entity>	Entity_SmartPtr;
	typedef std::weak_ptr<KE::Entity>	Entity_WeakPtr;

} // KE ns
