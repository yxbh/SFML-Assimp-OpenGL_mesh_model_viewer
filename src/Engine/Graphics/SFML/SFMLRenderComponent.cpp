#include "SFMLRenderComponent.hpp"

namespace KE
{
	SFMLRenderComponent::SFMLRenderComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::RenderComponent(p_spEntity)
		, m_Layer(5)
	{}

	SFMLRenderComponent::SFMLRenderComponent(const KE::RenderComponent & p_rEntityRenderComponent)
		: KE::RenderComponent(p_rEntityRenderComponent)
		, m_Layer(5)
	{}

	SFMLRenderComponent::~SFMLRenderComponent(void)
	{
	}

	void SFMLRenderComponent::operator=(KE::SFMLRenderComponent & p_rEntityRenderComponent)
	{
		this->GetEntity() = p_rEntityRenderComponent.GetEntity();
		m_Layer = p_rEntityRenderComponent.m_Layer;
	}

	void SFMLRenderComponent::Update(const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}


	
	SFMLRenderComponentNode::SFMLRenderComponentNode(KE::Entity_SmartPtr p_spEntity)
		: RenderComponentNode(p_spEntity)
	{
		for ( auto & node : m_Nodes)
			node = nullptr;
	}

	SFMLRenderComponentNode::SFMLRenderComponentNode(const KE::RenderComponentNode & p_rEntityRenderComponentNode)
		: RenderComponentNode(p_rEntityRenderComponentNode)
	{
	}

	void SFMLRenderComponentNode::operator=(KE::SFMLRenderComponentNode & p_rEntityRenderComponentNode)
	{
		this->RenderComponentNode::operator=(p_rEntityRenderComponentNode);
		m_Nodes = p_rEntityRenderComponentNode.m_Nodes;
	}

	void SFMLRenderComponentNode::Update(const KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}

	void SFMLRenderComponentNode::SetNode(KE::SFMLRenderComponentNode::NodePosition p_NodePosition, std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode)
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

	void SFMLRenderComponentNode::SetTopNode(std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode)
	{
		m_Nodes.at(NodePosition::Top) = p_NewNode;
	}

	void SFMLRenderComponentNode::SetBottomNode(std::shared_ptr<KE::SFMLRenderComponentNode> p_NewNode)
	{
		m_Nodes.at(NodePosition::Bottom) = p_NewNode;
	}

} // KE ns
