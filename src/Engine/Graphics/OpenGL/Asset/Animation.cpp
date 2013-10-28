#include "Animation.hpp"

namespace KG
{
	AnimationNode::AnimationNode
	(
		const KE::EntityID p_EntityID
		, const KG::RenderPass p_RenderPass
	)
		: KG::SceneNode(p_EntityID, p_RenderPass)
		, m_SkeletonBoneIndex(0)
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
		, m_SkeletonBoneIndex(0)
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

	const glm::dmat4 & AnimationNode::GetTransform(void) const
	{
		return m_Transform;
	}

	const unsigned AnimationNode::GetBoneIndex(void) const
	{
		return m_SkeletonBoneIndex;
	}

	void AnimationNode::SetTransform(const glm::dmat4 & p_rTransform)
	{
		m_Transform = p_rTransform;
	}

	const KE::Duration AnimationNode::ComputeScaleTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		if (m_ScaleKeys.back().first != KE::Duration::Zero)
			return time_stamp % m_ScaleKeys.back().first;
		return KE::Duration::Zero;
	}

	const KE::Duration AnimationNode::ComputeTranslationTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		if (m_TranslationKeys.back().first != KE::Duration::Zero)
			return time_stamp % m_TranslationKeys.back().first;
		return KE::Duration::Zero;
	}

	const KE::Duration AnimationNode::ComputeRotationTimeStamp(const KE::Duration & p_rDuration)
	{
		KE::Duration time_stamp(p_rDuration);
		if (m_RotationKeys.back().first != KE::Duration::Zero)
			return time_stamp % m_RotationKeys.back().first;
		return KE::Duration::Zero;
	}

	const glm::dvec3 AnimationNode::InterpolateScale(const KE::Duration & p_rTimeStamp)
	{
		if (m_ScaleKeys.size() == 1) // only 1 key
			return m_ScaleKeys[0].second;

		int head_index, tail_index;
		if (this->FindHeadScaleKeyIndex(head_index, p_rTimeStamp) && this->FindTailScaleKeyIndex(tail_index, p_rTimeStamp))
		{
			if ((tail_index+1) < static_cast<int>(m_ScaleKeys.size()))
					++tail_index;
			return this->InterpolateScale(m_ScaleKeys[head_index], m_ScaleKeys[tail_index], p_rTimeStamp);
		}
		return glm::dvec3(1.0, 1.0, 1.0);
	}

	const glm::dvec3 AnimationNode::InterpolateTranslation(const KE::Duration & p_rTimeStamp)
	{
		if (m_TranslationKeys.size() == 1) // only 1 key
			return m_TranslationKeys[0].second;

		int head_index, tail_index;
		if (this->FindHeadTranslationKeyIndex(head_index, p_rTimeStamp) && this->FindTailTranslationKeyIndex(tail_index, p_rTimeStamp))
		{
			if ((tail_index+1) < static_cast<int>(m_TranslationKeys.size()))
					++tail_index;
			return this->InterpolateTranslation(m_TranslationKeys[head_index], m_TranslationKeys[tail_index], p_rTimeStamp);
		}
		return glm::dvec3(0.0, 0.0, 0.0);
	}

	const glm::dquat AnimationNode::InterpolateRotation(const KE::Duration & p_rTimeStamp, const AnimationBehaviour p_Behaviour)
	{
		if (m_RotationKeys.size() == 1) // only 1 key
			return m_RotationKeys[0].second;

		int head_index, tail_index;
		if (this->FindHeadRotationIndex(head_index, p_rTimeStamp) && this->FindTailRotationIndex(tail_index, p_rTimeStamp))
		{
			if (head_index == tail_index)
			{
				if ((tail_index+1) < static_cast<int>(m_RotationKeys.size()))
					++tail_index;
			}
			return this->InterpolateRotation(m_RotationKeys[head_index], m_RotationKeys[tail_index], p_rTimeStamp, p_Behaviour);
		}
		return glm::dquat();
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

	const bool AnimationNode::FindHeadScaleKeyIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_ScaleKeys.empty())
			return false;
		if (m_ScaleKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = m_ScaleKeys.size() - 1; p_rIndex >= 0 ; --p_rIndex)
		{
			if (p_TimeStamp <= m_ScaleKeys[p_rIndex].first)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
	}

	const bool AnimationNode::FindTailScaleKeyIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_ScaleKeys.empty())
			return false;
		if (m_ScaleKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = 1; p_rIndex < static_cast<int>(m_ScaleKeys.size()); ++p_rIndex) // have to start from 1 (not 0). or you will get could get a head = 0 and tail = 0.
		{
			if (m_ScaleKeys[p_rIndex].first >= p_TimeStamp)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
	}

	const bool AnimationNode::FindHeadTranslationKeyIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_TranslationKeys.empty())
			return false;
		if (m_TranslationKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = m_TranslationKeys.size() - 1; p_rIndex >= 0 ; --p_rIndex)
		{
			if (m_TranslationKeys[p_rIndex].first <= p_TimeStamp)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
	}

	const bool AnimationNode::FindTailTranslationKeyIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_TranslationKeys.empty())
			return false;
		if (m_TranslationKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = 1; p_rIndex < static_cast<int>(m_TranslationKeys.size()); ++p_rIndex) // have to start from 1 (not 0). or you will get could get a head = 0 and tail = 0.
		{
			if (m_TranslationKeys[p_rIndex].first >= p_TimeStamp)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
	}

	const bool AnimationNode::FindHeadRotationIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_RotationKeys.empty())
			return false;
		if (m_RotationKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = m_RotationKeys.size() - 1; p_rIndex >= 0 ; --p_rIndex)
		{
			if (m_RotationKeys[p_rIndex].first <= p_TimeStamp)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
	}

	const bool AnimationNode::FindTailRotationIndex(int & p_rIndex, const KE::Duration p_TimeStamp)
	{
		if (m_RotationKeys.empty())
			return false;
		if (m_RotationKeys.size() == 1)
		{
			p_rIndex = 0;
			return true;
		}
		for (p_rIndex = 1; p_rIndex < static_cast<int>(m_RotationKeys.size()); ++p_rIndex) // have to start from 1 (not 0). or you will get could get a head = 0 and tail = 0.
		{
			if (m_RotationKeys[p_rIndex].first >= p_TimeStamp)
				return true;
		}
		assert(false); // shouldn't get here.
		return false;
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
			// scale
			const KE::Duration scale_time_stamp(anim_node_sp->ComputeScaleTimeStamp(m_Duration));
			const glm::dmat4 s_mat(glm::scale(anim_node_sp->InterpolateScale(scale_time_stamp)));
			// translation
			const KE::Duration translate_time_stamp(anim_node_sp->ComputeTranslationTimeStamp(m_Duration));
			const glm::dmat4 t_mat(glm::translate(anim_node_sp->InterpolateTranslation(translate_time_stamp)));
			//rotation
			const KE::Duration rotate_time_stamp(anim_node_sp->ComputeRotationTimeStamp(m_Duration));
			const glm::dmat4 r_mat(glm::mat4_cast(anim_node_sp->InterpolateRotation(rotate_time_stamp)));
			// final
			const glm::dmat4 final_mat( t_mat * r_mat * s_mat );
			anim_node_sp->SetTransform(final_mat);
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

	void Animation::SetDuration(const KE::Duration p_Duration)
	{
		m_Duration = p_Duration;
	}

} // KG ns
