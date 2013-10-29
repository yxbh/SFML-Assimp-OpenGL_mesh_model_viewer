#include "GUI.hpp"
#include "../Engine/Event/EventManager.hpp"
#include "CITSEvents.hpp"
#include "../Engine/Entity/Entity.hpp"

namespace CITS
{
	ControlPanelGUI::ControlPanelGUI(KE::Entity_SmartPtr p_spEntity)
		: KG::OGLEntityRenderComponent(p_spEntity)
	{
		m_wndmain = sfg::Window::Create( sfg::Window::TITLEBAR | sfg::Window::BACKGROUND | sfg::Window::RESIZE );
		m_wndmain->SetTitle( L"Control Panel" );

		// Layout.
		sfg::Box::Ptr boxmodeselect( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
		boxmodeselect->SetSpacing( 5.f );

		sfg::Box::Ptr radio_box1( sfg::Box::Create( sfg::Box::VERTICAL ) );
		sfg::Box::Ptr radio_box2( sfg::Box::Create( sfg::Box::VERTICAL ) );

		FFCam   = sfg::RadioButton::Create( "Free Flight Camera" );
		TPCam   = sfg::RadioButton::Create( "Third Person Camera", FFCam -> GetGroup() );
		ObjMan  = sfg::RadioButton::Create( "Object Manipulation", TPCam -> GetGroup() );
		MatEdit = sfg::RadioButton::Create( "Materials", ObjMan -> GetGroup() );

		radio_box1 -> Pack( FFCam ); FFCam->SetActive(true);
		radio_box1 -> Pack( TPCam );
		radio_box2 -> Pack( ObjMan );
		radio_box2 -> Pack( MatEdit );
	
		FFCam   -> GetSignal( sfg::Widget::OnLeftClick ).Connect( &ControlPanelGUI::OnFFCamClicked, this );
		TPCam	-> GetSignal( sfg::Widget::OnLeftClick ).Connect( &ControlPanelGUI::OnTPCamClicked, this );
		ObjMan  -> GetSignal( sfg::Widget::OnLeftClick ).Connect( &ControlPanelGUI::OnObjManClicked, this );
		MatEdit -> GetSignal( sfg::Widget::OnLeftClick ).Connect( &ControlPanelGUI::OnMatEditClicked, this );

		boxmodeselect -> Pack( radio_box1, false );
		boxmodeselect -> Pack( radio_box2, false );

		sfg::Frame::Ptr frame1( sfg::Frame::Create( L"Select Mode" ) );
		frame1->Add( boxmodeselect );

		sfg::Box::Ptr boxloadobj( sfg::Box::Create( sfg::Box::VERTICAL ) );
		boxloadobj->SetSpacing( 5.f );
		/////////////////////////////////////////////////////////////////////////////////////
		sfg::Separator::Ptr separatorv( sfg::Separator::Create( sfg::Separator::VERTICAL ) );

		m_Combo_Model = sfg::ComboBox::Create();
		std::string line;
		std::ifstream m_ModelFileList;
		m_ModelFileList.open("ModelList.txt");
		if( m_ModelFileList.is_open() )
		{
			while( std::getline(m_ModelFileList, line) )
				m_Combo_Model -> AppendItem( line );
		}
		m_ModelFileList.close();

		boxloadobj -> Pack( m_Combo_Model, true );

		m_Combo_Texture = sfg::ComboBox::Create();
		std::ifstream m_TextureFileList;
		m_TextureFileList.open( "TextureList.txt" );
		if( m_TextureFileList.is_open() )
		{
			while(std::getline( m_TextureFileList, line) )
				m_Combo_Texture -> AppendItem( line );
		}
		m_Combo_Texture->SelectItem(0);
		m_TextureFileList.close();

		boxloadobj -> Pack( m_Combo_Texture, true );

		// load object button.
		m_btnLoadObject = sfg::Button::Create( L"Load Object") ;
		m_btnLoadObject -> GetSignal(sfg::Button::OnLeftClick).Connect( &ControlPanelGUI::OnLoadObjectClicked, this);
		boxloadobj -> Pack( m_btnLoadObject, false );

		// change texture button
		m_btnChangeTexture = sfg::Button::Create( L"Change Texture") ;
		m_btnChangeTexture -> GetSignal(sfg::Button::OnLeftClick).Connect( &ControlPanelGUI::OnChangeTextureClicked, this);
		boxloadobj -> Pack( m_btnChangeTexture, false );

		m_LoadObjectFrame = sfg::Frame::Create( L"Load Objects" );
		m_LoadObjectFrame -> Add( boxloadobj );
		m_LoadObjectFrame -> SetAlignment( sf::Vector2f( .8f, .0f ) );



		//////////////////////////////////////////////////////////////////////////////////////
		sfg::Separator::Ptr separatorh( sfg::Separator::Create( sfg::Separator::HORIZONTAL ) );

		sfg::Box::Ptr box_TextureEdit( sfg::Box::Create( sfg::Box::VERTICAL ) );
		sfg::Box::Ptr box_TextureEdit1( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
		sfg::Box::Ptr box_TextureEdit2( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	    
		sfg::Box::Ptr boxmain( sfg::Box::Create( sfg::Box::VERTICAL ) );
		boxmain->SetSpacing( 5.f );

		m_EditMaterialsFrame = sfg::Frame::Create( L"Edit Materials" );
		m_EditMaterialsFrame -> Add( box_TextureEdit );
		m_EditMaterialsFrame -> SetAlignment( sf::Vector2f( .8f, .0f ) );

		sfg::Frame::Ptr DiffuseFrame = sfg::Frame::Create(L"Diffuse");
		sfg::Box::Ptr diffbox = sfg::Box::Create(sfg::Box::VERTICAL);

		m_DiffuseScale1 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_DiffuseScale2 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_DiffuseScale3 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);

		m_dAdjustment1 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_dAdjustment1 -> GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_DiffuseScale1 -> SetAdjustment(m_dAdjustment1);
		m_DiffuseScale1 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		m_dAdjustment2 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_dAdjustment2 -> GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );		
		m_DiffuseScale2 -> SetAdjustment(m_dAdjustment2);
		m_DiffuseScale2 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		m_dAdjustment3 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_dAdjustment3 -> GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );	
		m_DiffuseScale3 -> SetAdjustment(m_dAdjustment3);
		m_DiffuseScale3 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		diffbox -> Pack(m_DiffuseScale1,true);
		diffbox -> Pack(m_DiffuseScale2,true);
		diffbox -> Pack(m_DiffuseScale3,true);

		DiffuseFrame -> Add(diffbox);

		sfg::Frame::Ptr SpecularFrame = sfg::Frame::Create(L"Specular");
		sfg::Box::Ptr specbox1 = sfg::Box::Create(sfg::Box::VERTICAL);
		m_SpecularScale1 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_SpecularScale2 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_SpecularScale3 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);

