#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"

namespace object
{
	class Rectangle2D
		: public KG::Mesh
	{
	private:
		glm::dmat4	m_TransformMatrix;

	public:
		Rectangle2D(void);
		virtual ~Rectangle2D(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_ElapsedTime) final;

	};

} // object ns
