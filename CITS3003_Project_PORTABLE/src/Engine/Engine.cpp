#include "Engine.hpp"
#include "../Utility/Time.hpp"

namespace KE
{
	Engine::Config::Config(void)
	{
		/* initialize Engine parameters. */
		LoopMaxTickThres		= KE::Duration::Seconds(5);
		EventTick				= KE::Duration::Milliseconds(0); // anything > 0ms will introduce lag into renderer in dual thread mode.
		EventTickCountThres		= 1;
		InputTick				= KE::Duration::Milliseconds(5); 
		InputTickCountThres		= 1; 
		PhysicsTick				= KE::Duration::Milliseconds(30);
		PhysicsTickCountThres	= 3;
		RenderTick				= KE::Duration::Milliseconds(10);
		RenderTickCountThres	= 2;
		LogicTick				= KE::Duration::Milliseconds(20);
		LogicTickCountThres		= 3;
	}

	Engine * Engine::s_pEngine = nullptr;
	const char * Engine::sk_DefaultResourceConfigPath = "???";	

	Engine::Engine(void)
		: m_ThreadMode(ThreadMode::Undefined)
		, m_pMainLoopThread(nullptr)
		, m_Status(EngineStatus::OK)
		, m_EngineConfig()
		, m_RenderDevice()
		, m_EntityManager()
		, m_pMessageManager(nullptr)
		, m_RenderSystem(&m_RenderDevice)
	{
		assert(s_pEngine == nullptr);
		s_pEngine = this;
	}

	Engine::~Engine(void)
	{
		//if (m_pMainLoopThread != nullptr) delete m_pMainLoopThread; // <-- crash if I delete thread
	}

	void Engine::Init(void)
	{
		assert(this->InitRenderSystem()); // Render system must be initialized first
		assert(this->InitInputSystem());
		assert(this->InitEventManager());
		assert(this->InitMessageSystem());
	}

	void Engine::ShutDown(void)
	{
		bool trying_to_exit = true;
		while (trying_to_exit && m_ThreadMode == ThreadMode::Multi)
		{
			if (m_pMainLoopThread != nullptr && m_pMainLoopThread->joinable())
			{
				m_pMainLoopThread->join();
				break;
			}
		}
		this->ShutDownRenderSystem();
	}

	void Engine::BeginEventLoop(void)
	{
		KE::Timer clock;
		KE::Duration accumulatedTick(KE::Duration::Zero);		// time elapsed during last tick.
		while (m_Status == EngineStatus::OK)
		{
			accumulatedTick = KE::Duration::Microseconds(clock.ElapsedAsMicroseconds()); clock.Restart();

			// fixed maximum tick time.
			if ( accumulatedTick > m_EngineConfig.LoopMaxTickThres )
				accumulatedTick = m_EngineConfig.LoopMaxTickThres;

			m_SysEventManager.Update(m_EngineConfig.EventTick);
		}
	}

	void Engine::BeginMainLoopThread(void)
	{
		m_RenderDevice.setActive(false); // deactive device on main thread.
		m_pMainLoopThread = new std::thread(&Engine::EnterMainLoop, this);
	}

	void Engine::EnterMainLoop(void)
	{
		KE::Timer timer;
		m_RenderDevice.setActive(true);
		KE::Duration accumulatedTick(KE::Duration::Zero);		// time elapsed during last tick.
		KE::Duration InputTickAccumulator(KE::Duration::Zero);
		int InputTickCount = 0;
		int RenderTickCount = 0;
		KE::Duration RenderTickAccumulator(KE::Duration::Zero);
		int LogicTickCount = 0;
		KE::Duration LogicTickAccumulator(KE::Duration::Zero);
		while (m_Status == EngineStatus::OK && m_RenderDevice.isOpen())
		{
			//static unsigned tick_count = 0; ++tick_count; KE::Debug::print("Engine tick: " + std::to_string(tick_count));

			accumulatedTick = KE::Duration::Microseconds(timer.ElapsedAsMicroseconds()); timer.Restart();
			// fixed maximum tick time.
            if ( accumulatedTick > m_EngineConfig.LoopMaxTickThres )
				accumulatedTick = m_EngineConfig.LoopMaxTickThres;

 			this->ProcessEvents(); // process engine events
			this->ProcessMessages();

            m_pMessageManager->Update(m_EngineConfig.EventTick);

			CallBack1(*this);
			if (m_Status == Engine::EngineStatus::Exit)
				break;

			// Input
			InputTickAccumulator += accumulatedTick;
			InputTickCount = 0;
			while ( InputTickAccumulator >= m_EngineConfig.EventTick
				&& InputTickCount <= m_EngineConfig.EventTickCountThres)
			{
				UpdateInput(m_EngineConfig.EventTick);
				InputTickAccumulator -= m_EngineConfig.EventTick;
				++InputTickCount;
			}

			// Entities & Events
			LogicTickAccumulator += accumulatedTick;
			LogicTickCount = 0;
			while ( LogicTickAccumulator >= m_EngineConfig.LogicTick
				&& LogicTickCount <= m_EngineConfig.LogicTickCountThres)
			{
				m_EventManager.Update(m_EngineConfig.LogicTick);
				m_EntityManager.UpdateAll(m_EngineConfig.InputTick);
				LogicTickAccumulator -= m_EngineConfig.LogicTick;
				++LogicTickCount;
			}
			
			// Render
			RenderTickAccumulator += accumulatedTick;
			RenderTickCount = 0;
			while ( RenderTickCount <= m_EngineConfig.RenderTickCountThres
				&& RenderTickAccumulator >= m_EngineConfig.RenderTick
				)
			{
				UpdateRender(m_EngineConfig.RenderTick);
				RenderTickAccumulator -= m_EngineConfig.RenderTick;
				++RenderTickCount;
			}

			m_RenderDevice.setTitle
			(
				std::to_string(accumulatedTick.AsMicroseconds())
				+ "MicroSec/Tick @ "
				+ std::to_string(m_RenderSystem.GetRenderer().GetSize().window_width)
				+ "X"+std::to_string(m_RenderSystem.GetRenderer().GetSize().window_height)
			);
		}

		if (m_Status == EngineStatus::Exit)
			KE::Debugger::print(" Normal engine loop exit. ");
		else
			KE::Debugger::print(KE::Debugger::msg_type::DBG_ERROR, " Abnormal engine loop exit. ");

	} // EnterMainLoop

