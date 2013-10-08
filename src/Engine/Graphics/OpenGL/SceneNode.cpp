#include "SceneNode.hpp"
#include "Scene.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Renderer.hpp"

namespace KG
{
	SceneNode::SceneNode(const KE::EntityID p_EntityID, const KG::RenderPass p_RenderPass)
		: m_ID(p_EntityID)
		, m_Name("N/A")
		, m_RenderPass(p_RenderPass)
		, m_pParentSceneNode(nullptr)
		, m_spShaderProgram(nullptr)
	{}

	SceneNode::SceneNode(const KG::SceneNode & p_rSceneNode)
		: m_ID(p_rSceneNode.m_ID)
		, m_Name(p_rSceneNode.m_Name)
		, m_RenderPass(p_rSceneNode.m_RenderPass)
		, m_pParentSceneNode(p_rSceneNode.m_pParentSceneNode)
		, m_ChildSceneNodeList(p_rSceneNode.m_ChildSceneNodeList)
		, m_spShaderProgram(p_rSceneNode.m_spShaderProgram)
	{}

	SceneNode::~SceneNode(void)
	{
		KE::EntityIDGenerator::Remove(this->GetEntityID());
	}

	void SceneNode::Update(Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		KE_UNREFERENCED_PARAMETER(p_Elapsed); KE_UNREFERENCED_PARAMETER(p_rScene);
	}

	void SceneNode::UpdateChildren(KG::Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		for (auto & it : m_ChildSceneNodeList)
			it->Update(p_rScene, p_Elapsed);
	}

	void SceneNode::UpdateAll(KG::Scene & p_rScene, const KE::Duration p_Elapsed)
	{
		this->Update(p_rScene, p_Elapsed);
		for (auto & it : m_ChildSceneNodeList)
			it->UpdateAll(p_rScene, p_Elapsed);
	}

	void SceneNode::PreRender(KG::Scene & p_rScene)
	{
		KG::MatrixStack & stack_ref = p_rScene.GetMatrixStack(); 
		stack_ref.Push();
		stack_ref.SetMatrix(this->GetGLMMatd());
		this->SetModelMatrix(stack_ref.Top());
	}

	void SceneNode::Render(KG::Scene & p_rScene)
	{
		p_rScene.GetRenderer().Render(p_rScene, this);
	}

	void SceneNode::PostRender(KG::Scene & p_rScene)
	{
		p_rScene.GetMatrixStack().Pop();
	}

	void SceneNode::RenderChildren(KG::Scene & p_rScene)
	{
		this->PreRender(p_rScene);
		for (auto & it : m_ChildSceneNodeList)
			it->Render(p_rScene);
		this->PostRender(p_rScene);
	}

	void SceneNode::RenderAll(KG::Scene & p_rScene)
	{
		this->PreRender(p_rScene);
		this->Render(p_rScene);
		for (auto & it : m_ChildSceneNodeList)
			it->RenderAll(p_rScene);
		this->PostRender(p_rScene);
	}