		m_spAdjustment1 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_spAdjustment1->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_SpecularScale1 -> SetAdjustment(m_spAdjustment1);
		m_SpecularScale1 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		m_spAdjustment2 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_spAdjustment2->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_SpecularScale2 -> SetAdjustment(m_spAdjustment2);
		m_SpecularScale2 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		m_spAdjustment3 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_spAdjustment3->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_SpecularScale3 -> SetAdjustment(m_spAdjustment3);
		m_SpecularScale3 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		specbox1 -> Pack(m_SpecularScale1,true);
		specbox1 -> Pack(m_SpecularScale2,true);
		specbox1 -> Pack(m_SpecularScale3,true);

		SpecularFrame -> Add(specbox1);

		sfg::Frame::Ptr AmbientFrame = sfg::Frame::Create(L"Ambient");
		sfg::Box::Ptr ambbox = sfg::Box::Create(sfg::Box::VERTICAL);

		m_AmbientScale1 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_AmbientScale2 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_AmbientScale3 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);

		m_ambAdjustment1 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_ambAdjustment1->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_AmbientScale1 -> SetAdjustment(m_ambAdjustment1);
		m_AmbientScale1 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		m_ambAdjustment2 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_ambAdjustment2->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_AmbientScale2 -> SetAdjustment(m_ambAdjustment2);
		m_AmbientScale2 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		m_ambAdjustment3 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_ambAdjustment3->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );
		m_AmbientScale3 -> SetAdjustment(m_ambAdjustment3);
		m_AmbientScale3 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		ambbox -> Pack(m_AmbientScale1,true);
		ambbox -> Pack(m_AmbientScale2,true);
		ambbox -> Pack(m_AmbientScale3,true);

		AmbientFrame -> Add(ambbox);

		sfg::Frame::Ptr EmissiveFrame = sfg::Frame::Create(L"Emissive");
		sfg::Box::Ptr embox = sfg::Box::Create(sfg::Box::VERTICAL);

		m_EmissiveScale1 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_EmissiveScale2 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_EmissiveScale3 = sfg::Scale::Create(sfg::Scale::HORIZONTAL);

		m_emAdjustment1 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_emAdjustment1->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );

		m_EmissiveScale1 -> SetAdjustment(m_emAdjustment1);
		m_EmissiveScale1 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		m_emAdjustment2 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_emAdjustment2->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );

		m_EmissiveScale2 -> SetAdjustment(m_emAdjustment2);
		m_EmissiveScale2 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		m_emAdjustment3 = sfg::Adjustment::Create( 0.0f, 0.0f, 1.0f, 0.01f, 0.1f, 0.f);
		m_emAdjustment3->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );

		m_EmissiveScale3 -> SetAdjustment(m_emAdjustment3);
		m_EmissiveScale3 -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	
		embox -> Pack(m_EmissiveScale1,true);
		embox -> Pack(m_EmissiveScale2,true);
		embox -> Pack(m_EmissiveScale3,true);

		EmissiveFrame -> Add(embox);

		sfg::Frame::Ptr ShineFrame = sfg::Frame::Create(L"Shine");
		sfg::Box::Ptr shbox = sfg::Box::Create(sfg::Box::VERTICAL);

		m_ShineScale = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_shAdjustment1 = sfg::Adjustment::Create( 1.f, 0.f, 200.0f, 1.0f, 5.0f, 0.f);
		m_shAdjustment1->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::UpdateMaterials, this );

		m_ShineScale -> SetAdjustment(m_shAdjustment1);
		m_ShineScale -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		shbox -> Pack(m_ShineScale, false, false);

		ShineFrame -> Add(shbox);

		sfg::Frame::Ptr TScaleFrame = sfg::Frame::Create(L"Scale Texture");
		sfg::Box::Ptr scalebox = sfg::Box::Create(sfg::Box::VERTICAL);
		
		m_TextureScale = sfg::Scale::Create(sfg::Scale::HORIZONTAL);
		m_TextureScaleAdj = sfg::Adjustment::Create( 1.0f, 0.01f, 1.0f, 0.05f, 0.1f, 0.0f);
		m_TextureScaleAdj->GetSignal( sfg::Adjustment::OnChange ).Connect( &ControlPanelGUI::TextureMultiplier, this );

		m_TextureScale -> SetAdjustment(m_TextureScaleAdj);
		m_TextureScale -> SetRequisition( sf::Vector2f( 80.f, 20.f ) );

		scalebox -> Pack(m_TextureScale);

		TScaleFrame -> Add(scalebox);

		box_TextureEdit1 -> Pack(DiffuseFrame);
		box_TextureEdit1 -> Pack(SpecularFrame);
		box_TextureEdit2 -> Pack(AmbientFrame);
		box_TextureEdit2 -> Pack(EmissiveFrame);
		box_TextureEdit -> Pack(box_TextureEdit1);
		box_TextureEdit -> Pack(box_TextureEdit2);
		box_TextureEdit -> Pack(ShineFrame);
		box_TextureEdit -> Pack(TScaleFrame);
	
		m_EditMaterialsFrame -> Add(box_TextureEdit);

		boxmain->Pack( frame1, false );
		boxmain->Pack( m_LoadObjectFrame, false );
		boxmain->Pack( m_EditMaterialsFrame, false );
		boxmain->Pack( separatorh, false );

		m_LoadObjectFrame -> Show(false);
		m_EditMaterialsFrame -> Show(false);

		/* Instruction text. */
		instruction_label = sfg::Label::Create();
		instruction_label->SetText("# Controls:\nWASD - Movement.\nLeft Shift/Left Ctrl - Elevation/Depression.\nK/M - Increase Target Distance/Decrease Target Distance.\nF1/F2/F3 - FreeFlight/Object/Material.\nZ - Spawn last object.\nTab - toggle Control Panel visibility.");
		boxmain -> Pack(instruction_label);

		sfg::Button::Ptr btnhidewindow( sfg::Button::Create( L"Hide Control Panel" ) );
		btnhidewindow->SetId( "close" );
		boxmain->Pack( btnhidewindow, false );

		// Signals.
		btnhidewindow->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ControlPanelGUI::OnHideWindowClicked, this );

		m_wndmain->Add(boxmain);
		m_wndmain->SetRequisition(sf::Vector2f(475.f,200.f));
	}

	ControlPanelGUI::~ControlPanelGUI(void)
	{}

	void ControlPanelGUI::OnFFCamClicked()
	{
		m_LoadObjectFrame -> Show(false);
		m_EditMaterialsFrame -> Show(false);
		instruction_label -> Show(true);
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetFreeFlightMode_Event));
	}

	void ControlPanelGUI::OnTPCamClicked()
	{
		m_LoadObjectFrame -> Show(false);
		m_EditMaterialsFrame -> Show(false);
		instruction_label -> Show(true);
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetThirdPersonMode_Event));
	}

	void ControlPanelGUI::OnObjManClicked()
	{
		m_LoadObjectFrame->Show(true);
		m_EditMaterialsFrame->Show(false);
		instruction_label -> Show(true);
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetObjectManMode_Event));
	}

	void ControlPanelGUI::OnMatEditClicked()
	{
		m_LoadObjectFrame->Show(false);
		m_EditMaterialsFrame->Show(true);
		instruction_label -> Show(false);
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetObjectMatMode_Event));
	}

	void ControlPanelGUI::OnHideWindowClicked() 
	{
		//(m_wndmain->IsLocallyVisible());
		 m_wndmain->Show(false);
	}

	void ControlPanelGUI::OpenMenu()
	{
		 m_wndmain->Show(true);
	}

	void ControlPanelGUI::ToggleGUIVisibility(void)
	{
		m_wndmain->Show(!m_wndmain->IsLocallyVisible());
	}

	void ControlPanelGUI::UpdateMaterials()
	{
		glm::fvec3 diffuse(m_dAdjustment1->GetValue(), m_dAdjustment2->GetValue(), m_dAdjustment3->GetValue());
		glm::fvec3 specular(m_spAdjustment1->GetValue(), m_spAdjustment2->GetValue(), m_spAdjustment3->GetValue());
		glm::fvec3 ambient(m_ambAdjustment1->GetValue(), m_ambAdjustment2->GetValue(), m_ambAdjustment3->GetValue());
		glm::fvec3 emissive(m_emAdjustment1->GetValue(), m_emAdjustment2->GetValue(), m_emAdjustment3->GetValue());
		float shine = m_shAdjustment1->GetValue();

		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetMatDiffuse_Event(diffuse.x, diffuse.y, diffuse.z)));
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetMatSpecular_Event(specular.x, specular.y, specular.z)));
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetMatAmbient_Event(ambient.x, ambient.y, ambient.z)));
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetMatEmissive_Event(emissive.x, emissive.y, emissive.z)));
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetMatShininess_Event(shine)));
	}

	void ControlPanelGUI::OnLoadObjectClicked()
	{
		std::string mesh_file(m_Combo_Model->GetSelectedText());
		std::string texture_file(m_Combo_Texture->GetSelectedText());
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewObjectRequest_Event(mesh_file, texture_file)));
	}

	void ControlPanelGUI::OnChangeTextureClicked(void)
	{
		std::string texture_file(m_Combo_Texture->GetSelectedText());
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::ChangeTextureRequest_Event("", texture_file)));
	}

	void ControlPanelGUI::TextureMultiplier()
	{
		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::SetTexCoordMutiplier_Event(m_TextureScaleAdj->GetValue())));
	}

	void ControlPanelGUI::MeshStrafe(void)
	{

	}

}