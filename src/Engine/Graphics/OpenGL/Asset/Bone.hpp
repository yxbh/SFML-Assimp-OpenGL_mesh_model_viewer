#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"
#include "../SceneNode.hpp"

namespace KG
{

	/*! \class
	*/
	struct BoneTransform
	{
		glm::dmat4 offset;
		glm::dmat4 final;
	};

	/*! \class BoneNode
	*/
	class BoneNode
		: public KG::SceneNode
	{
	public:
		KG::BoneTransform	transform;

	public:
		BoneNode(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered)
			: KG::SceneNode(p_EntityID, p_RenderPass)
		{}
		~BoneNode(void) {}
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
		std::vector<std::string>			names;				// bone names
		std::vector<BoneIDs>				IDs;				// positon would be index to vertex. value is the IDs.
		std::vector<BoneWeights>			weights;			// positon would be index to vertex. value is the weights.
		std::vector<glm::dmat4>				offset_transforms;
		std::vector<glm::dmat4>				final_transforms;
		std::vector<KG::Animation_SmartPtr>	m_Animations;

	public:
		Skeleton
		(
			const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID()
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		~Skeleton(void);

		const bool HasAnimations(void) const;

		KG::SceneNodeList & GetChildren(void);

		void Reserve(const unsigned p_Size);
		void ReserveMemForIDs(const unsigned p_Size);
		void ReserveMemForWeights(const unsigned p_Size);

	private:
		void ComputeBoneTransforms(const KE::Duration, const glm::dmat4 & p_ParentTransform)
		{
			// TODO
			KE_UNREFERENCED_PARAMETER(p_ParentTransform);
		}

	}; // class Skeleton

	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;

} // KG ns
