#pragma once
#include <cstddef> // std::size_t
#include <cstdint> // std::int64_t

namespace KE
{
	/** \class ITime
		Time interface for implementing a class which acts sort of as a stop watch for getting elapsed time.
	*/
	class ITime
	{
	public:
		ITime(void) {}
		virtual ~ITime(){}
	
		/*! Freeze the internal timer/stopwatch/clock. Does nothing if already frozen.*/
		virtual void Freeze(void) = 0;
		/*! Un-freeze the internal timer/stopwatch/clock. */
		virtual void Unfreeze(void) = 0;
		/*! Return true if Time is frozen. */
		virtual bool Frozen(void) const = 0;
		/*! Restart the internal time.*/
		virtual void Restart(void) = 0;

		/*! Return the elapsed time in ms. Return duration between freeze point and last origin if frozen. */
		virtual const std::int64_t ElapsedAsMilliseconds(void) const = 0;
		/*! Return the elapsed time in microseconds. Return duration between freeze point and last origin if frozen. */
		virtual const std::int64_t ElapsedAsMicroseconds(void) const = 0;
		/*! Return the elapsed time in seconds. Return duration between freeze point and last origin if frozen. */
		virtual const double ElapsedAsSeconds(void) const = 0;

	}; // ITime class


	/*
		Might be ideal to wrap duration up into a duration class.
		Could implement features such as negative time for "reverse time" game features. (e.g. reverse animation, movement etc).
	*/


} // KE ns