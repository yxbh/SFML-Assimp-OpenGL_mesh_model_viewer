#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"
#include "../TransformMatrix.hpp"

namespace object
{
	class Triangle
		: public KG::Mesh
	{

	private:
		// transformation matrix
		KG::TransformMatrix m_TransformationMatrix;

	public:
		Triangle(void);
		virtual ~Triangle(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime) final;

	};

} // object ns

