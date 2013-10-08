#pragma once
#include "../../../common.hpp"

namespace KG
{
	class Mesh;
	typedef std::shared_ptr<KG::Mesh> Mesh_SmartPtr;

	/*! \class MeshLoader
	*/
	class MeshLoader
	{

	private:
		Assimp::Importer		m_Importer;
		const aiScene	*		m_pScene;

		std::vector<KG::Mesh_SmartPtr>	m_MeshList;

	public:
		MeshLoader(void);

		MeshLoader & Load(const std::string & p_rPath);
		std::vector<KG::Mesh_SmartPtr> GetMeshList(void) const;

	private:
		bool LoadScene(const std::string & p_rPath);

		void InitFromScene(const aiScene * p_pScene);
		Mesh_SmartPtr InitMesh(const aiMesh * const p_AiMesh);

		

	};

} // KG ns
