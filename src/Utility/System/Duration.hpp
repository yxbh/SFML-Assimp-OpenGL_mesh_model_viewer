#pragma once
#include <cstdint>
#define KE_UNREFERENCED_PARAMETER(X) X

namespace KE
{

	/*! \class Duration
		A class which defines a period of time (a duration). The duration can be netgative.
		The duration can be evalued to abirtary precision (seconds(double), milliseconds(in32) and microseconds(int64).

		Modeled after sf::Time.
	*/
	class Duration
	{
	public:
		enum class Value {Zero};
		static const Duration Zero;

	private:
		std::int64_t	m_DurationInMicroseconds;

	public:
		Duration(void);
		Duration(const Value p_Value);
		const double AsSeconds(void) const;
		const std::int32_t AsMilliseconds(void) const;
		const std::int64_t AsMicroseconds(void) const;

		static KE::Duration Seconds(const double p_Amount);
		static KE::Duration Milliseconds(const std::int32_t p_Amount);
		static KE::Duration Microseconds(const std::int64_t p_Amount);
		
	private:
		explicit Duration(const std::int64_t p_Microseconds);

	};

	bool operator ==(const KE::Duration p_Left, const KE::Duration p_Right);
	bool operator !=(const KE::Duration p_Left, const KE::Duration p_Right);
	bool operator <(const KE::Duration p_Left, const KE::Duration p_Right);
	bool operator >(const KE::Duration p_Left, const KE::Duration p_Right);
	bool operator <=(const KE::Duration p_Left, const KE::Duration p_Right);
	bool operator >=(const KE::Duration p_Left, const KE::Duration p_Right);
	const KE::Duration operator -(const KE::Duration p_Right);
	const KE::Duration operator +(const KE::Duration p_Left, const KE::Duration p_Right);
	const KE::Duration & operator +=(KE::Duration & p_Left, const KE::Duration p_Right);
	const KE::Duration operator -(const KE::Duration p_Left, const KE::Duration p_Right);
	const KE::Duration & operator -=(KE::Duration & p_Left, const KE::Duration p_Right);
	const KE::Duration operator *(const KE::Duration p_Left, const double p_Right);
	const KE::Duration operator *(const KE::Duration p_Left, const std::int64_t p_Right);
	const KE::Duration operator *(const double p_Left, const KE::Duration p_Right);
	const KE::Duration operator *(const std::int64_t p_Left, const KE::Duration p_Right);
	const KE::Duration & operator *=(KE::Duration & p_Left, const double p_Right);
	const KE::Duration & operator *=(KE::Duration & p_Left, const std::int64_t p_Right);
	const double operator /(KE::Duration p_Left, const KE::Duration p_Right);
	const KE::Duration operator /(const KE::Duration p_Left, const double p_Right);
	const KE::Duration operator /(const KE::Duration p_Left, const std::int64_t p_Right);
	const KE::Duration & operator /=(KE::Duration & p_rLeft, const double p_Right);
	const KE::Duration & operator /=(KE::Duration & p_rLeft, const std::int64_t p_Right);

} // KE ns

#include "Duration.inl"
