#pragma once
#include "../../common.hpp"

namespace KG
{
	class ShaderProgram;

	struct RenderStates
	{
		glm::mat4			transform;
		KG::ShaderProgram*	shader_program;

		RenderStates(void) : transform(1.0f), shader_program(nullptr) {}
	};

} // KG
