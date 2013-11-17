#pragma once
#include "../common.hpp"

namespace KE
{
	typedef unsigned long EntityComponentID;

	/*! \class EntityComponentID
		An ID that is statically unique.
		Each EntityComponent type should have an unique ID, declared at compile time.
	*/
	const EntityComponentID Invalid_EntityComponentID = 0;

}
