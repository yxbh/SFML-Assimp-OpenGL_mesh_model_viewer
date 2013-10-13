#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"

namespace object
{
	class Triangle
		: public KG::Mesh
	{

	private:
	public:
		Triangle(void);
		virtual ~Triangle(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime) final;

	};

} // object ns

