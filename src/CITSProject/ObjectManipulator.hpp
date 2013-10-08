#pragma once
#include "../Engine/common.hpp"
#include "CITSEvents.hpp"
#include "../Engine/Entity/InputComponent.hpp"
#include "../Engine/Entity/LogicComponent.hpp"

namespace CITS
{
	class ControlPanelGUI;

namespace ObjectManipulator
{

	class LogicComponent
		: public KE::LogicComponent
	{
	private:
		KE::Entity_WeakPtr							m_wpCurrentSelected;
		std::shared_ptr<CITS::ControlPanelGUI>		m_spControlPanelGUI;

		std::string		m_MeshName;
		std::string		m_TextureName;

	public:
		LogicComponent(KE::Entity_SmartPtr p_spEntity);
		virtual ~LogicComponent(void);

		virtual void Update(const KE::Duration p_Duration) override;

		const bool HandleManipulation(const KE::Event_SmartPtr p_spEvent);
		const bool HandleMaterialManipoulation(const KE::Event_SmartPtr p_spEvent);
		const bool HandleNewObjectRequest(const KE::Event_SmartPtr p_spEvent);
		const bool EventDelegate(KE::Event_SmartPtr p_spEvent);

		const bool SpawnNewObject(const std::shared_ptr<CITS::NewObjectRequest_Event> p_spEvent);
	};

	/*! Return an ObjectManipulator Entity. */
	extern KE::Entity_SmartPtr Make(void);
}
} // CITS ns
