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
		, AnimationTranslationKeyList p_TranslationKeys
		, AnimationRotationKeyList p_RotationKeys
		, const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_ScaleKeys(p_ScaleKeys)
		, m_TranslationKeys(p_TranslationKeys)
		, m_RotationKeys(p_RotationKeys)
	{}

	AnimationNode::~AnimationNode(void)
	{}

	const AnimationScaleKeyList & AnimationNode::GetScaleKeys(void) const
	{
		return m_ScaleKeys;
	}

	const AnimationTranslationKeyList & AnimationNode::GetTranslationKeys(void) const
	{
		return m_TranslationKeys;
	}

	const AnimationRotationKeyList & AnimationNode::GetRotationKeys(void) const
	{
		return m_RotationKeys;
	}

	const KE::Duration AnimationNode::ComputeScaleTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		const int mod(time_stamp % m_ScaleKeys.back().first);
		if (mod > 0)
			time_stamp -= m_ScaleKeys.back().first * std::int64_t(mod);
		return time_stamp;
	}

	const KE::Duration AnimationNode::ComputeTranslationTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		const int mod(time_stamp % m_TranslationKeys.back().first);
		if (mod > 0)
			time_stamp -= m_TranslationKeys.back().first * std::int64_t(mod);
		return time_stamp;
	}

	const KE::Duration AnimationNode::ComputeRotationTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		const int mod(time_stamp % m_RotationKeys.back().first);
		if (mod > 0)
			time_stamp -= m_RotationKeys.back().first * std::int64_t(mod);
		return time_stamp;
	}

	const glm::dvec3 AnimationNode::InterpolateScale(const KE::Duration & p_rTimeStamp)
	{
		const unsigned head_index = this->FindHeadScaleKeyIndex(p_rTimeStamp);
		const unsigned tail_index = this->FindTailScaleKeyIndex(p_rTimeStamp);
		return this->InterpolateScale(m_ScaleKeys[head_index], m_ScaleKeys[tail_index], p_rTimeStamp);
	}

	const glm::dvec3 AnimationNode::InterpolateTranslation(const KE::Duration & p_rTimeStamp)
	{
		const unsigned head_index = this->FindHeadTranslationKeyIndex(p_rTimeStamp);
		const unsigned tail_index = this->FindTailTranslationKeyIndex(p_rTimeStamp);
		return this->InterpolateTranslation(m_TranslationKeys[head_index], m_TranslationKeys[tail_index], p_rTimeStamp);
	}

	const glm::dquat AnimationNode::InterpolateRotation(const KE::Duration & p_rTimeStamp, const AnimationBehaviour p_Behaviour)
	{
		const unsigned head_index = this->FindHeadRotationIndex(p_rTimeStamp);
		const unsigned tail_index = this->FindTailRotationIndex(p_rTimeStamp);
		return this->InterpolateRotation(m_RotationKeys[head_index], m_RotationKeys[tail_index], p_rTimeStamp);
	}

	const glm::dvec3 AnimationNode::InterpolateScale
	(
		const AnimationScaleKey & p_rKeyL
		, const AnimationScaleKey & p_rKeyR
		, const KE::Duration & p_rTimeStamp
	)
	{
		const glm::dvec3 difference(p_rKeyR.second-p_rKeyL.second);
		const KE::Duration delta_t_stamp(p_rTimeStamp - p_rKeyL.first);
		const double percentage(delta_t_stamp/(p_rKeyR.first-p_rKeyL.first));
		return difference * percentage + p_rKeyL.second;
	}

	const glm::dvec3 AnimationNode::InterpolateTranslation
	(
		const AnimationTranslationKey & p_rKeyL
		, const AnimationTranslationKey & p_rKeyR
		, const KE::Duration & p_rTimeStamp
	)
	{
		const glm::dvec3 difference(p_rKeyR.second-p_rKeyL.second);
		const KE::Duration delta_t_stamp(p_rTimeStamp - p_rKeyL.first);
		const double percentage(delta_t_stamp/(p_rKeyR.first-p_rKeyL.first));
		return difference * percentage + p_rKeyL.second;
	}

	const glm::dquat AnimationNode::InterpolateRotation
	(
		const AnimationRotationKey & p_rKeyL
		, const AnimationRotationKey & p_rKeyR
		, const KE::Duration & p_rTimeStamp
		, const AnimationBehaviour p_Behaviour
	)
	{
		const KE::Duration delta_t_stamp(p_rTimeStamp - p_rKeyL.first);
		const double percentage(delta_t_stamp/(p_rKeyR.first-p_rKeyL.first));
		if (p_Behaviour & AnimationBehaviour::Sphereical)
			return glm::slerp(p_rKeyL.second, p_rKeyR.second, percentage);
		else // TODO : implement other interpolation type.
			return glm::slerp(p_rKeyL.second, p_rKeyR.second, percentage);
	}

	const unsigned AnimationNode::FindHeadScaleKeyIndex(const KE::Duration p_TimeStamp)
	{
		for (unsigned i = 0; i < m_ScaleKeys.size(); ++i)
		{
			if (p_TimeStamp > m_ScaleKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
	}

	const unsigned AnimationNode::FindTailScaleKeyIndex(const KE::Duration p_TimeStamp)
	{
		for (int i = m_ScaleKeys.size() - 1; i >= 0 ; --i)		{
			if (p_TimeStamp < m_ScaleKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
	}

	const unsigned AnimationNode::FindHeadTranslationKeyIndex(const KE::Duration p_TimeStamp)
	{
		for (unsigned i = 0; i < m_TranslationKeys.size(); ++i)
		{
			if (p_TimeStamp > m_TranslationKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
	}

	const unsigned AnimationNode::FindTailTranslationKeyIndex(const KE::Duration p_TimeStamp)
	{
		for (int i = m_TranslationKeys.size() - 1; i >= 0 ; --i)
		{
			if (p_TimeStamp < m_TranslationKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
	}

	const unsigned AnimationNode::FindHeadRotationIndex(const KE::Duration p_TimeStamp)
	{
		for (unsigned i = 0; i < m_RotationKeys.size(); ++i)
		{
			if (p_TimeStamp > m_RotationKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
	}

	const unsigned AnimationNode::FindTailRotationIndex(const KE::Duration p_TimeStamp)
	{
		for (int i = m_RotationKeys.size() - 1; i >= 0 ; --i)
		{
			if (p_TimeStamp < m_RotationKeys[i].first)
				return i;
		}
		assert(false); // shouldn't get here.
		return 0;
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

	void Animation::ComputePoses(const KE::Duration p_Elapsed)
	{
		m_Duration += p_Elapsed;
		for (KG::AnimationNode_SmartPtr anim_node_sp : m_Channels)
		{
			const KE::Duration scale_time_stamp(anim_node_sp->ComputeScaleTimeStamp(m_Duration));
			const glm::dmat4 s_mat(glm::scale(anim_node_sp->InterpolateScale(scale_time_stamp)));
			const KE::Duration translate_time_stamp(anim_node_sp->ComputeTranslationTimeStamp(m_Duration));
			const glm::dmat4 r_mat(glm::mat4_cast(anim_node_sp->InterpolateRotation(translate_time_stamp)));
			const KE::Duration rotate_time_stamp(anim_node_sp->ComputeRotationTimeStamp(m_Duration));
			const glm::dmat4 t_mat(glm::translate(anim_node_sp->InterpolateTranslation(rotate_time_stamp)));
			const glm::dmat4 final_mat( t_mat * r_mat * s_mat );

		}
	}

	AnimationChannels & Animation::GetChannels(void)
	{
		return m_Channels;
	}

	const std::int16_t Animation::GetBehaviour(void) const
	{
		return m_AnimationBehaviour;
	}

	Animation & Animation::SetBehaviour(const std::int16_t p_Behaviour)
	{
		m_AnimationBehaviour = p_Behaviour;
		return *this;
	}

} // KG ns
