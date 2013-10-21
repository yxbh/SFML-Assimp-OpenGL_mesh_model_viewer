#include "MeshLoader.hpp"
#include "Mesh.hpp"
#include "Meshes.hpp"
#include "TextureLoader.hpp"
#include "Texture.hpp"

namespace KG
{

	MeshLoader::MeshLoader(void)
		: m_pScene(nullptr)
	{}

	Meshes_SmartPtr MeshLoader::Load(const std::string & p_rPath)
	{
		if (!this->LoadScene(p_rPath))
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Mesh load failure.");
			return nullptr;
		}
		assert(m_pScene != nullptr);
		return InitFromScene(m_pScene, p_rPath);
	}

	bool MeshLoader::LoadScene(const std::string & p_rPath)
	{
		std::size_t index = p_rPath.find_last_of('x');
		std::size_t index2 = p_rPath.find_first_of("model");
		if (index != std::string::npos
			&& index != 0 // not first character.
			&& index2 == 0 // starts with "model"
			&& p_rPath.at(index-1) == '.'
			)// flip winding for CITS .x file. (normally .x has CW winding, but the CITS ones have CCW.
		{
			m_pScene = m_Importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipWindingOrder);
		}
		else
		{
			m_pScene = m_Importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		}

		if (!m_pScene)
		{
			KE::Debug::print
			(
				KE::Debug::DBG_ERROR, "Meshes LoadMeshes scene import failed. Importer error string = "
				+ std::string(m_Importer.GetErrorString())
			);
			return false;
		}
		return true;
	}

	Meshes_SmartPtr MeshLoader::InitFromScene(const aiScene * p_pScene, const std::string & p_rPath)
	{
		Meshes_SmartPtr meshes(new KG::Meshes);
		for (int i = 0; i < static_cast<int>(p_pScene->mNumMeshes); ++i)
		{
			// load mesh
			KG::Mesh_SmartPtr mesh = this->InitMesh(p_pScene->mMeshes[i]);
			// load texture
			this->InitMaterial(mesh, p_pScene->mMeshes[i], m_pScene, p_rPath);
			meshes->AddChild(mesh);
		}

		m_Importer.FreeScene(); m_pScene = nullptr;
		return meshes;
	}

	Mesh_SmartPtr MeshLoader::InitMesh(const aiMesh * const p_AiMesh)
	{
		KE::Debug::print("Model : Init mesh.");
		KG::Mesh_SmartPtr mesh(new KG::Mesh());
		
		// pos vertex
		if (p_AiMesh->HasPositions())
		{
			mesh->m_HasPosVertices = true;
			const unsigned num_pos = p_AiMesh->mNumVertices;
			mesh->m_PosVertices.reserve(num_pos);
			for (int i = 0; i < static_cast<int>(num_pos); ++i)
			{
				const aiVector3D & ai_pos = p_AiMesh->mVertices[i];
				mesh->m_PosVertices.push_back(glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z));
			}
		}

		// index
		if (p_AiMesh->HasFaces())
		{
			mesh->m_HasFaces = true;
			const unsigned num_faces = p_AiMesh->mNumFaces;
			mesh->SetNumIndex(num_faces * 3);
			mesh->SetNumElement(num_faces * 3);
			mesh->m_Indices.reserve(num_faces * 3);
			for (int i = 0; i < static_cast<int>(num_faces); ++i)
			{
				const aiFace & face = p_AiMesh->mFaces[i];
				assert(face.mNumIndices == 3);
				for (int j = 0; j < 3; ++j)
					mesh->m_Indices.push_back(face.mIndices[j]);
			}
		}

		// normal 
		if (p_AiMesh->HasNormals())
		{
			mesh->m_HasNormals = true;
			const unsigned num_normal = p_AiMesh->mNumVertices;
			mesh->m_NormalVertices.reserve(num_normal);
			for (int i = 0; i < static_cast<int>(num_normal); ++i)
			{
				const aiVector3D & ai_normal = p_AiMesh->mNormals[i];
				mesh->m_NormalVertices.push_back(glm::vec3(ai_normal.x, ai_normal.y, ai_normal.z));
			}
		}

		// material
		if (m_pScene->HasMaterials())
		{
			mesh->m_HasMaterial = true;
			int material_id = p_AiMesh->mMaterialIndex;
			const aiMaterial * const material_ptr = m_pScene->mMaterials[material_id];
			aiString name;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_NAME, name))
				mesh->m_Material.Name = name.C_Str();
			float shininess = 0.0f;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_SHININESS, shininess))
				mesh->m_Material.Shininess = shininess;
			aiColor3D ai_vec3;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_AMBIENT, ai_vec3)) // ambient
				mesh->m_Material.Ambient = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_DIFFUSE, ai_vec3)) // diffuse
				mesh->m_Material.Diffuse = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_SPECULAR, ai_vec3)) // specular
				mesh->m_Material.Specular = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_EMISSIVE, ai_vec3)) // emissive
				mesh->m_Material.Emissive = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
		}

		// color vertex.
		const unsigned vc_id = 0;
		if (p_AiMesh->HasVertexColors(vc_id))
		{
			mesh->m_HasColors = true;
			const unsigned num_colors = mesh->GetVertices().size();
			mesh->m_ColorVertices.reserve(num_colors);
			for (int i = 0; i < static_cast<int>(num_colors); ++i)
			{
				const aiColor4D & color_vert = p_AiMesh->mColors[vc_id][i];
				mesh->m_ColorVertices.push_back(glm::vec4(color_vert.r, color_vert.g, color_vert.b, color_vert.a));
			}
		}

		// texture coords
		const unsigned tc_id = 0;
		if (p_AiMesh->HasTextureCoords(tc_id))
		{
			mesh->m_HasTexCoords = true;
			const unsigned num_tex_coord = mesh->GetVertices().size();
			mesh->m_TexCoordVertices.reserve(num_tex_coord);
			for (int i = 0; i < static_cast<int>(num_tex_coord); ++i)
			{				
				const aiVector3D & tex_coord = p_AiMesh->mTextureCoords[tc_id][i];
				mesh->m_TexCoordVertices.push_back(glm::vec3(tex_coord.x, tex_coord.y, tex_coord.z));
			}
		}

		mesh->m_Loaded = true;
		return mesh;
	}

	const bool MeshLoader::InitMaterial
		(
			Mesh_SmartPtr p_spMesh
			, const aiMesh * const p_pAiMesh
			, const aiScene * const p_pAiScene
			, const std::string & p_Path
		)
	{
		bool result(true);

		// Extract the directory part from the file name
		std::string::size_type slash_index = p_Path.find_last_of("/");
		std::string dir;

		if (slash_index == std::string::npos)
			dir = ".";
		else if (slash_index == 0) 
			dir = "/";
		else 
			dir = p_Path.substr(0, slash_index);

		const int material_index = p_pAiMesh->mMaterialIndex;
		const aiMaterial * const material_ptr = p_pAiScene->mMaterials[material_index];
		if (!material_ptr)
			return false;

		// load material texture. Only diffuse ATM.
		if (material_ptr->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if ( material_ptr->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)
					== AI_SUCCESS
				)
			{
				const std::string FullPath = dir + "/" + Path.data;
				KG::Texture_SmartPtr texture(new KG::Texture(KG::Texture::DType::Tex2D, FullPath));
				p_spMesh->SetTexture(texture);
			}
		}	

		return result; // TODO
	}

} // KG ns