	bool SceneNode::AddChild(KG::SceneNode_SmartPtr p_spSceneNode)
	{
		if (p_spSceneNode == nullptr)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "SceneNode : Adding nullptr Child.");
			return false;
		}
		m_ChildSceneNodeList.push_back(p_spSceneNode);
		return true;
	}

	bool SceneNode::RemoveChild(const KE::EntityID p_EntityID)
	{
		auto it = m_ChildSceneNodeList.begin();
		for (; it != m_ChildSceneNodeList.end(); ++it)
		{
			const KE::EntityID id = (*it)->GetEntityID();
			if (id != KE::Invalid_EntityID && id == p_EntityID)
			{
				it = m_ChildSceneNodeList.erase(it); return true;
			}
		}
		return false;
	}

	bool SceneNode::RecursiveRemove(const KE::EntityID p_EntityID)
	{
		SceneNodeList::iterator it = m_ChildSceneNodeList.begin();
		while (it != m_ChildSceneNodeList.end())
		{
			if (it->get()->GetEntityID() == p_EntityID)
			{
				m_ChildSceneNodeList.erase(it);
				return true;
			}
			else
				it->get()->RecursiveRemove(p_EntityID);
			++it;
		}
		return false;
	}

	SceneNode * const SceneNode::GetChildNode(const KE::EntityID p_EntityID)
	{
		for (const SceneNode_SmartPtr & it : m_ChildSceneNodeList)
		{
			const KE::EntityID id = it->GetEntityID();
			if (id != KE::Invalid_EntityID && id == p_EntityID)
				return it.get();
		}
		return nullptr;
	}

	const glm::dmat4 SceneNode::GetModelMatrix(void) const
	{
		return m_ModelMatrix;
	}

	KG::SceneNode * const SceneNode::GetParentNode(void) const
	{
		return m_pParentSceneNode;
	}

	KG::ShaderProgram_SmartPtr SceneNode::GetShaderProgram(void)
	{
		if (!m_spShaderProgram)
			KE::Debug::print(KE::Debug::DBG_ERROR, "ShaderProgram is nullptr!");
		return m_spShaderProgram;
	}

	const KE::EntityID KG::SceneNode::GetEntityID(void) const
	{
		return m_ID;
	}

	const KG::RenderPass SceneNode::GetRenderPass(void) const
	{
		return m_RenderPass;
	}

	KG::SceneNodeList & SceneNode::GetChildSceneNodeList(void)
	{
		return m_ChildSceneNodeList;
	}

	void SceneNode::SetID(const KE::EntityID p_ID)
	{
		m_ID = p_ID;
	}

	void SceneNode::SetName(const std::string & p_rName)
	{
		m_Name = p_rName;
	}

	void SceneNode::SetRenderPass(const KG::RenderPass & p_rRenderPass)
	{
		m_RenderPass = p_rRenderPass;
	}

	void SceneNode::SetParentNode(KG::SceneNode_SmartPtr p_spNewParentSceneNode)
	{
		this->SetParentNode(p_spNewParentSceneNode.get());
	}

	void SceneNode::SetParentNode(KG::SceneNode * const p_pNewParentSceneNode)
	{
		m_pParentSceneNode = p_pNewParentSceneNode;
	}

	void SceneNode::SetModelMatrix(const glm::dmat4 p_ModelMat4)
	{
		m_ModelMatrix = std::move(p_ModelMat4);
	}

	void SceneNode::SetShaderProgram(KG::ShaderProgram_SmartPtr p_spShaderProgram)
	{
		m_spShaderProgram = p_spShaderProgram;
	}	

	RootSceneNode::RootSceneNode(const KE::EntityID & p_EntityID, const RenderPass & p_RenderPass)
		: SceneNode(p_EntityID, p_RenderPass)
	{
		unsigned number_of_groups = static_cast<short>(KG::RenderPass::Size);
		auto & child_scenenode_list = this->GetChildSceneNodeList();
		child_scenenode_list.reserve(number_of_groups);

		for (int i = 0; i < int(number_of_groups); ++i)
		{
			KG::SceneNode_SmartPtr new_scenenode(new KG::SceneNode(KE::EntityIDGenerator::NewID(), KG::RenderPass::NotRendered));
			child_scenenode_list.push_back(new_scenenode);
		}
	}

	RootSceneNode::~RootSceneNode(void)
	{}

	bool RootSceneNode::AddChild(KG::SceneNode_SmartPtr p_spSceneNode)
	{
		unsigned renderpass = static_cast<unsigned>(p_spSceneNode->GetRenderPass());
		auto & child_scenenode_list = this->GetChildSceneNodeList(); 
		if (renderpass >= child_scenenode_list.size() || !child_scenenode_list[renderpass])
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "RootSceneNode : RenderPass out of bounds.");
			return false;
		}
		return child_scenenode_list[renderpass]->AddChild(p_spSceneNode);
	}

} // KG ns
