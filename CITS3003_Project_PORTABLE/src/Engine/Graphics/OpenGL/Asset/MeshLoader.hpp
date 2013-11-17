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
	class Skeleton;
	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;
	class BoneNode;
	typedef std::shared_ptr<KG::BoneNode> BoneNode_SmartPtr;
	typedef std::weak_ptr<KG::BoneNode> BoneNode_WeakPtr;


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
		Mesh_SmartPtr InitMesh(const aiMesh * const p_pAiMesh, const std::string & p_rPath);
		/*! initialize position vertices. */
		void InitPositions(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! initialize vertex indices */
		void InitFaces(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! initialize normals. */
		void InitNormals(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! initialize texture coordinates. */
		void InitTexCoords(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! initialize color vertices. */
		void InitColors(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! initialize materials. */
		void InitMaterials(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! */
		const bool InitMaterialTexture(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh, const aiScene * const p_pAiScene, const std::string & p_Path);
		/*! */
		void InitSkeleton(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh);
		/*! */
		void InitAnimations(Mesh_SmartPtr p_spMesh);
		/*! Construct the Skeleton (bone tree) and recover any bones that are not specified in the AiMesh because it does not have vertex influences. */
		void ConstructSkeleton(KG::Skeleton_SmartPtr p_spSkeleton, const aiMesh * const p_pAiMesh, const aiNode * const p_AiNode);
		/*! recursively find a bone with specified name and compute it's depth. */
		const bool FindBoneDepth(unsigned & p_Depth, const aiNode * const p_pAiNode, const std::string & p_BoneName);
		const aiNode * const FindAiNodeByName(const std::string & p_rNodeName, const aiNode * const p_pAiNode);
		void GrowBoneTree(KG::Skeleton_SmartPtr p_spSkeleton, KG::BoneNode_SmartPtr p_spBoneNode, const aiMesh * const p_pAiMesh, const aiNode * const p_pAiNode);
		
		/*! Compute the inverse bind pose matrix recursively for a given aiNode, given the aiNode is also a bone node in the actual model heirachy. */
		const glm::dmat4 CalculateBoneOffset(const aiNode * const p_pAiNode, glm::dmat4 p_Offset = glm::dmat4());
		void AiMatToGLMMat(const aiMatrix4x4 & p_rAiMat, glm::dmat4 & p_rGLMMat);
		const glm::dmat4 AiMatToGLMMat(const aiMatrix4x4 & p_rAiMat);
	};

} // KG ns
