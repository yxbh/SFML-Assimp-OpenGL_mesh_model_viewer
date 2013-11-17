#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include <string>
#include <cmath>
#include <memory>

namespace KE
{
namespace Utility
{
	/*! Get the absolute path to the executable. */
	extern std::string GetExecutablePath(void);

	/*! Convert a std::weak_ptr to std::shared_ptr. Return nullptr if expired. */
	template <class Type>
	std::shared_ptr<Type> MakeStrongPtr(std::weak_ptr<Type> p_wpPtr)
	{
		if (p_wpPtr.expired())
			return std::shared_ptr<Type>();
		return std::shared_ptr<Type>(p_wpPtr);
	}

} // Utility ns

namespace Math
{
	extern const double k_PI;
	template <typename T>
	inline const T DegreeToRadian(const T p_AngleInDegrees)
	{
		return static_cast<T>(k_PI * static_cast<double>(p_AngleInDegrees)/180.0);
	}

	template <typename T>
	inline const T RadianToDegree(const T p_AngleInRadian)
	{
		return static_cast<T>(static_cast<double>(p_AngleInRadian)*180.0/k_PI);
	}
} // Math ns

} // KE ns
