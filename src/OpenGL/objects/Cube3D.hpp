#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"

namespace object
{
	class Cube3D
		: public KG::Mesh
	{
	private:
	public:
		Cube3D(void);
		virtual ~Cube3D(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_DeltaTime) final;
	};

} // object ns