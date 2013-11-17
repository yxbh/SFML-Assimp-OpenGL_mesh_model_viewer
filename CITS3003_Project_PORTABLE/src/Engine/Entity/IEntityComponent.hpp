#pragma once
#include "../common.hpp"
#include "EntityComponentID.hpp"

namespace KE
{
	class Entity;
	typedef std::shared_ptr<KE::Entity> Entity_SmartPtr;
	typedef std::weak_ptr<KE::Entity> Entity_WeakPtr;

	/*! \class IEntityComponent
		An EntityComponent must be created with a valid Entity object. A valid Entity object must always
		be provided into the constructors. Bad things will happen if you dont!!!
	*/
	class IEntityComponent
	{
		friend class EntityFactory;
	protected:
		KE::Entity_SmartPtr			m_spEntity;  // owner

	public:
		IEntityComponent(Entity_SmartPtr p_spEntity);
		virtual ~IEntityComponent(void);

		virtual void Init(void /* path to ini file? */) {};
		virtual void PostInit(void) {};

		virtual void Update(const KE::Duration p_ElapsedTime) = 0;
		
		/*! Base class will always return invalid ID. */
		virtual const KE::EntityComponentID GetID(void) const
		{
			return KE::Invalid_EntityComponentID;
		}

		void SetEntity(KE::Entity_SmartPtr p_spEntity)
		{
			m_spEntity = p_spEntity;
		}

	protected:
		Entity_SmartPtr GetEntity(void)
		{
			return m_spEntity;
		}		

	}; // IEntityComponent class

	typedef std::shared_ptr<KE::IEntityComponent> EntityComponent_SmartPtr; // strong pointer
	typedef std::weak_ptr<KE::IEntityComponent> EntityComponent_WeakPtr;

} // KE ns
