#pragma once
#include "../../common.hpp"

namespace KG
{

	/*! 
	
		Commonly used number, vector and matrix related stuff.
		Usually just long template names typedef into something cooler.
	
	*/

	typedef GLuint				Vec1u;
	typedef std::vector<GLuint> Vec1uVector;

	typedef glm::vec3			Vec3f;
	typedef std::vector<Vec3f>	Vec3fVector;
	typedef glm::vec4			Vec4f;
	typedef std::vector<Vec4f>	Vec4fVector;

} // KG ns
