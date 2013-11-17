#ifndef _CONSOLE_DBG_
#define _CONSOLE_DBG_
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

// macros
#define DBG_PRINT_IGNORE(X) X
#define KE_UNREFERENCED_PARAMETER( PARAMETER ) PARAMETER

namespace KE
{

	/** \class Debugger
		A static class that only prints messeges to the console window at the moment.
	*/
	class Debugger
	{
	public:
		enum msg_type {DBG_DEFAULT = 0, DBG_OK, DBG_WARNING, DBG_ERROR};

	private:
		static bool s_output_to_file;

	public:
		/* ctor, does nothing at the moment.*/
		Debugger(void){}
		/*! dtor*/
		~Debugger(void){}
		
		/*! call print() with a "Default" debug output header.*/
		static void print(const std::string & p_msg);
		/*! call print() with a specified debug output header.*/
		static void print(msg_type p_msg_type, const std::string & p_msg);

	}; // KE::Debugger class

	template<class Type>
	std::string dbg_toString(Type p_val)
	{
		std::ostringstream oss;
		oss << p_val;
		return oss.str();
	} // dbg_toString()

namespace Debug
{
	enum msg_type
	{
		DBG_DEFAULT = 0,
		DBG_OK		= 1,
		DBG_WARNING = 2,
		DBG_ERROR	= 3,
	};

	/*! call print() with a "Default" debug output header.*/
	extern void print(const std::string & p_msg);
	/*! call print() with a specified debug output header.*/
	extern void print(msg_type p_msg_type, const std::string & p_msg);
	/*! Clear the console screen. */
	extern void clear_console(void);

	extern void check_for_GL_error(void);
}
	
} //KE ns

#endif