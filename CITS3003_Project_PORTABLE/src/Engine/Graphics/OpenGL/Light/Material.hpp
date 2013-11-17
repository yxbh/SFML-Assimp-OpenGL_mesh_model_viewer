#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"

namespace KG
{
	/*! \class Material

		Everything's between 0 and 1 except Shininess which goes beyond 1.
	*/
	struct Material
	{
	public:
		// name of material
		std::string	Name;
		// diffuse
		glm::vec3	Diffuse;
		// specular
		glm::vec3	Specular;
		// ambient
		glm::vec3	Ambient;
		// emission
		glm::vec3	Emissive;
		// strength / shinness?
		float		Shininess;

	public:
		Material(void);

	}; // TEMP

} // KG ns
