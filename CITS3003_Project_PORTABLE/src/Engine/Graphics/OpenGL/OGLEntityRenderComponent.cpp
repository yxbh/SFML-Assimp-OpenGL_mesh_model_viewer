#include "OGLEntityRenderComponent.hpp"
#include "SceneNode.hpp"

namespace KG
{
	OGLEntityRenderComponent::OGLEntityRenderComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::RenderComponent(p_spEntity)
	{}

	const KE::EntityComponentID OGLEntityRenderComponent::GetID(void) const
	{
		return sk_ComponentID;
	}

} // KG ns
