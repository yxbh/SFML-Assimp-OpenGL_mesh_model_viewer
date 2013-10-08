#pragma once

// Interface
#include "Interface/ITime.hpp"

// Implementations.
//#define _KE_USE__std_Time_ // probably a good idea to put this somewhere central for all component selection config's.
#define _KE_USE__SFML2_Time
// _KE_USE__std_Time_

#if defined(_KE_USE__std_Time_)
	#include "System\std_Time.hpp"
#elif defined(_KE_USE__SFML2_Time)
	#include "System\SFML2_Time.hpp"
#endif


