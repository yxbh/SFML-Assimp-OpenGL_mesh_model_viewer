#include "Skeleton.hpp"
#include "Animation.hpp"

namespace KG
{

	Skeleton::Skeleton(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: KG::SceneNode(p_EntityID, p_RenderPass)
	{}

	Skeleton::~Skeleton(void)
	{}

	const bool Skeleton::HasAnimations(void) const
	{
		return m_Animations.size() != 0;
	}

	KG::SceneNodeList & Skeleton::GetChildren(void)
	{
		return m_ChildSceneNodeList;
	}

	void Skeleton::Reserve(const unsigned p_Size)
	{
		names.reserve(p_Size);
		offset_transforms.reserve(p_Size);
		final_transforms.reserve(p_Size);
	}

	void Skeleton::ReserveMemForIDs(const unsigned p_Size)
	{
		IDs.reserve(p_Size);
	}

	void Skeleton::ReserveMemForWeights(const unsigned p_Size)
	{
		weights.reserve(p_Size);
	}


} // KG ns
