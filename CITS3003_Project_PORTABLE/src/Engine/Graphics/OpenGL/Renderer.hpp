#pragma once
#include "../../common.hpp"
#include "../../Message/IMessageHandler.hpp"
#include "../../SystemEvent/ISystemEventHandler.hpp"
#include "RenderPass.hpp"

namespace KG
{
	struct size
	{
		unsigned window_width;
		unsigned window_height;
		unsigned viewport_width;
		unsigned viewport_height;
		size(unsigned p_WW, unsigned p_WH, unsigned p_VW, unsigned p_VH) : window_width(p_WW), window_height(p_WH), viewport_width(p_VW), viewport_height(p_VH) {}
	};

	class Scene;
	class SceneNode;
	typedef std::shared_ptr<KG::SceneNode>						SceneNode_SmartPtr;
	typedef std::vector<KG::SceneNode*const>					SceneNodeSPVector;
	typedef std::map<KG::RenderPass, KG::SceneNodeSPVector>		RenderMap;
	class ShaderProgram;
	typedef std::shared_ptr<KG::ShaderProgram>					ShaderProgram_SmartPtr;


	/** \class Renderer
		Handles window creation and destruction.
		TODO: investigate whether KE::RenderDevice should be constructed in here and pass back to engine or the other way around.
			UPDATE: entire engine will be refactored and rewritten to be more aligned like the GCC engine. RenderDevice
					will hopefully be a static object in the View portion of the Engine.
	*/
	class Renderer
		: public KE::IMessageHandler
		, public KE::ISystemEventHandler
	{
		friend class RenderSystem;

	private:
		static const double			sk_DefaultAspectRatio;
		static const unsigned		sk_DefaultWidth;
		static const unsigned		sk_DefaultHeight;
		double						m_CurrentAspectRatio;
		unsigned int				m_CurrentWindowWidth, m_CurrentWindowHeight;
		unsigned int				m_CurrentViewportWidth, m_CurrentViewportHeight;
		unsigned int				m_ViewportDifferenceX, m_ViewportDifferenceY;
		KE::RenderDevice *			m_pRenderDevice;
		bool						m_RenderDeviceRegistered;
		bool						m_Initialized;
		KG::ShaderProgram_SmartPtr	m_spShaderProgram;

		KG::RenderMap				m_RenderMap;

	public:
		Renderer(void);
		~Renderer(void);

		void Initalize(void);
		void ShutDown(void);

		void RegisterRenderDevice(KE::RenderDevice * const p_pRenderDevice);

		void Update(const KE::Duration p_ElapsedTime); /*! whats this for?? */
		void Clear(const float p_R = 0.0f, const float p_G = 0.0f, const float p_B = 0.0f, const float p_A = 0.0f) const;
		/*! prepare for render. */
		void PreRender(void);
		/*!  add scene node into render map for render passes. This method does not actually render/draw the object yet. */
		void Render(KG::Scene & p_rScene, KG::SceneNode * const p_pSceneNode);
		/*! Render meshes into internal buffer. */
		void RenderPassMesh(void);
		/*! Render shadows into internal buffer. */
		void RenderPassShadow(void);
		/*! post render clean up. */
		void PostRender(void);
		/*! */
		void DoRenderPasses(void);
		void Display(void) const;

		const double GetAspectRatio(void) const;
		const size GetSize(void) const;
		const std::string GetAPIVersion(void) const;

		void SetAspectRatio(const double p_Ratio);
		void SetViewportSize(GLint p_X,GLint p_Y, GLsizei p_Width, GLsizei p_Height);
		void SetWindowSize(const unsigned p_NewWindowWidth, const unsigned p_NewWindowHeight);
		/*! Set renderer to be active on the current thread. */
		void SetActive(const bool p_Active);

	private:
		bool Avaliable(void) const;
		virtual void ProcessMessages(void) final;
		virtual void ProcessEvents(void) final;
	};

	typedef std::shared_ptr<KG::Renderer> Renderer_SmartPtr;

} // KE ns
