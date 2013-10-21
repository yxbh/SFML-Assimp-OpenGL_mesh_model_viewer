#pragma once
#include "../../../common.hpp"
#include "Mesh.hpp"

namespace KG
{
	class Texture;
	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;
	typedef std::vector<KG::Texture_SmartPtr> TextureList;


	/*! \class Meshes

		A simple container containing some Meshes in a tree structure.

	*/
	class Meshes
		: KG::SceneNode
	{
		friend class MeshLoader;

	private:
		TextureList		m_Textures;

	public:
		Meshes(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::Null);
	
		const bool LoadMeshes(const std::string & p_rPath);
		void ClearTextures(void);
		/*! Clear all textures and child meshes. */
		void Clear(void);

	private:
		const bool InitFromScene(const aiScene * const p_pScene, const std::string & p_rPath);
	};

	typedef std::shared_ptr<KG::Meshes> Meshes_SmartPtr;
	typedef std::weak_ptr<KG::Meshes> Meshes_WeakPtr;

} // KG ns

