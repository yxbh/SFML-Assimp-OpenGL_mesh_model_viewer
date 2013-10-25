#include "../../../common.hpp"
#include "../SceneNode.hpp"

namespace KG
{

	/*! \class
	*/
	enum class AnimationState
	{
		InAction,
		NotInAction,
	};


	/*! \class
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
	typedef std::pair<KE::Duration, glm::dvec3> AnimationScaleKey;
	/*! \class AnimationPositionKey
		<Time, Position>
	*/
	typedef std::pair<KE::Duration, glm::dvec3> AnimationPositionKey;
	/*! \class AnimationOrientationKey
		<Time, Orientation>
	*/
	typedef std::pair<KE::Duration, glm::dquat> AnimationOrientationKey;
	/*! \class AnimationScaleKeyList
	*/
	typedef std::vector<AnimationScaleKey> AnimationScaleKeyList;
	/*! \class AnimationPositionKeyList
	*/
	typedef std::vector<AnimationPositionKey> AnimationPositionKeyList;
	/*! \class AnimationOrientationKeyList
	*/
	typedef std::vector<AnimationOrientationKey> AnimationOrientationKeyList;


	/*! \class AnimationNode

	*/
	class AnimationNode
		: public KG::SceneNode
	{
		friend class MeshLoader;

	private:
		AnimationScaleKeyList				m_ScaleKeys;
		AnimationPositionKeyList			m_PositionKeys;
		AnimationOrientationKeyList			m_OrientationKeys;
		AnimationState						m_AnimationState;
		std::int16_t						m_AnimationBehaviour;

	public:
		AnimationNode
		(
			const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		AnimationNode
		(
			AnimationScaleKeyList p_ScaleKeys
			, AnimationPositionKeyList p_PositionKeys
			, AnimationOrientationKeyList p_OrientationKeys
			, const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		virtual ~AnimationNode(void);

		const AnimationScaleKeyList & GetScaleKeys(void) const;
		const AnimationPositionKeyList & GetPositionKeys(void) const;
		const AnimationOrientationKeyList & GetOrientationKeys(void) const;
		const std::int16_t GetBehaviour(void) const;

		AnimationNode & SetBehaviour(const std::int16_t p_Behaviour);
	
	}; // class AnimationNode

	typedef std::shared_ptr<KG::AnimationNode> AnimationNode_SmartPtr;
	typedef std::weak_ptr<KG::AnimationNode> AnimationNode_WeakPtr;


	/*! \class Animation
	*/
	class Animation
		: public KG::SceneNode
	{
		friend class MeshLoader;

	private:
		std::vector<AnimationNode_SmartPtr>		m_Channels;

	public:
		Animation
		(
			const KE::EntityID p_EntityID
			, const KG::RenderPass p_RenderPass = KG::RenderPass::NotRendered
		);
		virtual ~Animation(void);

	}; // class Animation

	typedef std::shared_ptr<Animation> Animation_SmartPtr;
	typedef std::weak_ptr<Animation> Animation_WeakPtr;


} // KG ns
