#include "Material.hpp"

namespace KG
{

	Material::Material(void)
		: Name("N/A")
		, Emissive(0.0f, 0.0f, 0.0f)
		, Ambient(0.1f, 0.1f, 0.1f)
		, Diffuse(0.8f, 0.8f, 0.8f)
		, Specular(1.0f, 1.0f, 1.0f)
		, Shininess(20.f)
	{}

} // KG ns
