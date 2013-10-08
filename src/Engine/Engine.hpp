#ifndef _KE_ENGINE_
#define _KE_ENGINE_
#include "common.hpp"
#include "Graphics/OpenGL/RenderSystem.hpp"
#include "SystemEvent/SystemEventManager.hpp"
#include "Message/MessageManager.hpp"
#include "Message/IMessageHandler.hpp"
#include "SystemEvent/ISystemEventHandler.hpp"
#include "Event/EventManager.hpp"
#include "Input/InputManager.hpp"
#include "Entity/EntityManager.hpp"

namespace KE
{

	/** \class Engine
		Central hub of all engine components.
	*/
	class Engine
		: public KE::IMessageHandler
		, public KE::ISystemEventHandler
	{
	public:
		enum class EngineStatus {OK, ShuttingDown, Exit, ExitError};
		enum class ThreadMode { Undefined = 0, Single = 1 << 0, Multi = 1 << 1};

	private:
		static const char *			sk_DefaultResourceConfigPath;
		static Engine *				s_pEngine;

		std::string					m_CurrentWorkingDirectoy;
		volatile EngineStatus		m_Status;
		ThreadMode					m_ThreadMode;

		std::thread *				m_pMainLoopThread; // EXPERIMENTAL

		KE::RenderDevice			m_RenderDevice;
		KE::InputManager			m_InputManager;
		KE::SystemEventManager		m_SysEventManager;
		KE::MessageManager *		m_pMessageManager;
		KE::EventManager			m_EventManager;
		KE::EntityManager			m_EntityManager;

		KG::RenderSystem			m_RenderSystem;

		struct Config
		{
			static int	tickTimeMutiplier;
			Duration	LoopMaxTickThres;		/*! Maximum duration for a single tick. */
			Duration	EventTick;
			int			EventTickCountThres;
			Duration	InputTick;				/*! Input respond rate. */ 
			int			InputTickCountThres;
			Duration	PhysicsTick;
			int			PhysicsTickCountThres;
			Duration	RenderTick;
			int			RenderTickCountThres;
			Duration	LogicTick;
			int			LogicTickCountThres;

			Config(void);
		};
		Config			m_EngineConfig;

	public:
		/*! Ctor */
		Engine(void);
		/*! Dtor. Clean up pointers to Engine components. */
		~Engine(void);

		/* Setup components */
		void Init(void);
		/*! Shut down engine. */
		void ShutDown(void);
		/*! Create a new thread for main loop and pass BeginMainLoop to the thread. Thread life time is the enitre game. */
		void BeginMainLoopThread(void);
		/*! Begin the event loop on the main thread. */
		void BeginEventLoop(void);
		/*! Enter the main game loop. This is where Logic, Physics and Render take place. */
		void EnterMainLoop(void);
		/*! This is the single thread version of the game loop. */
		void BeginSingleThreadLoop(void);
		/*! Enter engine/game loop. This loop only exits when application exits. */
		void Run(unsigned p_Flag);
		/*! Do something given command. */
		void Do(int p_Command);

		/*! Return reference to engine's config struct. */
		Config & GetConfig(void);
		/*! Return reference to engine's EntityManager. */
		KE::EntityManager & GetEntityManager(void);
		/*! Return reference to engine's RenderDevice. */
		RenderDevice & GetRenderDevice(void);
		/*! */
		KG::RenderSystem & GetRenderSystem(void);

		void CallBack1(KE::Engine & p_rEngine);

	private:
		/*!*/
		bool InitEventManager(void);
		/*!*/
		bool InitMessageSystem(void);
		/*! Create the Render window. */
		bool InitRenderSystem(void);
		/*! Close the Render window. */
		void ShutDownRenderSystem(void);
		/*! Setup engine InputComponent. */
		bool InitInputSystem(void);

		void UpdateInput(KE::Duration p_ElapsedTime = KE::Duration::Zero);
		void UpdatePhysics(KE::Duration p_ElapsedTime);
		void UpdateRender(KE::Duration p_ElapsedTime);

		virtual void ProcessEvents(void) final;
		virtual void ProcessMessages(void) final;
	}; // Engine class

} // KE ns


#endif