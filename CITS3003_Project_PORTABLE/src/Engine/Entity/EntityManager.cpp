#include "EntityManager.hpp"
#include "Entity.hpp"

namespace KE
{
	EntityManager::EntityManager(void)
	{
		assert(g_pEntityManager == nullptr);
		g_pEntityManager = this;
	}

	EntityManager::~EntityManager(void)
	{
		this->Clear();
	}

	void EntityManager::UpdateAll(const KE::Duration p_ElapsedTime)
	{
		for ( auto & entity : m_EntityCollection)
			entity.second->UpdateAll(p_ElapsedTime);
	}

	void EntityManager::Add(Entity_SmartPtr p_spEntity)
	{
		assert(p_spEntity->GetID() != KE::Invalid_EntityID);
		auto insert_result = m_EntityCollection.insert(std::make_pair(p_spEntity->GetID(), p_spEntity));
		KE::Debug::print("EntityManager : Add(Entity), ID = " + std::to_string(p_spEntity->GetID()));
		//assert(insert_result.second);
	}

	KE::Entity_SmartPtr EntityManager::GetEntityByID(const KE::EntityID p_EntityID)
	{
		assert(p_EntityID != KE::Invalid_EntityID);
		auto result = m_EntityCollection.find(p_EntityID);
		if (result == m_EntityCollection.end())
			return Entity_SmartPtr();
		return result->second;
	}

	void EntityManager::Erase(const KE::EntityID p_EntityID)
	{
		assert(p_EntityID != KE::Invalid_EntityID);
		auto result = m_EntityCollection.find(p_EntityID);
		if (result == m_EntityCollection.end()) return; // does not exist.
		result->second->Destory();
		m_EntityCollection.erase(result);
	}

	void EntityManager::EraseAll(void)
	{
		for ( auto & master_object : m_EntityCollection)
			master_object.second->Destory();
		m_EntityCollection.clear();
	}

	void EntityManager::Clear(void)
	{
		this->EraseAll();
	}

	KE::EntityManager *	g_pEntityManager = nullptr;
	KE::EntityManager & GetEntityManager(void)
	{
		assert(g_pEntityManager != nullptr);
		return *g_pEntityManager;
	}

} // KE ns
