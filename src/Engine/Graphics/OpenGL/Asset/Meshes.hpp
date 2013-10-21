#pragma once
#include "../../../common.hpp"
#include "Mesh.hpp"

namespace KG
{
	
	/*! \class Meshes

		A simple container containing some Meshes in a tree structure.

	*/
	class Meshes
		: KG::SceneNode
	{
		friend class MeshLoader;

	private:

	public:
		Meshes(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::Null);

		/*! Return the first Mesh smart pointer or nullptr if there isn't any. */
		Mesh_SmartPtr GetFirst(void);
		/*! This is an expensive call. It returns a copy of the internal SceneNode vector with the nodes casted to Mesh_SmartPtr.*/
		KG::Mesh_SP_List GetList(void);
		Mesh_SmartPtr At(unsigned p_Index);
		const unsigned GetNumMesh(void) const;

		void Clear(void);

	private:

	};

	typedef std::shared_ptr<KG::Meshes> Meshes_SmartPtr;
	typedef std::weak_ptr<KG::Meshes> Meshes_WeakPtr;

} // KG ns

