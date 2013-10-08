#pragma once
#include "..\Interface\ITime.hpp"
#include <chrono>
#include <stdexcept>

namespace KE
{
	/** \class std_Time
		
		A time utility class implementing the ITime interface using the std's chrono library.
	*/
	class std_Time : public KE::ITime
	{
	private:
		std::chrono::high_resolution_clock::time_point m_TimeOrigin;
		std::chrono::high_resolution_clock::time_point m_TimeFreezePoint;
		bool m_Frozen;

	public:
		/*! Ctor. Starts the stopwatch/timer/clock. */
		std_Time(void)
			: ITime(), m_TimeOrigin(std::chrono::high_resolution_clock::now())
			, m_TimeFreezePoint(m_TimeOrigin)
			, m_Frozen(false) {} // Ctor
		/*! Dtor. Does nothing.*/
		~std_Time(void) {} // Dtor

		/*! Freeze the internal timer/stopwatch/clock. Does nothing if already frozen.*/
		virtual void Freeze(void) final
		{
			if (!m_Frozen)
			{
				m_Frozen = true;
				m_TimeFreezePoint = std::chrono::high_resolution_clock::now();
			}			
		}
		/*! Un-freeze the internal timer/stopwatch/clock. */
		virtual void Unfreeze(void) final
		{
			m_Frozen = false;
		}
		/*! Return true if Time is frozen. */
		virtual bool Frozen(void) const final
		{
			return m_Frozen;
		}

		/*! Restart the internal time.*/
		virtual void Restart(void) final
		{
			m_TimeOrigin = std::chrono::high_resolution_clock::now();
		} // Restart()

		/*! Return the elapsed time in ms. Return duration between freeze point and last origin if frozen. */
		virtual const std::int64_t ElapsedAsMilliseconds(void) const final
		{
			if (Frozen())
				return static_cast<std::int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(m_TimeFreezePoint - m_TimeOrigin).count());
			else
				return static_cast<std::int64_t>(
					std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::high_resolution_clock::now() - m_TimeOrigin
					).count()
				);
		}
		virtual const std::int64_t ElapsedAsMicroseconds(void) const final
		{
			if (Frozen())
				return static_cast<std::int64_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_TimeFreezePoint - m_TimeOrigin).count());
			else
				return static_cast<std::int64_t>(
					std::chrono::duration_cast<std::chrono::microseconds>(
						std::chrono::high_resolution_clock::now() - m_TimeOrigin
					).count()
				);
		}

		/*! Return the elapsed time in seconds. Return duration between freeze point and last origin if frozen. */
		virtual const double ElapsedAsSeconds(void) const final
		{
			if (Frozen())
				return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_TimeFreezePoint - m_TimeOrigin).count())/1000;
			else
				return static_cast<double>
				(
					std::chrono::duration_cast<std::chrono::milliseconds>
					(
						std::chrono::high_resolution_clock::now() - m_TimeOrigin
					).count()
				)/1000;
		}

	}; // std_Time class
		
	typedef KE::std_Time Time; //using Time = std_Time; NOT SUPPORTED BY VC110

} // KE ns

