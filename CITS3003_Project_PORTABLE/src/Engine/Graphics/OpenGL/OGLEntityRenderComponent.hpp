#pragma once
#include "../../Entity/RenderComponent.hpp"

namespace KG
{

	class SceneNode;
	typedef std::shared_ptr<KG::SceneNode> SceneNode_SmartPtr;

	class OGLEntityRenderComponent
		: public KE::RenderComponent
	{
	public:
		const static KE::EntityComponentID sk_ComponentID = 1000;
	private:
	
	public:
		OGLEntityRenderComponent(KE::Entity_SmartPtr p_spEntity);
		virtual ~OGLEntityRenderComponent(void) {};

		const KE::EntityComponentID GetID(void) const override;
	};

} // KG ns
