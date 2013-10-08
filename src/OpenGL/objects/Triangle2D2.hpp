#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"
#include "../TransformMatrix.hpp"

namespace object
{
	class Triangle2D
		: public KG::Mesh
	{

	private:
		// transformation matrix
		KG::TransformMatrix m_TransformationMatrix;

	public:
		Triangle2D(void);
		virtual ~Triangle2D(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime) final;

	};

} // object ns

