#pragma once

namespace KE
{
	inline Duration::Duration(void)
		: m_DurationInMicroseconds(0)
	{}

	inline Duration::Duration(const Value p_Value)
		: m_DurationInMicroseconds(0)
	{
		KE_UNREFERENCED_PARAMETER(p_Value);
	}

	inline const double Duration::AsSeconds(void) const
	{
		return static_cast<double>(m_DurationInMicroseconds)/1000000.0;
	}

	inline const std::int32_t Duration::AsMilliseconds(void) const
	{
		return static_cast<std::int32_t>(m_DurationInMicroseconds/1000);
	}

	inline const std::int64_t Duration::AsMicroseconds(void) const
	{
		return m_DurationInMicroseconds;
	}


	inline KE::Duration Duration::Seconds(const double p_Amount)
	{
		return KE::Duration(static_cast<std::int64_t>(p_Amount*1000000));
	}

	inline KE::Duration Duration::Milliseconds(const std::int32_t p_Amount)
	{
		return KE::Duration(static_cast<std::int64_t>(p_Amount*1000));
	}

	inline KE::Duration Duration::Microseconds(const std::int64_t p_Amount)
	{
		return KE::Duration(p_Amount);
	}

	inline Duration::Duration(const std::int64_t p_Microseconds)
		: m_DurationInMicroseconds(p_Microseconds) {}

	inline bool operator ==(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() == p_Right.AsMicroseconds();
	}

	inline bool operator !=(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() != p_Right.AsMicroseconds();
	}

	inline bool operator <(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() < p_Right.AsMicroseconds();
	}

	inline bool operator >(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() > p_Right.AsMicroseconds();
	}

	inline bool operator <=(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() <= p_Right.AsMicroseconds();
	}

	inline bool operator >=(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsMicroseconds() >= p_Right.AsMicroseconds();
	}

	inline const KE::Duration operator -(const KE::Duration p_Right)
	{
		return KE::Duration::Microseconds(-1*p_Right.AsMicroseconds());
	}

	inline const KE::Duration operator +(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return KE::Duration::Microseconds(p_Left.AsMicroseconds()+p_Right.AsMicroseconds());
	}

	inline const KE::Duration & operator +=(KE::Duration& p_Left, const KE::Duration p_Right)
	{
		return p_Left = p_Left + p_Right;
	}

	inline const KE::Duration operator -(const KE::Duration p_Left, const KE::Duration p_Right)
	{
		return KE::Duration::Microseconds(p_Left.AsMicroseconds()-p_Right.AsMicroseconds());
	}

	inline const KE::Duration & operator -=(KE::Duration& p_Left, const KE::Duration p_Right)
	{
		return p_Left = p_Left - p_Right;
	}

	inline const KE::Duration operator *(const KE::Duration p_Left, const double p_Right)
	{
		return KE::Duration::Seconds(p_Left.AsSeconds()*p_Right);
	}

	inline const KE::Duration operator *(const KE::Duration p_Left, const std::int64_t p_Right)
	{
		return KE::Duration::Microseconds(p_Left.AsMicroseconds()*p_Right);
	}

	inline const KE::Duration operator *(const double p_Left, const KE::Duration p_Right)
	{
		return KE::Duration::Seconds(p_Right.AsSeconds()*p_Left);
	}

	inline const KE::Duration operator *(const std::int64_t p_Left, const KE::Duration p_Right)
	{
		return KE::Duration::Microseconds(p_Right.AsMicroseconds()*p_Left);
	}

	inline const KE::Duration & operator *=(KE::Duration& p_Left, const double p_Right)
	{
		return p_Left = p_Left * p_Right;
	}

	inline const KE::Duration & operator *=(KE::Duration& p_Left, const std::int64_t p_Right)
	{
		return p_Left = p_Left * p_Right;
	}
	
	inline const double operator /(KE::Duration p_Left, const KE::Duration p_Right)
	{
		return p_Left.AsSeconds()/p_Right.AsSeconds();
	}

	inline const KE::Duration operator /(const KE::Duration p_Left, const double p_Right)
	{
		return KE::Duration::Seconds(p_Left.AsSeconds()/p_Right);
	}

	inline const KE::Duration operator /(const KE::Duration p_Left, const std::int64_t p_Right)
	{
		return KE::Duration::Microseconds(p_Left.AsMicroseconds()*p_Right);
	}

	inline const KE::Duration & operator /=(KE::Duration & p_rLeft, const double p_Right)
	{
		return p_rLeft = p_rLeft / p_Right;
	}

	inline const KE::Duration & operator /=(KE::Duration & p_rLeft, const std::int64_t p_Right)
	{
		return p_rLeft = p_rLeft / p_Right;
	}
	
} // KE ns
