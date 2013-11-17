#include "Skeleton.hpp"
#include "Animation.hpp"

namespace KG
{

	BoneNode::BoneNode
	(
		const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, skeleton_bone_index(0)
	{}

	BoneNode::~BoneNode(void)
	{}



	Skeleton::Skeleton(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_CurAnimIndex(0)
	{}

	Skeleton::~Skeleton(void)
	{}

	const bool Skeleton::HasAnimations(void) const
	{
		return !m_Animations.empty();
	}

	void Skeleton::ReserveForBones(const unsigned p_Size)
	{
		bone_names.reserve(p_Size);
		bone_transforms.reserve(p_Size);
		bone_offsets.reserve(p_Size);
		intermediate_transforms.reserve(p_Size);
		final_transforms.reserve(p_Size);
	}

	void Skeleton::ReserveForVertices(const unsigned p_Size)
	{
		bone_IDs.reserve(p_Size);
		bone_weights.reserve(p_Size);
	}

	void Skeleton::ComputePose(const KE::Duration p_Elapsed)
	{
		if (/*true || */m_Animations.empty())
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Skeleton::ComputePose : no animations to compute pose! Default to bind pose. ");

			// set intermediate to bone node transforms to retain bind pose.
			for (unsigned i = 0; i < this->intermediate_transforms.size(); ++i)
				this->intermediate_transforms[i] = this->bone_transforms[i];
		}
		else
		{
			// calculate animation pose.
			KG::Animation_SmartPtr current_animation_sp(m_Animations[m_CurAnimIndex]);
			current_animation_sp->ComputePoses(p_Elapsed);
			for (KG::AnimationNode_SmartPtr anim_node_sp : current_animation_sp->GetChannels())
			{
				const unsigned bone_index(anim_node_sp->GetBoneIndex());
				this->intermediate_transforms[bone_index] = anim_node_sp->GetTransform();
			}
		}

		for (KG::SceneNode_SmartPtr scenenode : m_ChildSceneNodeList)
		{
			const KG::BoneNode_SmartPtr bonenode(std::static_pointer_cast<KG::BoneNode>(scenenode));
			this->ComputePoseTransforms(bonenode, glm::dmat4());
		}

	}

	void Skeleton::ComputePoseTransforms(const KG::BoneNode_SmartPtr p_spBoneNode, const glm::dmat4 & p_ParentTransform)
	{
		const unsigned bone_index(p_spBoneNode->skeleton_bone_index);
		const glm::dmat4 global_transform( p_ParentTransform * this->intermediate_transforms[bone_index] );
		this->final_transforms[bone_index]
			= this->global_inverse_transform * global_transform * this->bone_offsets[bone_index];

		for (KG::SceneNode_SmartPtr scenenode : p_spBoneNode->GetChildSceneNodeList())
		{
			const KG::BoneNode_SmartPtr bonenode(std::static_pointer_cast<KG::BoneNode>(scenenode));
			this->ComputePoseTransforms(bonenode, global_transform);
		}
	}

} // KG ns
