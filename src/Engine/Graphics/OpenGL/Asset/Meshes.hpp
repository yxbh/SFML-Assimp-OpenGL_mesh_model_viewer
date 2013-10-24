#pragma once
#include "../../../common.hpp"
#include "Mesh.hpp"
#include "Bone.hpp"

namespace KG
{
	
	/*! \class Meshes

		A simple container containing some Meshes in a tree structure.

	*/
	class Meshes
		: public KG::Mesh
	{
		friend class MeshLoader;

	private:
		/*! animation related. */
		glm::dmat4				m_GlobalInverse;
		KG::Skeleton_SmartPtr	m_spAnimSkeleton;		// animaition skeleton.
		bool					m_HasSkeleton;

	public:
		Meshes(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered);
		virtual ~Meshes(void) {}

		/*! Return the first Mesh smart pointer or nullptr if there isn't any. */
		Mesh_SmartPtr GetFirst(void);
		/*! This is an expensive call. It returns a copy of the internal SceneNode vector with the nodes casted to Mesh_SmartPtr.*/
		KG::Mesh_SP_List GetList(void);
		Mesh_SmartPtr At(unsigned p_Index);
		const unsigned GetNumMesh(void) const;
		/*! buffer all Meshes onto the GPU. */
		virtual const bool BufferAll(void);

		void Clear(void);

		virtual void SetTexture(KG::Texture_SmartPtr p_spTexture) override;
		void SetSkeleton(KG::Skeleton_SmartPtr p_spSkeleton);

		const bool HasSkeleton(void) const;

	private:

	};

	typedef std::shared_ptr<KG::Meshes> Meshes_SmartPtr;
	typedef std::weak_ptr<KG::Meshes> Meshes_WeakPtr;

} // KG ns

