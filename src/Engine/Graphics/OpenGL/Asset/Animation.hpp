#include "../../../common.hpp"
#include "../SceneNode.hpp"

namespace KG
{

	typedef KE::Duration TimeStamp;


	/*! \class AnimationState
		Whether an Animation is in action or not.
	*/
	enum class AnimationState
	{
		InAction,
		NotInAction,
	};


	/*! \class AnimationBehaviour
	*/
	enum AnimationBehaviour
	{
		Flat = 2,
		Linear = 4,
		Sphereical = 8, // ??
		Repeat = 16,
		Default = Flat|Linear|Repeat,
	};

	/*! \class AnimationScaleKey
		<Time, Scale>
	*/
	struct AnimationScaleKey
	{
		TimeStamp time_stamp;
		glm::dvec3 scale;
		AnimationScaleKey(void)
		{}
		AnimationScaleKey(const TimeStamp p_TimeStamp, const glm::dvec3 & p_rScale)
			: time_stamp(p_TimeStamp), scale(p_rScale) 
		{}
	};

	/*! \class AnimationPositionKey
		<TimeStamp, Position>
	*/
	struct AnimationTranslationKey
	{
		TimeStamp time_stamp;
		glm::dvec3 translation;
		AnimationTranslationKey(void)
		{}
		AnimationTranslationKey(const TimeStamp p_TimeStamp, const glm::dvec3 & p_rScale)
			: time_stamp(p_TimeStamp), translation(p_rScale) 
		{}
	};

	/*! \class AnimationOrientationKey
		<Time, Orientation>
	*/
	struct AnimationRotationKey
	{
		TimeStamp time_stamp;
		glm::dquat rotation;
		AnimationRotationKey(void)
		{}
		AnimationRotationKey(const TimeStamp p_TimeStamp, const glm::dquat & p_rScale)
			: time_stamp(p_TimeStamp), rotation(p_rScale) 
		{}
	};

	/*! \class AnimationScaleKeyList
	*/
	typedef std::vector<AnimationScaleKey> AnimationScaleKeyList;
	/*! \class AnimationPositionKeyList
	*/
	typedef std::vector<AnimationTranslationKey> AnimationTranslationKeyList;
	/*! \class AnimationOrientationKeyList
	*/
	typedef std::vector<AnimationRotationKey> AnimationRotationKeyList;


	/*! \class AnimationNode

		An AnimationNode corresponds to a single BoneNode in a Skeleton, where the AnimationNode animates the BoneNode.

		Each scale/position/orientation is not corresponded to a position/scale/orientation.
		Number of each key types are not equal. i.e. Number of ScaleKeys could be different to the number of PositionKeys.

	*/
	class AnimationNode
		: public KG::SceneNode
	{
		friend class MeshLoader;

	private:
		AnimationScaleKeyList				m_ScaleKeys;
		AnimationTranslationKeyList			m_TranslationKeys;
		AnimationRotationKeyList			m_RotationKeys;
		glm::dmat4							m_Transform;
		unsigned							m_SkeletonBoneIndex;	// index to bone arrays in the Skeleton.

	public:
		AnimationNode
		(
			const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		AnimationNode
		(
			AnimationScaleKeyList p_ScaleKeys
			, AnimationTranslationKeyList p_TranslationKeys
			, AnimationRotationKeyList p_RotationKeys
			, const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		virtual ~AnimationNode(void);

		const AnimationScaleKeyList & GetScaleKeys(void) const;
		const AnimationTranslationKeyList & GetTranslationKeys(void) const;
		const AnimationRotationKeyList & GetRotationKeys(void) const;
		const glm::dmat4 & GetTransform(void) const;
		const unsigned GetBoneIndex(void) const;

		void SetTransform(const glm::dmat4 & p_rTransform);

		const KE::Duration ComputeScaleTimeStamp(const KE::Duration & p_rDuration);
		const KE::Duration ComputeTranslationTimeStamp(const KE::Duration & p_rDuration);
		const KE::Duration ComputeRotationTimeStamp(const KE::Duration & p_rDuration);

		const glm::dvec3 InterpolateScale(const KE::Duration & p_rTimeStamp);
		const glm::dvec3 InterpolateTranslation(const KE::Duration & p_rTimeStamp);
		const glm::dquat InterpolateRotation(const KE::Duration & p_rTimeStamp, const AnimationBehaviour p_Behaviour = AnimationBehaviour::Flat);
		const glm::dvec3 InterpolateScale(const AnimationScaleKey & p_rKeyL, const AnimationScaleKey & p_rKeyR, const KE::Duration & p_rTimeStamp);
		const glm::dvec3 InterpolateTranslation(const AnimationTranslationKey & p_rKeyL, const AnimationTranslationKey & p_rKeyR, const KE::Duration & p_rTimeStamp);
		const glm::dquat InterpolateRotation(const AnimationRotationKey & p_rKeyL, const AnimationRotationKey & p_rKeyR, const KE::Duration & p_rTimeStamp, const AnimationBehaviour p_Behaviour = AnimationBehaviour::Flat);

	private:
		/*! Find the two indices pointing to the head and tail key frames. Both indices will be equal if there is only one keyframe.*/
		const bool FindScaleKeyFrameIndices(int & p_rHeadIndex, int & p_rTailIndex, const KE::Duration p_TimeStamp);
		/*! Find the two indices pointing to the head and tail key frames. Both indices will be equal if there is only one keyframe.*/
		const bool FindTranslationKeyFrameIndices(int & p_rHeadIndex, int & p_rTailIndex, const KE::Duration p_TimeStamp);
		/*! Find the two indices pointing to the head and tail key frames. Both indices will be equal if there is only one keyframe.*/
		const bool FindRotationKeyFrameIndices(int & p_rHeadIndex, int & p_rTailIndex, const KE::Duration p_TimeStamp);
	
	}; // class AnimationNode

	typedef std::shared_ptr<KG::AnimationNode> AnimationNode_SmartPtr;
	typedef std::weak_ptr<KG::AnimationNode> AnimationNode_WeakPtr;

	typedef std::vector<AnimationNode_SmartPtr> AnimationChannels;


	/*! \class Animation

		Animation is an abstract representation of a animation sequences for one or multiple bones.

		Each channel has a single node corresponding to a BoneNode. Each AnimationNode animates a BoneNode.

		*/
	class Animation
		: public KG::SceneNode
	{
		friend class MeshLoader;

	private:
		AnimationChannels	m_Channels; // each channel has a single node corresponding to a BoneNode. Each AnimationNode animates a BoneNode.
		AnimationState		m_AnimationState;
		std::int16_t		m_AnimationBehaviour;
		KE::Duration		m_Duration;			// total duration of the animation in real time.

	public:
		Animation
		(
			const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		virtual ~Animation(void);

		void ComputePoses(const KE::Duration p_Elapsed);

		AnimationChannels & GetChannels(void);
		const std::int16_t GetBehaviour(void) const;

		Animation & SetBehaviour(const std::int16_t p_Behaviour);
		void SetDuration(const KE::Duration p_Duration);

	}; // class Animation

	typedef std::shared_ptr<Animation> Animation_SmartPtr;
	typedef std::weak_ptr<Animation> Animation_WeakPtr;


} // KG ns
