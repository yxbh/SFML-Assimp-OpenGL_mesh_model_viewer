#pragma once
#include "../common.hpp"
#include "../Message/IMessageHandler.hpp"
#include "../SystemEvent/ISystemEventHandler.hpp"
#include "../Event/IEvent.hpp"
#include "../../CITSProject/GUI.hpp"

namespace KE
{

	class GUISystem
		: public KE::ISystemEventHandler
		, public KE::IMessageHandler
	{
	private:
		KE::RenderDevice *			m_pRenderDevice;
		sfg::SFGUI					m_SFGUIContext;
		sfg::Desktop				m_SFGUIDesktop;

	public:
		GUISystem(void);
		~GUISystem(void);
		void Init(void);

		void RegisterRenderDevice(KE::RenderDevice * const p_pRenderDevice);

		void TestSetup(void)
		{
			//sfg::Window::Ptr window = sfg::Window::Create();
			//
			//// Create our label smart pointer.
			//sfg::Label::Ptr label;
			//// Create the label itself.
			//label = sfg::Label::Create();
			//// Set the text of the label.
			//label->SetText( "Hello World!\nAnother Line" );

			//// Add the label to the window.

			//// Windows are a subclass of the Bin widget type. Bins are only
			//// allowed to contain one child widget so adding more than 1 widget
			//// to a window will result in a warning and no effect. For adding more
			//// widgets to a window refer to the Box example later on. For a full
			//// widget hierarchy refer to the documentation.
			//window->Add( label );
			//m_SFGUIDesktop.Add( window );
			
		}

		void Update(const KE::Duration p_Duration);
		void Render(void);

		sfg::Desktop & GetSFGDesktop(void);

		const bool EventDelegate(const KE::Event_SmartPtr p_spEvent);

	private:
		virtual void ProcessEvents(void) final;
		virtual void ProcessMessages(void) final;

	};

	typedef std::shared_ptr<KE::GUISystem> GUISystem_SmartPtr;

} // KE ns