#include "Entity.hpp"

namespace KE
{
	EntityID EntityIDGenerator::s_CurrentID = 0;
	std::vector<KE::EntityID> EntityIDGenerator::s_ListOfCurrentIDs(0, 0);

	const EntityID EntityIDGenerator::NewID(void)
	{
		while(Used(s_CurrentID))
			++s_CurrentID;
		s_ListOfCurrentIDs.push_back(s_CurrentID);
		return s_CurrentID;
	}

	bool EntityIDGenerator::Used(const EntityID & p_rID)
	{
		if (p_rID == Invalid_EntityID)
			return true;
		auto id = std::find(s_ListOfCurrentIDs.begin(), s_ListOfCurrentIDs.end(), p_rID);
		if (id == s_ListOfCurrentIDs.end())
			return false;
		return *id == p_rID;
	}

	bool EntityIDGenerator::Remove(const EntityID & p_rID)
	{
		if (p_rID == Invalid_EntityID)
			return false;
		auto id = std::find(s_ListOfCurrentIDs.begin(), s_ListOfCurrentIDs.end(), p_rID);
		if (id == s_ListOfCurrentIDs.end())
			return false;
		s_ListOfCurrentIDs.erase(id);
		return true;
	}

	EntityID EntityIDGenerator::MaxIDValue(void)
	{
		return ULLONG_MAX;
	}

} // KE ns
