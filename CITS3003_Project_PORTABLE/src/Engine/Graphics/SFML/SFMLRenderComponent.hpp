#pragma once
#include "../../common.hpp"
#include "../../Entity/RenderComponent.hpp"

namespace KE
{
	class Entity;

	/** \class EntityRenderComponent
		The render component of an Entity. Belonging to the render world.
	*/
	class SFMLRenderComponent
		: public KE::RenderComponent
	{
	protected:
		int m_Layer;

	public:
		/*! Ctor. */
		SFMLRenderComponent(KE::Entity_SmartPtr p_spEntity);
		/*! Copy ctor. ID and master should be reassigned. */
		SFMLRenderComponent(const KE::RenderComponent & p_rEntityRenderComponent);
		/*! Dtor */
		~SFMLRenderComponent(void);
		/*! Copy assignment. */
		void operator=(KE::SFMLRenderComponent & p_rEntityRenderComponent);

		/*! Draw object to sf::RenderWindow using sf::Drawable draw() API. */
		virtual void draw(sf::RenderTarget & p_rRenderTarget, sf::RenderStates p_RenderStates = sf::RenderStates::Default) const
		{KE_UNREFERENCED_PARAMETER(p_rRenderTarget); KE_UNREFERENCED_PARAMETER(p_RenderStates);}
		/*! Draw object to the render window. */
		virtual void Draw(KE::RenderDevice & p_rRenderWindow)
		{KE_UNREFERENCED_PARAMETER(p_rRenderWindow);}
		/*! Update animation frame etc. */
		virtual void Update(const KE::Duration p_ElapsedTime) override;

	};

	/** \class EntityRenderComponentNode
	*/
	class SFMLRenderComponentNode
		: public KE::RenderComponentNode
	{
	public:
		enum NodePosition {Top = 0, Bottom = 1, Left, Right};

	public:
		/*! Ctor. */
		SFMLRenderComponentNode(KE::Entity_SmartPtr p_spEntity);
		/*! Copy ctor. */
		SFMLRenderComponentNode(const KE::RenderComponentNode & p_rEntityRenderComponentNode);
		/*! Copy assignment. */
		void operator=(KE::SFMLRenderComponentNode & p_rEntityRenderComponentNode);
		/*! Add node to the specificed position. */
		void SetNode(NodePosition p_NodePosition,  std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode);
		/*! Add node to top position. */
		void SetTopNode(std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode);
		/*! Add node to bottom position. */
		void SetBottomNode(std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode);

		/*! Draw node to render window. Should only be called by Draw()*/
	private: virtual void draw(sf::RenderTarget & p_rRenderTarget, sf::RenderStates p_RenderStates = sf::RenderStates::Default) const
			 {KE_UNREFERENCED_PARAMETER(p_rRenderTarget); KE_UNREFERENCED_PARAMETER(p_RenderStates);} public:
		/*! Draw node to render window. */
		virtual void Draw(KE::RenderDevice & p_rRenderWindow)
		{KE_UNREFERENCED_PARAMETER(p_rRenderWindow);}
		/*! Update animation frame etc. */
		virtual void Update(const KE::Duration p_ElapsedTime) override;

	}; // EntityRenderComponentNode class


} // KE ns
