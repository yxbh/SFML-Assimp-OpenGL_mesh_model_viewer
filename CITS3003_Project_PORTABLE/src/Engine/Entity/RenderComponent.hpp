#pragma once
#include "../common.hpp"
#include "IEntityComponent.hpp"

namespace KG
{
	class SceneNode;
	typedef std::shared_ptr<KG::SceneNode> SceneNode_SmartPtr;
	typedef std::weak_ptr<KG::SceneNode> SceneNode_WeakPtr;
}

namespace KE
{
	class Entity;

	/** \class EntityRenderComponent
		The render component of an Entity. Belonging to the render world.
	*/
	class RenderComponent
		: public KE::IEntityComponent
	{
	public:
		const static KE::EntityComponentID sk_ComponentID = 1000;
	protected:
		KG::SceneNode_SmartPtr		m_spSceneNode;

	public:
		RenderComponent(KE::Entity_SmartPtr p_spEntity) : KE::IEntityComponent(p_spEntity), m_spSceneNode() {}
		/*! Dtor */
		virtual ~RenderComponent(void);

		/*! Update animation frame etc. */
		virtual void Update(const KE::Duration p_ElapsedTime);
	
		virtual const KE::EntityComponentID GetID(void) const;

		void SetSceneNode(KG::SceneNode_SmartPtr p_spSceneNode);

		KG::SceneNode_SmartPtr GetSceneNode(void);
	};

	typedef std::shared_ptr<KE::RenderComponent> RenderComponent_SmartPtr;
	typedef std::weak_ptr<KE::RenderComponent> RenderComponent_WeakPtr;

	/** \class EntityRenderComponentNode
	*/
	class RenderComponentNode
		: public KE::RenderComponent
	{
	public:
		enum NodePosition {Top = 0, Bottom = 1, Left, Right};

	protected:
		std::array<std::shared_ptr<KE::RenderComponentNode>, 2> m_Nodes;

	public:
		RenderComponentNode(KE::Entity_SmartPtr p_spEntity);
		virtual ~RenderComponentNode(void) {};
		/*! Copy ctor. */
		RenderComponentNode(const KE::RenderComponentNode & p_rEntityRenderComponentNode);
		/*! Copy assignment. */
		void operator=(KE::RenderComponentNode & p_rEntityRenderComponentNode);
		/*! Add node to the specificed position. */
		void SetNode(NodePosition p_NodePosition,  std::shared_ptr<KE::RenderComponentNode> p_NewNode);
		/*! Add node to top position. */
		void SetTopNode(std::shared_ptr<KE::RenderComponentNode> p_NewNode);
		/*! Add node to bottom position. */
		void SetBottomNode(std::shared_ptr<KE::RenderComponentNode> p_NewNode);

	}; // EntityRenderComponentNode class


} // KE ns
