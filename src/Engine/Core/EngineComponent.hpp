#ifndef _KE_ENGINECOMPONENT_
#define _KE_ENGINECOMPONENT_
#include <cstdint>

namespace KE
{
	/** \class EngineComponent
		An interface for common functionalies & properties shared by all Engine components.
	*/
	class EngineComponent
	{
	protected:
		uint64_t m_ElapsedDurationThreshold; // AKA minial time threshold for update.
		uint64_t m_CummulatedElapsedDuration;

	public:
		/*! Ctor. */
		explicit EngineComponent(uint64_t p_ElapsedDurationThreshold)
			: m_ElapsedDurationThreshold(p_ElapsedDurationThreshold) {}
		~EngineComponent(void) {}

		/*! */
		void SetElapsedThreshold(const uint64_t p_Duration)
		{
			m_ElapsedDurationThreshold = p_Duration;
		}

	}; // EngineComponent class

} // KE ns


#endif