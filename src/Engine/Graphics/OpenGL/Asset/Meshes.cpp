#include "Meshes.hpp"
#include "Skeleton.hpp"

namespace KG
{

	Meshes::Meshes(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: KG::Mesh(p_EntityID, p_RenderPass)
		, m_spAnimSkeleton(nullptr)
		, m_HasSkeleton(false)
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

	const bool Meshes::BufferAll(void)
	{
		bool result = true;
		for (auto node_ptr : m_ChildSceneNodeList)
		{
			KG::Mesh_SmartPtr mesh_ptr(std::static_pointer_cast<KG::Mesh>(node_ptr));
			if (mesh_ptr) // make sure ptr is valid post-cast
			{
				if (!mesh_ptr->Loaded())
					result = false;
				else
					mesh_ptr->BufferAll();
			}			
		}
		return result;
	}

	void Meshes::Clear(void)
	{
		m_ChildSceneNodeList.clear();
	}

	void Meshes::SetTexture(KG::Texture_SmartPtr p_spTexture)
	{
		for (auto node_ptr : m_ChildSceneNodeList)
		{
			KG::Mesh_SmartPtr mesh_ptr(std::static_pointer_cast<KG::Mesh>(node_ptr));
			if (mesh_ptr) // make sure ptr is valid post-cast
				mesh_ptr->SetTexture(p_spTexture);
		}
	}

	void Meshes::SetSkeleton(KG::Skeleton_SmartPtr p_spSkeleton)
	{
		if (this->HasSkeleton())
			KE::Debug::print(KE::Debug::DBG_WARNING, "Meshes::SetSkeleton : already contains Skeleton. Go ahead anyway.");
		m_spAnimSkeleton = p_spSkeleton;
		m_HasSkeleton = true;
	}

	const bool Meshes::HasSkeleton(void) const
	{
		return m_HasSkeleton;
	}

} // KG ns