	void Engine::BeginSingleThreadLoop(void)
	{
		KE::Timer timer;
		m_RenderDevice.setActive();
		KE::Duration accumulatedTick(KE::Duration::Zero);		// time elapsed during last tick.
		KE::Duration InputTickAccumulator(KE::Duration::Zero);
		int InputTickCount = 0;
		int RenderTickCount = 0;
		KE::Duration RenderTickAccumulator(KE::Duration::Zero);
		int LogicTickCount = 0;
		KE::Duration LogicTickAccumulator(KE::Duration::Zero);
		while (m_Status == EngineStatus::OK)
		{
			accumulatedTick = KE::Duration::Microseconds(timer.ElapsedAsMicroseconds()); timer.Restart();
			// fixed maximum tick time.
			if ( accumulatedTick > m_EngineConfig.LoopMaxTickThres )
				accumulatedTick = m_EngineConfig.LoopMaxTickThres;

			m_SysEventManager.Update(m_EngineConfig.EventTick);
			m_pMessageManager->Update(m_EngineConfig.EventTick);

			this->ProcessEvents(); // process engine events
			this->ProcessMessages();			

			CallBack1(*this);
			if (m_Status == Engine::EngineStatus::Exit)
				break;

			// Input
			InputTickAccumulator += accumulatedTick;
			InputTickCount = 0;
			while ( InputTickAccumulator >= m_EngineConfig.EventTick
				&& InputTickCount <= m_EngineConfig.EventTickCountThres)
			{
				UpdateInput(m_EngineConfig.EventTick);
				InputTickAccumulator -= m_EngineConfig.EventTick;
				++InputTickCount;
			}

			// Entities & Events
			LogicTickAccumulator += accumulatedTick;
			LogicTickCount = 0;
			while ( LogicTickAccumulator >= m_EngineConfig.LogicTick
				&& LogicTickCount <= m_EngineConfig.LogicTickCountThres)
			{
				m_EventManager.Update(m_EngineConfig.LogicTick);
				m_EntityManager.UpdateAll(m_EngineConfig.LogicTick);
				LogicTickAccumulator -= m_EngineConfig.LogicTick;
				++LogicTickCount;
			}
			
			// Render
			RenderTickAccumulator += accumulatedTick;
			RenderTickCount = 0;
			static unsigned frame_count = 0;
			while ( RenderTickCount <= m_EngineConfig.RenderTickCountThres
				&& RenderTickAccumulator >= m_EngineConfig.RenderTick
				)
			{
				UpdateRender(m_EngineConfig.RenderTick);
				RenderTickAccumulator -= m_EngineConfig.RenderTick;
				++RenderTickCount;
				++frame_count;
			}

			static unsigned tick_count = 0; ++tick_count;
			m_RenderDevice.setTitle
			(
				std::to_string(frame_count)
				+ " tick count: "
				+ std::to_string(tick_count)
				+ " "
				+ std::to_string(accumulatedTick.AsMicroseconds())
				+ "MicroSec/Tick @ "
				+ std::to_string(m_RenderSystem.GetRenderer().GetSize().window_width)
				+ "X"+std::to_string(m_RenderSystem.GetRenderer().GetSize().window_height)
			);

		}

		if (m_Status == EngineStatus::Exit)
			KE::Debugger::print(" Normal engine loop exit. ");
		else
			KE::Debugger::print(KE::Debugger::msg_type::DBG_ERROR, " Abnormal engine loop exit. ");
	}

