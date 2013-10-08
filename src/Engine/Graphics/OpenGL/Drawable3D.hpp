#pragma once
#include "../../common.hpp"
#include "RenderState.hpp"

namespace KG
{
	/*! \class Drawable3D
	*/
	class Drawable3D
	{
	private:
	public:
		Drawable3D(void) {};
		virtual ~Drawable3D(void) {};

		virtual void Draw(KG::RenderStates p_RenderStates) { KE_UNREFERENCED_PARAMETER(p_RenderStates);};
	};

	typedef std::shared_ptr<KG::Drawable3D> Drawable3D_SmartPtr;

} // KE ns
