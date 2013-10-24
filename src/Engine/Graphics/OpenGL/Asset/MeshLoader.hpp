#pragma once
#include "../../../common.hpp"

namespace KG
{
	class Mesh;
	typedef std::shared_ptr<KG::Mesh>	Mesh_SmartPtr;
	class Meshes;
	typedef std::shared_ptr<KG::Meshes> Meshes_SmartPtr;
	typedef std::weak_ptr<KG::Meshes>	Meshes_WeakPtr;
	class Texture;
	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;

	/*! \class MeshLoader
	*/
	class MeshLoader
	{

	private:
		Assimp::Importer		m_Importer;
		const aiScene	*		m_pScene;

		std::vector<KG::Texture_SmartPtr> m_Textures;

	public:
		MeshLoader(void);
		/*! Return nullptr if loading fails. */
		Meshes_SmartPtr Load(const std::string & p_rPath);

	private:
		bool LoadScene(const std::string & p_rPath);

		Meshes_SmartPtr LoadMeshes(const std::string & p_rPath);
		Meshes_SmartPtr InitFromScene(const aiScene * p_pScene, const std::string & p_rPath);
		Mesh_SmartPtr InitMesh(const aiMesh * const p_AiMesh);
		void ConstructSkeleton(const aiNode * const p_AiNode);
		const bool InitMaterial(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh, const aiScene * const p_pAiScene, const std::string & p_Path);
		
	};

} // KG ns
