#include "Scene.hpp"
#include "SceneNode.hpp"

namespace KG
{
	Scene::Scene(void)
		: m_spRootNode(nullptr)
		, m_spCamera(nullptr)
		, m_spRenderer(nullptr)
	{
		this->InitDefault();
	}

	Scene::~Scene(void)
	{
	}

	void Scene::InitDefault(void)
	{
		m_spCamera = Camera_SmartPtr(new KG::Camera());
		this->SetRootSceneNode(KG::RootSceneNode_SmartPtr(new KG::RootSceneNode()));
	}

	void Scene::UpdateAll(const KE::Duration p_Elapsed)
	{
		if (m_spRootNode != nullptr)
			m_spRootNode->UpdateAll(*this, p_Elapsed);
	}
	
	void Scene::RenderAll(void)
	{
		if (SceneReady())
			m_spRootNode->RenderAll(*this);
	}

	bool Scene::SceneReady(void) const
	{
		return (m_spRootNode != nullptr) && (m_spCamera != nullptr) && (m_spRenderer != nullptr);
	}

	bool Scene::AddSceneNode(SceneNode_SmartPtr p_spNewSceneNode)
	{
		if (p_spNewSceneNode == nullptr) return false;
		if (p_spNewSceneNode->GetEntityID() == KE::Invalid_EntityID)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Scene : adding invalid node with invalid id.");
			return false;
		}
		
		m_SceneNodeMap.insert(std::make_pair(p_spNewSceneNode->GetEntityID(), p_spNewSceneNode));
		
		if (m_spRootNode == nullptr)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Scene : m_RootNode is null.");
			return false;
		}
		m_spRootNode->AddChild(p_spNewSceneNode); return true;
	}

	bool Scene::RemoveSceneNode(const KE::EntityID p_ActorID)
	{
		unsigned num_erased = m_SceneNodeMap.erase(p_ActorID);
		if (num_erased == 0) return false;
		else return m_spRootNode->RecursiveRemove(p_ActorID);
	}

	void Scene::SetCamera(KG::Camera_SmartPtr p_spNewCamera)
	{
		m_spCamera = p_spNewCamera;
	}

	void Scene::SetRenderer(KG::Renderer_SmartPtr p_pRenderer)
	{
		m_spRenderer = p_pRenderer;
	}

	void Scene::SetRootSceneNode(KG::RootSceneNode_SmartPtr p_spRootSceneNode)
	{
		if (m_spRootNode != nullptr)
			KE::Debug::print(KE::Debug::DBG_WARNING, "Scene : a RootSceneNode already exists.");
		m_spRootNode = p_spRootSceneNode;
	}

	KG::SceneNode & Scene::GetRootSceneNode(void)
	{
		return *m_spRootNode.get();
	}

	KG::MatrixStack & Scene::GetMatrixStack(void)
	{
		return m_MatrixStack;
	}

	KG::Renderer & Scene::GetRenderer(void)
	{
		assert(m_spRenderer != nullptr);
		return *m_spRenderer;
	}

	KG::Camera & Scene::GetCamera(void)
	{
		assert(m_spCamera != nullptr);
		return *m_spCamera;
	}


} // KG ns
