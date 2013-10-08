#include "utility.hpp"

namespace KE
{
namespace Utility
{
	std::string GetExecutablePath(void)
	{
		char * path = new char[100]; std::string string_path;
	#if defined(_WIN32) || defined(_WIN64)
		GetModuleFileName(NULL, path, 100);
		string_path = path;
	#endif
		delete [] path;
		return string_path;
	}
}

namespace Math
{
	const double k_PI = 4*std::atan(1);

}

}