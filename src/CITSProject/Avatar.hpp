#pragma once
#include "../Engine/common.hpp"
#include "../Engine/Entity/RenderComponent.hpp"
#include "../Engine/Entity/InputComponent.hpp"
#include "../Engine/Entity/LogicComponent.hpp"
#include "../Engine/Event/IEvent.hpp"
#include "../Engine/Graphics/OpenGL_Graphics.hpp"


namespace CITS
{
namespace Avatar
{
	class InputComponent
		: public KE::InputComponent
	{
	private:

	public:
		InputComponent(KE::Entity_SmartPtr p_spEntity);
		virtual ~InputComponent(void);
		virtual void Init(void);
		virtual void Update(const KE::Duration p_Elapsed) final;

		bool MovementDelegate(KE::Event_SmartPtr p_spEvent);
	};

	class RenderComponent
		: public KG::OGLEntityRenderComponent
	{
	public:
		RenderComponent(KE::Entity_SmartPtr p_spEntity);
		virtual void Init(void);
		virtual void Update(const KE::Duration p_Elapsed) final;
	};


	class LogicComponent
		: public KE::LogicComponent
	{
	public:
		LogicComponent(KE::Entity_SmartPtr p_spEntity);
		virtual void Update(const KE::Duration p_Elapsed) final;
	};

}
}
