#include "MeshLoader.hpp"
#include "Mesh.hpp"
#include "Meshes.hpp"
#include "TextureLoader.hpp"
#include "Texture.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"

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
			m_pScene = m_Importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipWindingOrder|aiProcess_FlipUVs);
		}
		else
		{
			m_pScene = m_Importer.ReadFile(p_rPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs);
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
		// load all the meshes
		for (int i = 0; i < static_cast<int>(p_pScene->mNumMeshes); ++i)
		{
			// load mesh
			KG::Mesh_SmartPtr mesh(this->InitMesh(p_pScene->mMeshes[i]));
			mesh->SetID(meshes->GetEntityID());

			// load skeleton if there's any.
			this->InitSkeleton(mesh, p_pScene->mMeshes[i]);

			// load all animatins if there's any.
			this->InitAnimations(mesh);

			// load texture
			this->InitMaterialTexture(mesh, p_pScene->mMeshes[i], m_pScene, p_rPath);

			meshes->AddChild(mesh);
		}

		m_Importer.FreeScene(); m_pScene = nullptr; // cleanup
		return meshes;
	}

	Mesh_SmartPtr MeshLoader::InitMesh(const aiMesh * const p_pAiMesh)
	{
		KE::Debug::print("MeshLoader::InitMesh : New mesh.");
		assert(p_pAiMesh); // cannot be null
		KG::Mesh_SmartPtr mesh(new KG::Mesh());
		mesh->SetName(p_pAiMesh->mName.data);

		// pos vertex
		this->InitPositions(mesh, p_pAiMesh);

		// index
		this->InitFaces(mesh, p_pAiMesh);

		// normal
		this->InitNormals(mesh, p_pAiMesh);

		// material
		this->InitMaterials(mesh, p_pAiMesh);

		// color vertex.
		this->InitColors(mesh, p_pAiMesh);

		// texture coords
		this->InitTexCoords(mesh, p_pAiMesh);

		mesh->m_Loaded = true;
		return mesh;
	}

	void MeshLoader::InitPositions(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasPositions())
		{
			KE::Debug::print("MeshLoader::InitPositions : Init positions.");
			p_spMesh->m_HasPosVertices = true;
			const unsigned num_pos(p_pAiMesh->mNumVertices);
			p_spMesh->m_PosVertices.reserve(num_pos);
			for (int i = 0; i < static_cast<int>(num_pos); ++i)
			{
				const aiVector3D & ai_pos = p_pAiMesh->mVertices[i];
				p_spMesh->m_PosVertices.push_back(glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z));
			}
		}
		else
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::InitPositions : mesh has no position vertices!");
			assert(false);
		}
	}

	void MeshLoader::InitFaces(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasFaces())
		{
			KE::Debug::print("MeshLoader::InitFaces : Init indices.");
			p_spMesh->m_HasFaces = true;
			const unsigned num_faces(p_pAiMesh->mNumFaces);
			p_spMesh->SetNumIndex(num_faces * 3);
			p_spMesh->SetNumElement(num_faces * 3);
			p_spMesh->m_Indices.reserve(num_faces * 3);
			for (int i = 0; i < static_cast<int>(num_faces); ++i)
			{
				const aiFace & face = p_pAiMesh->mFaces[i];
				assert(face.mNumIndices == 3);
				for (int j = 0; j < 3; ++j)
					p_spMesh->m_Indices.push_back(face.mIndices[j]);
			}
		}
	}

	void MeshLoader::InitNormals(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasNormals())
		{
			KE::Debug::print("MeshLoader::InitNormals : Init normals.");
			p_spMesh->m_HasNormals = true;
			const unsigned num_normal(p_pAiMesh->mNumVertices);
			p_spMesh->m_NormalVertices.reserve(num_normal);
			for (int i = 0; i < static_cast<int>(num_normal); ++i)
			{
				const aiVector3D & ai_normal = p_pAiMesh->mNormals[i];
				p_spMesh->m_NormalVertices.push_back(glm::vec3(ai_normal.x, ai_normal.y, ai_normal.z));
			}
		}
	}

	void MeshLoader::InitTexCoords(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		const unsigned texcoord_id = 0;
		if (p_pAiMesh->HasTextureCoords(texcoord_id))
		{
			KE::Debug::print("MeshLoader::InitTexCoords : Init material.");
			p_spMesh->m_HasTexCoords = true;
			const unsigned num_tex_coord(p_spMesh->GetVertices().size());
			p_spMesh->m_TexCoordVertices.reserve(num_tex_coord);
			for (int i = 0; i < static_cast<int>(num_tex_coord); ++i)
			{				
				const aiVector3D & tex_coord = p_pAiMesh->mTextureCoords[texcoord_id][i];
				p_spMesh->m_TexCoordVertices.push_back(glm::vec3(tex_coord.x, tex_coord.y, tex_coord.z));
			}
		}
	}

	void MeshLoader::InitColors(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		const unsigned vc_id = 0; // TODO : ??
		if (p_pAiMesh->HasVertexColors(vc_id))
		{
			KE::Debug::print("MeshLoader::InitColors : Init colors.");
			p_spMesh->m_HasColors = true;
			const unsigned num_colors(p_spMesh->GetVertices().size());
			p_spMesh->m_ColorVertices.reserve(num_colors);
			for (int i = 0; i < static_cast<int>(num_colors); ++i)
			{
				const aiColor4D & color_vert = p_pAiMesh->mColors[vc_id][i];
				p_spMesh->m_ColorVertices.push_back(glm::vec4(color_vert.r, color_vert.g, color_vert.b, color_vert.a));
			}
		}
	}

	void MeshLoader::InitMaterials(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (m_pScene->HasMaterials())
		{
			KE::Debug::print("MeshLoader::InitMaterials : Init texture coords.");
			p_spMesh->m_HasMaterial = true;
			int material_id = p_pAiMesh->mMaterialIndex;
			const aiMaterial * const material_ptr = m_pScene->mMaterials[material_id];
			aiString name;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_NAME, name))
				p_spMesh->m_Material.Name = name.C_Str();
			float shininess = 0.0f;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_SHININESS, shininess))
				p_spMesh->m_Material.Shininess = shininess;
			aiColor3D ai_vec3;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_AMBIENT, ai_vec3)) // ambient
				p_spMesh->m_Material.Ambient = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_DIFFUSE, ai_vec3)) // diffuse
				p_spMesh->m_Material.Diffuse = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_SPECULAR, ai_vec3)) // specular
				p_spMesh->m_Material.Specular = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_EMISSIVE, ai_vec3)) // emissive
				p_spMesh->m_Material.Emissive = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
		}
	}

	void MeshLoader::InitSkeleton(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (!p_pAiMesh->HasBones())
			return; // no bones, leave.

		KE::Debug::print("MeshLoader::InitSkeleton : Init Skeleton.");
		KG::Skeleton_SmartPtr skeleton_ptr(new KG::Skeleton(p_spMesh->GetEntityID()));
		// get and set global inverse matrix
		const aiMatrix4x4 & ai_mat(m_pScene->mRootNode->mTransformation); // TODO : correct?
		const glm::dvec4 col1(ai_mat.a1, ai_mat.a2, ai_mat.a3, ai_mat.a4);
		const glm::dvec4 col2(ai_mat.b1, ai_mat.b2, ai_mat.b3, ai_mat.b4);
		const glm::dvec4 col3(ai_mat.c1, ai_mat.c2, ai_mat.c3, ai_mat.c4);
		const glm::dvec4 col4(ai_mat.d1, ai_mat.d2, ai_mat.d3, ai_mat.d4);
		skeleton_ptr->global_inverse_transform = glm::inverse(glm::dmat4(col1, col2, col3, col4));

		// convert Bone to Vertices relation to Vertex to bones relation.
		const unsigned num_bones = p_pAiMesh->mNumBones;
		skeleton_ptr->Reserve(num_bones);
		// resize intermediate and final transform array.
		skeleton_ptr->intermediate_transforms.resize(num_bones);
		skeleton_ptr->final_transforms.resize(num_bones);

		// collect name, offset, and weights of each bone:
		std::map<unsigned, std::multimap<float, std::string>> vertex_to_bones_map; // <v_index, <weight, bone_name>>
		for (unsigned i = 0; i < num_bones; ++i)
		{
			const aiBone * const ai_bone_ptr = p_pAiMesh->mBones[i];
			
			// collect bone name for Skeleton (per-bone)
			const std::string bone_name(ai_bone_ptr->mName.data);
			skeleton_ptr->names.push_back(bone_name);
			
			// collect offset transform for Skeleton (per-bone)
			const aiMatrix4x4 & ai_mat(ai_bone_ptr->mOffsetMatrix);
			const glm::dvec4 col1(ai_mat.a1, ai_mat.a2, ai_mat.a3, ai_mat.a4);
			const glm::dvec4 col2(ai_mat.b1, ai_mat.b2, ai_mat.b3, ai_mat.b4);
			const glm::dvec4 col3(ai_mat.c1, ai_mat.c2, ai_mat.c3, ai_mat.c4);
			const glm::dvec4 col4(ai_mat.d1, ai_mat.d2, ai_mat.d3, ai_mat.d4);
			//skeleton_ptr->bone_offsets.push_back(glm::transpose(glm::dmat4(col1, col2, col3, col4)));
			skeleton_ptr->bone_offsets.push_back(glm::dmat4(col1, col2, col3, col4));

			// insert bone weights into map. (per-vertex & per-weight)

			for (unsigned weight_i = 0; weight_i < ai_bone_ptr->mNumWeights; ++weight_i)
			{
				const aiVertexWeight ai_vweight = ai_bone_ptr->mWeights[weight_i];
				vertex_to_bones_map[ai_vweight.mVertexId]
					.insert(std::make_pair(ai_vweight.mWeight, bone_name));
			}
		}
		for (unsigned vertex_index = 0; vertex_index < p_spMesh->m_PosVertices.size(); ++vertex_index)  // pre-fill map with empty entries first.
		{
			vertex_to_bones_map.insert(std::make_pair(vertex_index, std::multimap<float, std::string>()));
		}
		for (auto & pair_it  : vertex_to_bones_map) // needs map to be pre-filled to work properly.
		{
			while (pair_it.second.size() > 4) // reduce num of bones for each vertex to 4.
				pair_it.second.erase(pair_it.second.begin());
			while (pair_it.second.size() != 4) // fill up with 0 weights if less than 4 actual bone influences.
				pair_it.second.insert(std::make_pair(0.0f, std::string(p_pAiMesh->mBones[0]->mName.data))); // use any bone name. It doesn't matter since weight is 0 anyway.
				// TODO : normalize total weight so it equals 1.
		}

		// fill in Skeleton's ID's and weights vectors for each vertex
		//	also calculate indices for each weight for each vertex.
		unsigned vertex_index = 0;
		skeleton_ptr->IDs.resize(p_pAiMesh->mNumVertices);		// resize first and then iterate.
		skeleton_ptr->weights.resize(p_pAiMesh->mNumVertices);	// resize first and then iterate.
		for (auto & vertex_to_names : vertex_to_bones_map)
		{ // for vertex to 4 x Weights pair
			auto bone_weight_pair_it = vertex_to_names.second.begin(); //--bone_weight_pair_it;

			// 1st weight
			auto it = std::find(skeleton_ptr->names.begin(), skeleton_ptr->names.end(), bone_weight_pair_it->second);
			if (it == skeleton_ptr->names.end())
				assert(false); // should never fail.
			unsigned bone_index = std::distance(skeleton_ptr->names.begin(), it);
			if (bone_index < 0 || bone_index > 4) assert(false);
			skeleton_ptr->IDs[vertex_index].x = bone_index;
			skeleton_ptr->weights[vertex_index].x = bone_weight_pair_it->first;

			// 2nd weight
			++bone_weight_pair_it;
			it = std::find(skeleton_ptr->names.begin(), skeleton_ptr->names.end(), bone_weight_pair_it->second);
			if (it == skeleton_ptr->names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_ptr->names.begin(), it);
			if (bone_index < 0 || bone_index > 4) assert(false);
			skeleton_ptr->IDs[vertex_index].y = bone_index;
			skeleton_ptr->weights[vertex_index].y = bone_weight_pair_it->first;

			// 3rd weight
			++bone_weight_pair_it;
			it = std::find(skeleton_ptr->names.begin(), skeleton_ptr->names.end(), bone_weight_pair_it->second);
			if (it == skeleton_ptr->names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_ptr->names.begin(), it);
			if (bone_index < 0 || bone_index > 4) assert(false);
			skeleton_ptr->IDs[vertex_index].z = bone_index;
			skeleton_ptr->weights[vertex_index].z = bone_weight_pair_it->first;

			// 4th weight
			++bone_weight_pair_it;
			it = std::find(skeleton_ptr->names.begin(), skeleton_ptr->names.end(), bone_weight_pair_it->second);
			if (it == skeleton_ptr->names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_ptr->names.begin(), it);
			if (bone_index < 0 || bone_index > 4) assert(false);
			skeleton_ptr->IDs[vertex_index].w = bone_index;
			skeleton_ptr->weights[vertex_index].w = bone_weight_pair_it->first;
			++vertex_index;

			this->ConstructSkeleton(skeleton_ptr, p_pAiMesh, m_pScene->mRootNode);

			p_spMesh->SetSkeleton(skeleton_ptr);
			p_spMesh->m_HasSkeleton = true;
			
		} // if has bones
	}

	void MeshLoader::ConstructSkeleton(KG::Skeleton_SmartPtr p_spSkeleton, const aiMesh * const p_pAiMesh, const aiNode * const p_AiNode)
	{

		/*
			To reconstruct the bone tree. Build a bone depth map by going through each Bone name and search them in the scene.
			For each bone. calculate the depth from the root. the one with the lowest depth would be the root bone.
		*/

		//construct a bone depth map
		std::map<std::string, unsigned> bone_depth_map; // bones must have different names.
		for ( auto & bone_name : p_spSkeleton->names)
		{
			unsigned depth = 0; // depth starts from 1 at the root AiNode
			const aiNode * scene_root_node = m_pScene->mRootNode;
			if (!scene_root_node) // check root node first.
			{
				KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::ConstructSkeleton : root node is null?");
				return;
			}
			
			if (this->FindBoneDepth(depth, scene_root_node, bone_name))
			{
				bone_depth_map.insert(std::make_pair(bone_name, depth));
			}
			else
				assert(false);
		}

		// find root bones of skeleton.
		// Note: there could be more than 1. (e.g. Blender's armture is a root but isn't technically a bone).
		unsigned min_depth = 9999; // just something bigger than the possible number of bones...
		std::string root_bone_name("N/A");
		for (auto & it : bone_depth_map)
		{
			if (it.second < min_depth)
			{
				min_depth = it.second;
				root_bone_name = it.first;
			}
		}
		std::vector<std::string> root_bone_names;
		for (auto & it : bone_depth_map) // get all root bone names.
		{
			if (it.second == min_depth)
				root_bone_names.push_back(it.first);
		}
		
		for (auto & name : root_bone_names)
		{
			this->GrowBoneTree(p_spSkeleton, nullptr, p_pAiMesh, this->FindAiNodeByName(name,  p_AiNode));
		}

	}

	const bool MeshLoader::FindBoneDepth(unsigned & p_Depth, const aiNode * const p_pAiNode, const std::string & p_BoneName)
	{
		++p_Depth;
		const std::string this_bone_name(p_pAiNode->mName.data);
		if (p_BoneName == this_bone_name) // FOUND!!!
			return true;
		else
		{
			for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			{
				if (this->FindBoneDepth(p_Depth, p_pAiNode->mChildren[i], p_BoneName))
					return true;
				else
					--p_Depth;
			}
		}
		return false;
	}

	const aiNode * const MeshLoader::FindAiNodeByName(const std::string & p_rNodeName, const aiNode * const p_pAiNode)
	{
		if (!p_pAiNode)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::FindAiNodeByName : received a nullptr");
			assert(false);
		}
		
		const std::string this_bone_name(p_pAiNode->mName.data);
		if (p_rNodeName == this_bone_name) // FOUND!!!
			return p_pAiNode;
		else
		{
			for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			{
				const aiNode * const ai_node_ptr = this->FindAiNodeByName(p_rNodeName, p_pAiNode->mChildren[i]);
				if (ai_node_ptr)
					return ai_node_ptr;
			}
		}
		return nullptr;
	}

	void MeshLoader::GrowBoneTree
	(
		KG::Skeleton_SmartPtr p_spSkeleton
		, KG::BoneNode_SmartPtr p_spBoneNode
		, const aiMesh * const p_pAiMesh
		, const aiNode * const p_pAiNode
	)
	{
		if (!p_pAiNode) // check valid pointer.
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::GrowBoneTree : received a nullptr");
			assert(false);
		}

		// create and fill in bone node
		KG::BoneNode_SmartPtr bone_node_sp(new KG::BoneNode(p_spSkeleton->GetEntityID(), KG::RenderPass::NotRendered));
		const std::string bone_name(p_pAiNode->mName.data);
		bone_node_sp->SetName(bone_name);
		bool bone_found = false;
		for (unsigned i = 0; i < p_pAiMesh->mNumBones; ++i)
		{
			if (bone_name == std::string(p_pAiMesh->mBones[i]->mName.data))
			{
				bone_found = true;

				// compute index to array in Skeleton.
				unsigned index = 0;
				for (const std::string & name : p_spSkeleton->names)
				{
					if (bone_name == name)
					{
						bone_node_sp->skeleton_bone_index = index;
						break;
					}
					else
						++index;
				}

				break; // found
			}
		}
		if (!bone_found)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::GrowBoneTree : unexpected error: bone not found in aiMesh!");
			assert(false);
		}

		// attach to parent:
		if (p_spBoneNode == nullptr) // if given BoneNode is null then it's a root bone.
			p_spSkeleton->AddChild(bone_node_sp);
		else
			p_spBoneNode->AddChild(bone_node_sp);

		// recursively grow branches:
		for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			this->GrowBoneTree(p_spSkeleton, bone_node_sp, p_pAiMesh, p_pAiNode->mChildren[i]);
	}

	void MeshLoader::InitAnimations(Mesh_SmartPtr p_spMesh)
	{
		assert(p_spMesh);
		assert(m_pScene);
		if (!m_pScene->HasAnimations())
			return; // no animations

		// load all aiAnimations
		for (unsigned i = 0; i < m_pScene->mNumAnimations; ++i) // for each aiAnimation
		{ // load each animation into proprietary format.
			KG::Animation_SmartPtr animation_sp(new KG::Animation(p_spMesh->GetEntityID()));
			const aiAnimation * const ai_anim_ptr = m_pScene->mAnimations[i];
			const double factor = (ai_anim_ptr->mTicksPerSecond > 0) ? ai_anim_ptr->mTicksPerSecond : 1.0;
			for (unsigned a = 0; a < ai_anim_ptr->mNumChannels; ++a)
			{ // iterate through each aiNode
				KG::AnimationNode_SmartPtr anim_node_sp(new KG::AnimationNode(p_spMesh->GetEntityID()));
				const aiNodeAnim * const ai_animnode_ptr = ai_anim_ptr->mChannels[a];

				// compute index to array in Skeleton.
				unsigned index = 0;
				const std::string bone_name(ai_animnode_ptr->mNodeName.data);
				anim_node_sp->SetName(bone_name);
				for (const std::string & name : p_spMesh->m_spSkeleton->names)
				{
					if (bone_name == name)
					{
						anim_node_sp->m_SkeletonBoneIndex = index;
						break;
					}
					else
						++index;
				}

				// collect scaling keys
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumScalingKeys; ++node_index)
				{
					const aiVectorKey & ai_key = ai_animnode_ptr->mScalingKeys[node_index];
					const glm::dvec3 vec(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/factor));
					const KG::AnimationScaleKey key(time, vec);
					anim_node_sp->m_ScaleKeys.push_back(key);
				}

				// collect translation offset keys
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumPositionKeys; ++node_index)
				{
					const aiVectorKey & ai_key = ai_animnode_ptr->mPositionKeys[node_index];
					const glm::dvec3 vec(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/factor));
					const KG::AnimationTranslationKey key(time, vec);
					anim_node_sp->m_TranslationKeys.push_back(key);
				}

				// collect rotation keys.
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumRotationKeys; ++node_index)
				{
					const aiQuatKey & ai_key = ai_animnode_ptr->mRotationKeys[node_index];
					const glm::dquat quat(ai_key.mValue.w, ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/factor));
					const KG::AnimationRotationKey key(time, quat);
					anim_node_sp->m_RotationKeys.push_back(key);
				}
				animation_sp->m_Channels.push_back(anim_node_sp);
			} // end for each channel.

			p_spMesh->m_spSkeleton->m_Animations.push_back(animation_sp);
		} // end for each aiAnimation.

	}

	const bool MeshLoader::InitMaterialTexture
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
			if ( material_ptr->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr)
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
