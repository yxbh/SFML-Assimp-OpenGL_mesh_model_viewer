#pragma once
#include "../../common.hpp"
#include "../../Entity/EntityID.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "MatrixStack.hpp"

namespace KG
{
	class SceneNode;
	typedef std::shared_ptr<KG::SceneNode> SceneNode_SmartPtr;
	typedef std::map<KE::EntityID, KG::SceneNode_SmartPtr> SceneNodeMap;
	class RootSceneNode;
	typedef std::shared_ptr<KG::RootSceneNode> RootSceneNode_SmartPtr;

	/*! \class Scene
	*/
	class Scene
	{
		friend class RenderSystem;

	protected:
		KG::SceneNode_SmartPtr		m_spRootNode;
		KG::SceneNodeMap			m_SceneNodeMap;

		KG::MatrixStack				m_MatrixStack;

		KG::Camera_SmartPtr			m_spCamera;
		KG::Renderer_SmartPtr		m_spRenderer;

	public:
		Scene(void);
		virtual ~Scene(void);

		virtual void InitDefault(void);

		virtual void UpdateAll(const KE::Duration p_Elapsed);
		virtual void RenderAll(void);
		virtual bool SceneReady(void) const;

		bool AddSceneNode(SceneNode_SmartPtr p_spNewSceneNode);
		bool RemoveSceneNode(const KE::EntityID p_ActorID);

		void SetCamera(KG::Camera_SmartPtr p_spNewCamera);
		void SetRenderer(KG::Renderer_SmartPtr p_pRenderer);
		void SetRootSceneNode(KG::RootSceneNode_SmartPtr p_spRootSceneNode);

		KG::SceneNode & GetRootSceneNode(void);
		KG::MatrixStack & GetMatrixStack(void);
		KG::Renderer & GetRenderer(void);
		KG::Camera & GetCamera(void);

	}; // class Scene

} // KG ns
