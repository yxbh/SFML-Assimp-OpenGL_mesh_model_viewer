#pragma once
#include "../Engine/common.hpp"
#include "../Engine/Graphics/OpenGL/OGLEntityRenderComponent.hpp"

namespace KE
{
	class Entity;
	typedef std::shared_ptr<KE::Entity> Entity_SmartPtr;
}

namespace CITS
{

	class ControlPanelGUI
		: public KG::OGLEntityRenderComponent
	{
	public:
		sfg::Window::Ptr m_wndmain;

	private:
		sfg::Scale::Ptr m_DiffuseScale1, m_DiffuseScale2, m_DiffuseScale3;
		sfg::Adjustment::Ptr m_dAdjustment1, m_dAdjustment2, m_dAdjustment3;

		sfg::Scale::Ptr m_SpecularScale1, m_SpecularScale2, m_SpecularScale3;
		sfg::Adjustment::Ptr m_spAdjustment1, m_spAdjustment2, m_spAdjustment3;

		sfg::Scale::Ptr m_AmbientScale1, m_AmbientScale2, m_AmbientScale3;
		sfg::Adjustment::Ptr m_ambAdjustment1, m_ambAdjustment2, m_ambAdjustment3;
		
		sfg::Scale::Ptr m_EmissiveScale1, m_EmissiveScale2, m_EmissiveScale3;
		sfg::Adjustment::Ptr m_emAdjustment1, m_emAdjustment2, m_emAdjustment3 ;
		
		sfg::Scale::Ptr m_ShineScale;
		sfg::Adjustment::Ptr m_shAdjustment1;

		sfg::Button::Ptr m_btnLoadObject;
		sfg::Button::Ptr m_btnChangeTexture;

		sfg::RadioButton::Ptr MatEdit;
		sfg::RadioButton::Ptr ObjMan;
		sfg::RadioButton::Ptr FFCam;
		sfg::RadioButton::Ptr TPCam;

		sfg::ComboBox::Ptr m_Combo_Model;
		sfg::ComboBox::Ptr m_Combo_Texture;

		sfg::Frame::Ptr m_LoadObjectFrame;
		sfg::Frame::Ptr m_EditMaterialsFrame;

		sfg::Label::Ptr instruction_label;

	public:
		ControlPanelGUI(KE::Entity_SmartPtr p_spEntity);
		virtual ~ControlPanelGUI(void);

		void ToggleGUIVisibility(void);

	private:
		void OnHideWindowClicked(void);
		void OnLoadObjectClicked(void);
		void OnChangeTextureClicked(void);

		void OnFFCamClicked(void);
		void OnTPCamClicked(void);
		void OnObjManClicked(void);
		void OnMatEditClicked(void);

		void UpdateMaterials(void);
		
		void OpenMenu(void);
	};
}