#include "Entity.hpp"
#include "IEntityComponent.hpp"

namespace KE
{

	Entity::Entity(const KE::EntityID p_ID)
		: IEntity(p_ID)
	{}

	Entity::~Entity(void)
	{
		assert(m_Components.empty());
	}

	void Entity::AddComponent(KE::EntityComponent_SmartPtr p_spEntityComponent)
	{
		assert(p_spEntityComponent != nullptr); // should not be null.
		assert(p_spEntityComponent->GetID() != KE::Invalid_EntityComponentID);	//
		auto result = m_Components.insert(std::make_pair(p_spEntityComponent->GetID(), p_spEntityComponent));
		assert(result.second); // fails if insertion failed.
	}

	void Entity::Destory(void)
	{
		m_Components.clear();
	}

	void Entity::UpdateAll(const KE::Duration p_ElapsedTime)
	{
		for (auto & it : m_Components)
			it.second->Update(p_ElapsedTime);
	}

	const KE::EntityType Entity::GetType(void) const
	{
		return m_Type;
	}

	void Entity::SetType(const KE::EntityType & p_rEntityType)
	{
		m_Type = p_rEntityType;
	}


} // KE ns
