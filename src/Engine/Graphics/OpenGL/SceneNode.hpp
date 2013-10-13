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
	typedef std::vector<KG::SceneNode_SmartPtr> SceneNodeList; // vector for random access
	class ShaderProgram;
	typedef std::shared_ptr<KG::ShaderProgram> ShaderProgram_SmartPtr;

	/*! \class SceneNode
		Not meant to be used directly.
	*/
	class SceneNode
		:public KG::Transform
	{
		friend class SceneNodeProperties;
		friend class Scene;
		friend class Renderer;

	protected:
		KE::EntityID						m_ID;
		std::string							m_Name;
		KG::SceneNode::RenderPass			m_RenderPass;
		KE::RenderComponent_WeakPtr			m_wpRenderComponent;
		KG::SceneNode *						m_pParentSceneNode;
		KG::SceneNodeList					m_ChildSceneNodeList;

		KG::ShaderProgram_SmartPtr			m_spShaderProgram;
		glm::dmat4							m_ModelMatrix;			// to_world matrix. object space -> world space
		glm::dmat4							m_InverseModelMatrix;	// does the opposite to above.

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
		const glm::dmat4 GetModelMatrix(void) const;
		virtual KG::SceneNode * const GetParentNode(void) const;
		KG::ShaderProgram_SmartPtr GetShaderProgram(void);
		const KE::EntityID GetEntityID(void) const;
		const std::string & GetName(void) const;
		const RenderPass GetRenderPass(void) const;
		KG::SceneNodeList & GetChildSceneNodeList(void);

		void SetID(const KE::EntityID p_ID);
		void SetName(const std::string & p_rName);
		void SetRenderPass(const RenderPass & p_rRenderPass);
		void SetParentNode(KG::SceneNode_SmartPtr p_spNewParentSceneNode);
		void SetParentNode(KG::SceneNode * const p_pNewParentSceneNode);
		void SetModelMatrix(const glm::dmat4 p_ModelMat4); // test std::move
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
