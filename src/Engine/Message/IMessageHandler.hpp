#pragma once
#include "MessageQueue.hpp"

namespace KE
{
	/*! \class IMessageHandler
		Inherit from this class to receive and send KE::Messages.
	*/
	class IMessageHandler
	{
		friend class MessageManager;

	private:
		KE::MessageQueue	m_MessageInbox, m_MessageOutbox;

	protected:
		void PushMessageToInbox(const KE::Message & p_Msg)
		{
			m_MessageInbox.Push(p_Msg);
		}

		void PushMessageToOutbox(const KE::Message & p_Msg)
		{
			m_MessageOutbox.Push(p_Msg);
		}

		KE::MessageQueue & GetMessageInbox(void)
		{
			return m_MessageInbox;
		}

		KE::MessageQueue & GetMessageOutbox(void)
		{
			return m_MessageOutbox;
		}

		void EmptyMessageInbox(void)
		{
			KE::Message new_msg;
			while (this->GetMessageInbox().Poll(new_msg)) {}
		}

		void EmptyMessageOutbox(void)
		{
			KE::Message new_msg;
			while (this->GetMessageOutbox().Poll(new_msg)) {}
		}

		virtual void ProcessMessages(void)
		{
			this->EmptyMessageInbox();
		}

	};

} // KE ns
