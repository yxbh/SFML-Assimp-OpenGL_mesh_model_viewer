#include "dbg.hpp"

bool KE::Debugger::s_output_to_file = true;

namespace KE
{


	Debugger debugger;

	void Debugger::print(const std::string & p_msg)
	{
		return print(msg_type::DBG_DEFAULT, p_msg);
	}

	void Debugger::print(msg_type p_msg_type, const std::string & p_msg)
	{
	#if defined _DEBUG || defined KE_DEBUG
		std::string msg_header;
		switch (p_msg_type)
		{
		case DBG_OK:
			msg_header = "OK";	break;
		case DBG_WARNING:
			msg_header = "WARNING"; break;
		case DBG_ERROR:
			msg_header = "ERROR";	break;
		default:
			msg_header = "DEFAULT";
		}
		std::cout << "DEBUG(" << msg_header << "): " << p_msg << std::endl;
		std::cout.flush();
	#else
		DBG_PRINT_IGNORE(p_msg_type); DBG_PRINT_IGNORE(p_msg);
	#endif
	} // Debugger::print()


namespace Debug
{
	void print(const std::string & p_msg)
	{
		return print(msg_type::DBG_DEFAULT, p_msg);
	}

	void print(msg_type p_msg_type, const std::string & p_msg)
	{
	#if defined _DEBUG || defined KE_DEBUG
		std::string msg_header;
		switch (p_msg_type)
		{
		case DBG_OK:
			msg_header = "OK";	break;
		case DBG_WARNING:
			msg_header = "WARNING"; break;
		case DBG_ERROR:
			msg_header = "ERROR";	break;
		default:
			msg_header = "DEFAULT";
		}
		if (p_msg_type == DBG_ERROR)
		{
			std::cerr << "DEBUG(" << msg_header << "): " << p_msg << std::endl;
			std::cerr.flush();
		}
		else
		{
			std::cout << "DEBUG(" << msg_header << "): " << p_msg << std::endl;
			std::cout.flush();
		}
	#else
		DBG_PRINT_IGNORE(p_msg_type); DBG_PRINT_IGNORE(p_msg);
	#endif
	}

	void clear_console(void)
	{
	#if defined(_WIN32) || defined(_WIN64)
		system("CLS");
	#else
		for (int i = 0; i < 20; ++i) // not tested.
			std::clog << '\n';
		std::clog.flush();
	#endif
	}

	void check_for_GL_error(void)
	{
		#if defined _DEBUG || defined KE_DEBUG
		GLint error = glGetError();
		bool errored = false;
		while (error != GL_NO_ERROR)
		{
			errored = true;
			switch(error)
			{
			case GL_NO_ERROR: break;
			case GL_INVALID_ENUM:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_OUT_OF_MEMORY");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_INVALID_FRAMEBUFFER_OPERATION");
				break;
			case GL_TABLE_TOO_LARGE: // deprecated since 3.0
				KE::Debug::print(KE::Debug::DBG_ERROR, "GL_TABLE_TOO_LARGE");
				break;
			}
			error = glGetError();
		}

		if (errored) assert(false);
		#endif
	}

} // Debug ns
} // KE ns
