#pragma once
#include "../../common.hpp"
#include "RenderPass.hpp"
#include "../../Entity/EntityID.hpp"
#include "Transform.hpp"


namespace KE
{
	class RenderComponent;
	typedef std::weak_ptr<RenderComponent> RenderComponent_WeakPtr;
} // KE ns

namespace KG
{
	class SceneNode;
	typedef std::shared_ptr<SceneNode> SceneNode_SmartPtr;
	typedef std::vector<KG::SceneNode_SmartPtr> SceneNodeSPList; // vector for random access
	class ShaderProgram;
	typedef std::shared_ptr<KG::ShaderProgram> ShaderProgram_SmartPtr;

	/*! \class SceneNode
		
		Basic building blocks of the render system's scene graph and/or any tree data structures in game or engine.
		Not meant to be used directly.

		Currently, each individual SceneNode and it's children should have a different and unique EntityID.

		Know issues:
			- each SceneNode holds only a raw pointer to it's parent SceneNode. Returning the parent node could become
			a dangerous operation where parent could be destoryed first via a smart pointer or whatever what something
			else could still hold a reference to this pointer. Try to avoid use cases like this.

	*/
	class SceneNode
		:public KG::Transform
	{
		friend class Scene;
		friend class Renderer;

	protected:
		KE::EntityID						m_ID;
		std::string							m_Name;
		KG::SceneNode::RenderPass			m_RenderPass;
		KE::RenderComponent_WeakPtr			m_wpRenderComponent;
		KG::SceneNode *						m_pParentSceneNode;
		KG::SceneNodeSPList					m_ChildSceneNodeList;
		glm::dmat4							m_CurrentToWorldMatix; // AKA model matrix. Object space -> world space.

		KG::ShaderProgram_SmartPtr			m_spShaderProgram;

	public:
		SceneNode(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::Null);
		SceneNode(const KG::SceneNode & p_rSceneNode);
		virtual ~SceneNode(void);

		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed);
		virtual void UpdateChildren(KG::Scene & p_rScene, const KE::Duration p_Elapsed);
		virtual void UpdateAll(KG::Scene & p_rScene, const KE::Duration p_Elapsed);
		/*! push model matrix into stack and set own model matrix to that of the top of the stack. */
		virtual void PreRender(KG::Scene & p_rScene);
		/*! Pass scenenode into the renderer. */
		virtual void Render(KG::Scene & p_rScene);
		/*! clean up. pop statck. */
		virtual void PostRender(KG::Scene & p_rScene);
		virtual void RenderChildren(KG::Scene & p_rScene);
		virtual void RenderAll(KG::Scene & p_rScene);

		virtual bool AddChild(KG::SceneNode_SmartPtr p_spSceneNode);
		/*! Attempt to remove immediate child with ID. Does not check grandchildren. */
		virtual bool RemoveChild(const KE::EntityID p_EntityID);
		/*! recursively search for the scenenode and remove if found. VERY expensive! */
		bool RecursiveRemove(const KE::EntityID p_EntityID);

		SceneNode * const GetChildNode(const KE::EntityID p_EntityID);
		virtual KG::SceneNode * const GetParentNode(void) const;
		KG::ShaderProgram_SmartPtr GetShaderProgram(void);
		const KE::EntityID GetEntityID(void) const;
		const std::string & GetName(void) const;
		const RenderPass GetRenderPass(void) const;
		KG::SceneNodeSPList & GetChildSceneNodeList(void);
		const glm::dmat4 & GetCurrentToWorldTransform(void);

		void SetEntityID(const KE::EntityID p_ID);
		void SetName(const std::string & p_rName);
		void SetRenderPass(const RenderPass & p_rRenderPass);
		void SetParentNode(KG::SceneNode_SmartPtr p_spNewParentSceneNode);
		void SetParentNode(KG::SceneNode * const p_pNewParentSceneNode);
		/*! Sets the current model matrix. Used for immediate rendering purpose. Matrix should be updated every frame. */
		void SetCurrentToWorldMatrix(const glm::dmat4 & p_rModelMat4);
		void SetShaderProgram(KG::ShaderProgram_SmartPtr p_spShaderProgram);

	}; // SceneNode class 

	/*! \class RootSceneNode
	*/
	class RootSceneNode
		: public KG::SceneNode
	{
	private:

	public:
		RootSceneNode(const KE::EntityID & p_EntityID = KE::EntityIDGenerator::NewID(), const RenderPass & p_RenderPass = RenderPass::NotRendered);
		virtual ~RootSceneNode(void);

		virtual bool AddChild(KG::SceneNode_SmartPtr p_spSceneNode) final;

	private:
		RootSceneNode(const KG::RootSceneNode & p_rRootSceneNode);
	}; // RootSceneNode class 

} // KG ns
