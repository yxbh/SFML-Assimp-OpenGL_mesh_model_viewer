#pragma once
#include "../common.hpp"

namespace KE
{
	typedef unsigned long long EntityID;

	const EntityID Invalid_EntityID = 0;

	/*! \class EntityIDGenerator
		A static class for generating unique EntityIDs.
	*/
	class EntityIDGenerator
	{
	private:
		static std::vector<KE::EntityID>	s_ListOfCurrentIDs;
		static EntityID						s_CurrentID;
	
	public:
		/*! return a new unsued EntityID. */
		static const EntityID NewID(void);
		/*! check if an EntityID is currently being used. */
		static bool Used(const EntityID & p_rID);
		/*! remove an EntityID from the used pool. */
		static bool Remove(const EntityID & p_rID);
		/*! return the highest possible value for EntityID. wrapper returning ULLONG_MAX. */
		static EntityID MaxIDValue(void);
	};

}
