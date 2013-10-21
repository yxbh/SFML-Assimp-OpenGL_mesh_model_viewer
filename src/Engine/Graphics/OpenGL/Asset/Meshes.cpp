#include "Meshes.hpp"

namespace KG
{
	Meshes::Meshes(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
	{}

	Mesh_SmartPtr Meshes::GetFirst(void)
	{
		if (m_ChildSceneNodeList.at(0) == nullptr)
			return nullptr;
		else
			return std::static_pointer_cast<KG::Mesh>(m_ChildSceneNodeList.at(0));
	}

	KG::Mesh_SP_List Meshes::GetList(void)
	{
		Mesh_SP_List mesh_list;
		for (auto mesh : m_ChildSceneNodeList)
			mesh_list.push_back(std::static_pointer_cast<KG::Mesh>(mesh));
		return mesh_list;
	}

	Mesh_SmartPtr Meshes::At(unsigned p_Index)
	{
		return std::static_pointer_cast<KG::Mesh>(m_ChildSceneNodeList.at(p_Index));
	}

	const unsigned Meshes::GetNumMesh(void) const
	{
		return m_ChildSceneNodeList.size();
	}

	void Meshes::Clear(void)
	{
		m_ChildSceneNodeList.clear();
	}

} // KG ns
