#pragma once
#include <SFML\System\Clock.hpp>
#include "..\Interface\ITime.hpp"

namespace KE
{
	/** \class SFML_Time
		
		A time utility class implementing the ITime interface using the SFML library's time related class.
	*/
	class SFML2_Time : ITime
	{
	private:
		sf::Clock m_clock;
		sf::Time m_FrozenDuration;
		bool m_Frozen;
	
	public:
		/*! Ctor */
		SFML2_Time(void)
			: ITime()
			, m_clock()
			, m_Frozen(false)
			, m_FrozenDuration()
		{}
		/*! Dtor */
		~SFML2_Time(void)
		{}

		/*! Freeze the internal timer/stopwatch/clock. Does nothing if already frozen.*/
		virtual void Freeze(void) final
		{
			if (!m_Frozen)
			{
				m_Frozen = true;
				m_FrozenDuration = m_clock.getElapsedTime();
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
			m_clock.restart();
		} // Restart()

		/*! Return the elapsed time in ms. Return duration between freeze point and last origin if frozen. */
		virtual const std::int64_t ElapsedAsMilliseconds(void) const final
		{
			if (Frozen())
				return static_cast<std::int64_t>(m_FrozenDuration.asMilliseconds());
			else
				return static_cast<std::int64_t>(m_clock.getElapsedTime().asMilliseconds());
		}
		/*! Return the elapsed time in microseconds. Return duration between freeze point and last origin if frozen. */
		virtual const std::int64_t ElapsedAsMicroseconds(void) const final
		{
			if (Frozen())
				return static_cast<std::int64_t>(m_FrozenDuration.asMicroseconds());
			else
				return static_cast<std::int64_t>(m_clock.getElapsedTime().asMicroseconds());
		}

		/*! Return the elapsed time in seconds. Return duration between freeze point and last origin if frozen. */
		virtual const double ElapsedAsSeconds(void) const final
		{
			if (Frozen())
				return static_cast<double>(m_FrozenDuration.asSeconds());
			else
				return static_cast<double>(m_clock.getElapsedTime().asSeconds());
		}

	}; // SFML_Time class
	
	typedef KE::SFML2_Time Timer; //using Time = std_Time; NOT SUPPORTED BY VC110

} // KE ns
