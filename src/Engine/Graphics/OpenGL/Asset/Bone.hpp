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

	/*! \class
	*/
	class BoneNode
		: public KG::SceneNode
	{
	public:
		BoneNode(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered)
			: KG::SceneNode(p_EntityID, p_RenderPass)
		{}
		~BoneNode(void) {}
	};

	typedef Vec4i VertexIDs;
	typedef Vec4i BoneIDs;
	typedef Vec4f BoneWeights;


	/*! \class

	*/
	class Skeleton
		: public KG::SceneNode
	{
	public:
		std::vector<std::string>	names;				// bone names
		std::vector<BoneIDs>		IDs;				// positon would be index to vertex. value is the IDs.
		std::vector<BoneWeights>	weights;			// positon would be index to vertex. value is the weights.
		std::vector<BoneTransform>	bone_transforms;	// offset and final transform for each bone.
		
	public:
		Skeleton(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered)
			: KG::SceneNode(p_EntityID, p_RenderPass)
		{}
		~Skeleton(void) {}

		KG::SceneNodeList & GetChildren(void)
		{
			return m_ChildSceneNodeList;
		}

		void Reserve(const unsigned p_Size)
		{
			names.reserve(p_Size);
			IDs.reserve(p_Size);
			weights.reserve(p_Size);
			bone_transforms.reserve(p_Size);
		}

	private:
		void ComputeBoneTransforms(const KE::Duration, const glm::dmat4 & p_ParentTransform)
		{
			// TODO
			KE_UNREFERENCED_PARAMETER(p_ParentTransform);
		}

	};

	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;

} // KG ns
