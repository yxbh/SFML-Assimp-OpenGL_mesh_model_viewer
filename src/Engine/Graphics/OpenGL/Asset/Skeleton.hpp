#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"
#include "../SceneNode.hpp"

namespace KG
{

	/*! \class BoneNode
	*/
	class BoneNode
		: public KG::SceneNode
	{
	public:
		unsigned			skeleton_bone_index;		// index to the per-bone data arrays in Skeleton.

	public:
		BoneNode(const KE::EntityID p_EntityID , const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered);
		~BoneNode(void);

	}; // class BoneNode

	typedef std::shared_ptr<KG::BoneNode> BoneNode_SmartPtr;
	typedef std::weak_ptr<KG::BoneNode> BoneNode_WeakPtr;

	typedef Vec4i VertexIDs;
	typedef Vec4i BoneIDs;
	typedef Vec4f BoneWeights;

	class Animation;
	typedef std::shared_ptr<Animation> Animation_SmartPtr;
	typedef std::weak_ptr<Animation> Animation_WeakPtr;


	/*! \class Skeleton

		Acts as root to all BoneNodes.
	*/
	class Skeleton
		: public KG::SceneNode
	{
		friend class MeshLoader;

	public:
		glm::dmat4							global_inverse_transform;
		/* per-bone data */
		std::vector<std::string>			names;				// bone names
		std::vector<glm::dmat4>				bone_transform;		// from aiNode. un-filled atm
		std::vector<glm::dmat4>				bone_offsets;		//
		std::vector<glm::dmat4>				intermediate_transforms; // individual AnimationNode transforms
		std::vector<glm::mat4>				final_transforms;	// uses float version for GLSL compatibility
		/* per-vertex data */
		std::vector<BoneIDs>				IDs;				// positon would be index to vertex. value is the IDs.
		std::vector<BoneWeights>			weights;			// positon would be index to vertex. value is the weights.
		/**/
		std::vector<KG::Animation_SmartPtr>	m_Animations;
		unsigned							m_CurAnimIndex;		// index to current animation.

	public:
		Skeleton
		(
			const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		~Skeleton(void);

		const bool HasAnimations(void) const;

		KG::SceneNodeList & GetChildren(void);

		void Reserve(const unsigned p_Size);
		void ReserveMemForIDs(const unsigned p_Size);
		void ReserveMemForWeights(const unsigned p_Size);

		/*! compute frame interpolation and etc. */
		void ComputePose(const KE::Duration p_Elapsed);

	private:
		/*! calculate final transform for each bone. */
		void ComputePoseTransforms(const KG::BoneNode_SmartPtr p_spBoneNode, const glm::dmat4 & p_ParentTransform);
		void ComputePoseTransforms(const KG::BoneNode_SmartPtr p_spBoneNode, const glm::dmat4 & p_ParentAnimTransform, const glm::dmat4 & p_ParentBoneTransform);

	}; // class Skeleton

	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;

} // KG ns
