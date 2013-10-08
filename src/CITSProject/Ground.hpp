#pragma once
#include "../Engine/common.hpp"
#include "../Engine/Entity/RenderComponent.hpp"
#include "../Engine/Entity/InputComponent.hpp"
#include "../Engine/Entity/LogicComponent.hpp"
#include "../Engine/Graphics/OpenGL_Graphics.hpp"

namespace CITS
{

namespace Ground
{

	class Mesh
		: public KG::Mesh
	{
	public:
		Mesh(void);
		virtual ~Mesh(void);

		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed);
	}; // Ground

} // Ground ns

} // CITS ns
