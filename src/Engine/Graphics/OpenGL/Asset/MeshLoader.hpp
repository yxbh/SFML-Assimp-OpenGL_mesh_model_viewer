#pragma once
#include "../../../common.hpp"

namespace KG
{
	class Mesh;
	typedef std::shared_ptr<KG::Mesh> Mesh_SmartPtr;
	class Meshes;
	typedef std::shared_ptr<KG::Meshes> Meshes_SmartPtr;
	typedef std::weak_ptr<KG::Meshes> Meshes_WeakPtr;
	class Texture;
	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;

	/*! \class MeshLoader
	*/
	class MeshLoader
	{

	private:
		Assimp::Importer		m_Importer;
		const aiScene	*		m_pScene;

		std::vector<KG::Mesh_SmartPtr>	m_MeshList;
		std::vector<KG::Texture_SmartPtr> m_Textures;

	public:
		MeshLoader(void);

		MeshLoader & Load(const std::string & p_rPath);
		std::vector<KG::Mesh_SmartPtr> GetMeshList(void) const;

	private:
		bool LoadScene(const std::string & p_rPath);

		Meshes_SmartPtr LoadMeshes(const std::string & p_rPath);
		void InitFromScene(const aiScene * p_pScene, const std::string & p_rPath);
		Mesh_SmartPtr InitMesh(const aiMesh * const p_AiMesh);
		const bool InitMaterial(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh, const aiScene * const p_pAiScene, const std::string & p_Path);

	};

} // KG ns
