#pragma once
#include "../../Engine/common.hpp"
#include "../../Engine/Graphics/OpenGL_Graphics.hpp"
#include "../TransformMatrix.hpp"

namespace object
{
	class Cube3D
		: public KG::Mesh
	{
	private:
		KG::TransformMatrix m_TransformationMatrix;

	public:
		Cube3D(void);
		virtual ~Cube3D(void);

		/*! Update animation frame etc. */
		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_DeltaTime) final;
	};

} // object ns