	void Engine::Run(unsigned p_Flag)
	{
		enum {SingleThread = 0, MultiThread = 1};
		KE::Debug::print("Engine : ignite engine. ");
		switch(p_Flag)
		{
		case MultiThread:
			BeginMainLoopThread();
			BeginEventLoop();
			break;
		case SingleThread:
			BeginSingleThreadLoop();
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	void Engine::Do(int p_Command)
	{
		switch (p_Command)
		{
		case -1:
			m_Status = Engine::EngineStatus::Exit;
			break;
		}
	}

	Engine::Config & Engine::GetConfig(void)
	{
		return m_EngineConfig;
	}

	KE::EntityManager & Engine::GetEntityManager(void)
	{
		return m_EntityManager;
	}

	KE::RenderDevice & Engine::GetRenderDevice(void)
	{
		return m_RenderDevice;
	}

	KG::RenderSystem & Engine::GetRenderSystem(void)
	{
		return m_RenderSystem;
	}

	void Engine::CallBack1(KE::Engine & p_rEngine)
	{
		if (KE::Input::KeyPressed(KE::Keyboard::Key::Escape))
			p_rEngine.Do(-1);
	}

	bool Engine::InitEventManager(void)
	{
		m_SysEventManager.RegisterInputDevice(&m_RenderDevice);
		m_SysEventManager.Register(this, KE::SystemEventManager::EngineSystemType::EngineBackBone);
		m_SysEventManager.Register(&m_RenderSystem, KE::SystemEventManager::EngineSystemType::RenderSystem);
		m_SysEventManager.Register(&m_RenderSystem.GetGUISystem(), KE::SystemEventManager::EngineSystemType::GUISystem);
		m_SysEventManager.Register(&m_RenderSystem.GetRenderer(), KE::SystemEventManager::EngineSystemType::Renderer);
		m_SysEventManager.Register(&m_InputManager, KE::SystemEventManager::EngineSystemType::InputSystem);
		
		return true;
	}

	bool Engine::InitMessageSystem(void)
	{
		m_pMessageManager = &KE::MessageManager::Create();
		m_pMessageManager->RegisterInputDevice(&m_RenderDevice);
		m_pMessageManager->Register(this, KE::MessageManager::EngineSystemType::EngineBackBone);
		m_pMessageManager->Register(&m_RenderSystem, KE::MessageManager::EngineSystemType::RenderSystem);
		m_pMessageManager->Register(&m_RenderSystem.GetGUISystem(), KE::MessageManager::EngineSystemType::GUISystem);
		m_pMessageManager->Register(&m_RenderSystem.GetRenderer(), KE::MessageManager::EngineSystemType::Renderer);
		m_pMessageManager->Register(&m_InputManager, KE::MessageManager::EngineSystemType::InputSystem);
		return true;
	}

	bool Engine::InitRenderSystem(void)
	{
		m_RenderSystem.GetRenderer().Initalize();
		KE::Debug::print(KE::Debug::msg_type::DBG_DEFAULT, m_RenderSystem.GetRenderer().GetAPIVersion());
		return true;
	}

	void Engine::ShutDownRenderSystem(void)
	{
		//m_RenderSystem.GetRenderer().ShutDown();
	}

	bool Engine::InitInputSystem(void)
	{
		m_InputManager.RegisterAppWindow(&m_RenderDevice);
		m_InputManager.Init();
		return true;
	}

	void Engine::UpdateInput(KE::Duration p_ElapsedTime)
	{
		m_InputManager.Update(p_ElapsedTime);
	}

	void Engine::UpdatePhysics(KE::Duration p_ElapsedTime)
	{
		KE_UNREFERENCED_PARAMETER(p_ElapsedTime);
	}

	void Engine::UpdateRender(KE::Duration p_ElapsedTime)
	{
		m_RenderSystem.Update(p_ElapsedTime);KE::Debug::check_for_GL_error();
		m_RenderSystem.UpdateGUI(p_ElapsedTime);KE::Debug::check_for_GL_error();
		m_RenderSystem.UpdateScene(p_ElapsedTime);KE::Debug::check_for_GL_error();
		
		m_RenderSystem.GetRenderer().Clear();KE::Debug::check_for_GL_error();
		m_RenderSystem.RenderScene();KE::Debug::check_for_GL_error();
		m_RenderSystem.RenderGUI();KE::Debug::check_for_GL_error();
		m_RenderSystem.GetRenderer().Display();KE::Debug::check_for_GL_error();
	}

	void Engine::ProcessEvents(void)
	{
		KE::SystemEvent new_event;
		while (this->GetEventInbox().Poll(new_event))
		{
			switch (new_event.type)
			{
			case sf::Event::Closed:
				m_Status = EngineStatus::Exit;
			}
		}
	}

	void Engine::ProcessMessages(void)
	{
		this->EmptyMessageInbox();
	}

} // KE ns
