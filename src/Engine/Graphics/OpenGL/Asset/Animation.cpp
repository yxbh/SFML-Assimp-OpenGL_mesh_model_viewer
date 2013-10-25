#include "Animation.hpp"

namespace KG
{
	AnimationNode::AnimationNode
	(
		const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
	{}

	AnimationNode::AnimationNode
	(
		AnimationScaleKeyList p_ScaleKeys
		, AnimationPositionKeyList p_PositionKeys
		, AnimationOrientationKeyList p_OrientationKeys
		, const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_ScaleKeys(p_ScaleKeys)
		, m_PositionKeys(p_PositionKeys)
		, m_OrientationKeys(p_OrientationKeys)
	{}

	AnimationNode::~AnimationNode(void)
	{}

	const AnimationScaleKeyList & AnimationNode::GetScaleKeys(void) const
	{
		return m_ScaleKeys;
	}

	const AnimationPositionKeyList & AnimationNode::GetPositionKeys(void) const
	{
		return m_PositionKeys;
	}

	const AnimationOrientationKeyList & AnimationNode::GetOrientationKeys(void) const
	{
		return m_OrientationKeys;
	}

	const std::int16_t AnimationNode::GetBehaviour(void) const
	{
		return m_AnimationBehaviour;
	}

	AnimationNode & AnimationNode::SetBehaviour(const std::int16_t p_Behaviour)
	{
		m_AnimationBehaviour = p_Behaviour;
		return *this;
	}


	Animation::Animation
	(
		const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
	{}

	Animation::~Animation(void)
	{}

} // KG ns
