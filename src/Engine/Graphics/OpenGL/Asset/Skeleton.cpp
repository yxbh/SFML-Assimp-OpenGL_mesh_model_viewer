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

	void Skeleton::ComputePose(const KE::Duration p_Elapsed)
	{
		

		if (m_Animations.size() == 0)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "Skeleton::ComputePose : no animations to compute pose!");
			return;
		}

		KG::Animation_SmartPtr current_animation_sp(m_Animations[m_CurAnimIndex]);


	}

	void Skeleton::ComputePoseTransforms(const KE::Duration p_Elapsed)
	{

	}

} // KG ns
