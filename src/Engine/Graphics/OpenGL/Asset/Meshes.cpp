#include "Meshes.hpp"

namespace KG
{
	Meshes::Meshes(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
	{}

	const bool Meshes::LoadMeshes(const std::string & p_rPath)
	{
		this->Clear();
		bool result(false);
		Assimp::Importer importer;

		const aiScene * scene(nullptr);

		// have to flip winding for cits models.
		const std::size_t index = p_rPath.find_last_of('x');
		const std::size_t index2 = p_rPath.find_first_of("model");
		if (index != std::string::npos
			&& index != 0 // not first character.
			&& index2 == 0 // starts with "model"
			&& p_rPath.at(index-1) == '.'
		)// flip winding for CITS .x file. (normally .x has CW winding, but the CITS ones have CCW.
			scene = importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipWindingOrder);
		else
			scene = importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene)
		{
			result = InitFromScene(scene, p_rPath);
		}
		else
		{
			KE::Debug::print
			(
				KE::Debug::DBG_ERROR, "Meshes LoadMeshes scene import failed. Importer error string = "
				+ std::string(importer.GetErrorString())
			);
			result = false;
		}

		return result;
	}

	void Meshes::ClearTextures(void)
	{
		m_Textures.clear();
	}

	void Meshes::Clear(void)
	{
		m_ChildSceneNodeList.clear();
		this->ClearTextures();
	}


	const bool Meshes::InitFromScene(const aiScene * const p_pScene, const std::string & p_rPath)
	{
		m_ChildSceneNodeList.resize(p_pScene->mNumMeshes);
		m_Textures.resize(p_pScene->mNumMaterials);

		// initialize mesh one by one.
		for (unsigned i = 0; i < m_ChildSceneNodeList.size(); ++i)
		{

		}
		return false;
	}

} // KG ns
