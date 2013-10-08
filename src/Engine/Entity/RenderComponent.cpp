#include "RenderComponent.hpp"
#include "../Graphics/OpenGL/SceneNode.hpp"

namespace KE
{

	RenderComponent::~RenderComponent(void)
	{}

	void RenderComponent::Update(const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}

	const KE::EntityComponentID RenderComponent::GetID(void) const
	{
		return sk_ComponentID;
	}

	void RenderComponent::SetSceneNode(KG::SceneNode_SmartPtr p_spSceneNode)
	{
		m_spSceneNode = p_spSceneNode;
	}

	KG::SceneNode_SmartPtr RenderComponent::GetSceneNode(void)
	{
		return m_spSceneNode;
	}

	
	RenderComponentNode::RenderComponentNode(KE::Entity_SmartPtr p_spEntity)
		: KE::RenderComponent(p_spEntity)
	{
		for ( auto & node : m_Nodes)
			node = nullptr;
	}

	RenderComponentNode::RenderComponentNode(const KE::RenderComponentNode & p_rEntityRenderComponentNode)
		: RenderComponent(p_rEntityRenderComponentNode)
		, m_Nodes(p_rEntityRenderComponentNode.m_Nodes)
	{
	}

	void RenderComponentNode::operator=(KE::RenderComponentNode & p_rEntityRenderComponentNode)
	{
		this->RenderComponent::operator=(p_rEntityRenderComponentNode);
		m_Nodes = p_rEntityRenderComponentNode.m_Nodes;
	}

	void RenderComponentNode::SetNode(KE::RenderComponentNode::NodePosition p_NodePosition, std::shared_ptr<KE::RenderComponentNode> p_NewNode)
	{
		switch (p_NodePosition)
		{
		case NodePosition::Top:
			m_Nodes.at(NodePosition::Top) = p_NewNode; break;
		case NodePosition::Bottom:
			m_Nodes.at(NodePosition::Bottom) = p_NewNode; break;
		default:
			assert(false); break;
		}
	}

	void RenderComponentNode::SetTopNode(std::shared_ptr<KE::RenderComponentNode> p_NewNode)
	{
		m_Nodes.at(NodePosition::Top) = p_NewNode;
	}

	void RenderComponentNode::SetBottomNode(std::shared_ptr<KE::RenderComponentNode> p_NewNode)
	{
		m_Nodes.at(NodePosition::Bottom) = p_NewNode;
	}

} // KE ns
