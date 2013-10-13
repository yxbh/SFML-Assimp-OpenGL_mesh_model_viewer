#include "ObjectManipulator.hpp"
#include "../Engine/Entity/Entity.hpp"
#include "../Engine/Entity/RenderComponent.hpp"
#include "../Engine/Entity/EntityManager.hpp"
#include "../Engine/Event/EventManager.hpp"
#include "../Engine/Graphics/OpenGL_Graphics.hpp"
#include "GUI.hpp"

namespace CITS
{
namespace ObjectManipulator
{

	LogicComponent::LogicComponent(KE::Entity_SmartPtr p_spEntity)
		: KE::LogicComponent(p_spEntity)
		, m_wpCurrentSelected()
		, m_spControlPanelGUI(nullptr)
	{
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::NewObject
		); 
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::TranslateDeltaY
		); 
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::StrafeRightRelCam
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::StrafeForeRelCam
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::UniformScale
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::RollUp
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::RollRight
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::NewObjectRequest
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetMatEmissive
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetMatAmbient
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetMatDiffuse
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetMatSpecular
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::SetMatShininess
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::NewGUICreated
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::ToggleControlPanelVisibility
		);
		KE::Event::Get().AddListener
		(
			this->GetEntity()->GetID()
			, std::bind(&LogicComponent::EventDelegate, this, std::placeholders::_1)
			, CITS::EventType::ChangeTextureRequest
		);
	}

	LogicComponent::~LogicComponent(void)
	{
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::NewObject);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::TranslateDeltaY);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::StrafeRightRelCam);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::StrafeForeRelCam);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::UniformScale);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::RollUp);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::RollRight);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::NewObjectRequest);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetMatEmissive);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetMatAmbient);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetMatDiffuse);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetMatSpecular);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::SetMatShininess);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::NewGUICreated);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::ToggleControlPanelVisibility);
		KE::Event::Get().RemoveListener(this->GetEntity()->GetID(), CITS::EventType::ChangeTextureRequest);
	}

	void LogicComponent::Update(const KE::Duration p_Duration)
	{
		KE_UNREFERENCED_PARAMETER(p_Duration);
	}

	const bool LogicComponent::HandleManipulation(const KE::Event_SmartPtr p_spEvent)
	{
		KE::Entity_SmartPtr entity_ptr = m_wpCurrentSelected.lock();
		if (!entity_ptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "ObjectManipulator : Nothing is selected at the moment.");
			return false;
		}
		std::shared_ptr<CITS::Control_Event> man_event = std::static_pointer_cast<CITS::Control_Event>(p_spEvent);
		std::weak_ptr<KE::RenderComponent> comp_wkptr = entity_ptr->GetComponent<KE::RenderComponent>(1000);
		std::shared_ptr<KE::RenderComponent> comp_sptr = comp_wkptr.lock();
		if (!comp_sptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "ObjectManipulator : Entity has no KE::RenderComponent.");
			return false;
		}

		double delta = man_event->GetDelta();
		KG::SceneNode_SmartPtr node = comp_sptr->GetSceneNode();
		switch (p_spEvent->GetType())
		{
		case CITS::EventType::TranslateDeltaY:
			node->OffsetPosition(0.0, delta, 0.0);
			break;
		case CITS::EventType::StrafeRightRelCam:
			node->StrafeRelativeTo(delta, 0.0, 0.0, KG::Graphics::Get().GetScene().GetCamera().GetOrientationMat());
			break;
		case CITS::EventType::StrafeForeRelCam:
			{
				glm::dvec4 forward(KG::Graphics::Get().GetScene().GetCamera().GetForwardVec3(), 0.0);
				forward.y = 0.0; // remove rotation around y-axis.
				forward = glm::normalize(forward);
				glm::dmat4 orientation;
				orientation[0] = glm::dvec4(1.0, 0.0, 0.0, 0.0);//right;
				orientation[1] = glm::dvec4(0.0, 1.0, 0.0, 0.0);//up;
				orientation[2] = forward; //forward;
				orientation[3] = glm::dvec4(0.0, 0.0, 0.0, 0.0);
				node->StrafeRelativeTo(0.0, 0.0, delta, orientation);
				break;
			}
		case CITS::EventType::UniformScale:
			node->OffsetScale(delta, delta, delta);
			break;
		case CITS::EventType::RollUp:
			{
				const glm::dquat camera_quat = KG::Graphics::Get().GetScene().GetCamera().GetOrientationQuat();
				const glm::dquat offset = glm::angleAxis(delta, glm::dvec3(1.0, 0.0, 0.0));
				glm::dquat Delta =  camera_quat * offset * glm::conjugate(camera_quat);
				Delta = Delta * node->GetOrientationQuat();
				node->SetOrientationQuat(glm::normalize(Delta)); 
				break;
			}
		case CITS::EventType::RollRight:
			{
				const glm::dquat camera_quat = KG::Graphics::Get().GetScene().GetCamera().GetOrientationQuat();
				//KG::Graphics::Get().GetScene().GetCamera().SetPitch(0.0);
				glm::dvec3 vec(KG::Graphics::Get().GetScene().GetCamera().GetForwardVec3());
				KE::Debug::print(std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z));
				const glm::dquat offset = glm::angleAxis(delta, glm::dvec3(0.0, 1.0, 0.0));
				glm::dquat Delta =  camera_quat * offset * glm::conjugate(camera_quat);
				Delta = Delta * node->GetOrientationQuat();
				node->SetOrientationQuat(glm::normalize(Delta)); 
				break;
			}
		case CITS::EventType::NewGUICreated:
			{
				std::shared_ptr<CITS::NewGUICreated_Event> man_event = std::static_pointer_cast<CITS::NewGUICreated_Event>(p_spEvent);
				m_spControlPanelGUI = std::static_pointer_cast<CITS::ControlPanelGUI>(man_event->GetGUI());
			}
			break;
		case CITS::EventType::ChangeTextureRequest:
			{
				KG::Mesh_SmartPtr mesh_node = std::static_pointer_cast<KG::Mesh>(node);
				if (!mesh_node)
					return false;
				std::shared_ptr<CITS::NewObjectRequest_Event> eve
					= std::static_pointer_cast<CITS::NewObjectRequest_Event>(p_spEvent);
				if (!eve)
					return false;
				if (eve->GetTextureFileName() == "NONE")
					mesh_node->SetHasTexture(false);
				else
					mesh_node->SetTexture(KG::Texture_SmartPtr(new KG::Texture("models-textures/" + eve->GetTextureFileName())));
			}
			break;
		}
		return true;
	}

	const bool LogicComponent::HandleMaterialManipoulation(const KE::Event_SmartPtr p_spEvent)
	{
		KE::Entity_SmartPtr entity_ptr = m_wpCurrentSelected.lock();
		if (!entity_ptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "ObjectManipulator : Nothing is selected at the moment.");
			return false;
		}
		std::shared_ptr<CITS::SetVec3f_Event> man_event = std::static_pointer_cast<CITS::SetVec3f_Event>(p_spEvent);
		std::weak_ptr<KE::RenderComponent> comp_wkptr = entity_ptr->GetComponent<KE::RenderComponent>(1000);
		std::shared_ptr<KE::RenderComponent> comp_sptr = comp_wkptr.lock();
		if (!comp_sptr)
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "ObjectManipulator : Entity has no KE::RenderComponent.");
			return false;
		}

		glm::vec3 delta(man_event->GetValues());
		KG::SceneNode_SmartPtr node = comp_sptr->GetSceneNode();
		KG::Mesh_SmartPtr mesh_node = std::static_pointer_cast<KG::Mesh>(node);
		if (!mesh_node)
			return false;

		KG::Material mat = mesh_node->GetMaterial();
		switch (man_event->GetType())
		{
			case CITS::EventType::SetMatAmbient:
				mat.Ambient = man_event->GetValues();
				break;
			case CITS::EventType::SetMatEmissive:
				mat.Emissive = man_event->GetValues();
				break;
			case CITS::EventType::SetMatDiffuse:
				mat.Diffuse = man_event->GetValues();
				break;
			case CITS::EventType::SetMatSpecular:
				mat.Specular = man_event->GetValues();
				break;
			case CITS::EventType::SetMatShininess:
				mat.Shininess = man_event->GetValues().x;
				break;
		}
		mesh_node->SetMaterial(mat);
		return true;
	}

	const bool LogicComponent::HandleNewObjectRequest(const KE::Event_SmartPtr p_spEvent)
	{
		std::shared_ptr<CITS::NewObjectRequest_Event> eve
					= std::static_pointer_cast<CITS::NewObjectRequest_Event>(p_spEvent);
		if (!eve)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "ObjectManipulator : NewObjectRequest failure.");
			return false;
		}
		return this->SpawnNewObject(eve);
	}

	const bool LogicComponent::EventDelegate(KE::Event_SmartPtr p_spEvent)
	{
		switch (p_spEvent->GetType())
		{
		case CITS::EventType::NewObject:
			{
				std::shared_ptr<CITS::NewObject_Event> new_obj
					= std::static_pointer_cast<CITS::NewObject_Event>(p_spEvent);
				assert(new_obj);
				m_wpCurrentSelected = KE::GetEntityManager().GetEntityByID(new_obj->GetEntityID());
				break;
			}
		case CITS::EventType::UniformScale:
		case CITS::EventType::TranslateDeltaY:
		case CITS::EventType::StrafeRightRelCam:
		case CITS::EventType::StrafeForeRelCam:
		case CITS::EventType::RollUp:
		case CITS::EventType::RollRight:
		case CITS::EventType::NewGUICreated:
		case CITS::EventType::ChangeTextureRequest:
			this->HandleManipulation(p_spEvent);
			break;
		case CITS::EventType::NewObjectRequest:	
			this->HandleNewObjectRequest(p_spEvent);
			break;
		case CITS::EventType::SetMatAmbient:
		case CITS::EventType::SetMatEmissive:
		case CITS::EventType::SetMatDiffuse:
		case CITS::EventType::SetMatSpecular:
		case CITS::EventType::SetMatShininess:		
			this->HandleMaterialManipoulation(p_spEvent);
			break;
		case CITS::EventType::ToggleControlPanelVisibility:
			{
				if (m_spControlPanelGUI)
					m_spControlPanelGUI->ToggleGUIVisibility();
				else
					return false;
			}
			break;
		}
		return true;
	}
	
	const bool LogicComponent::SpawnNewObject(const std::shared_ptr<CITS::NewObjectRequest_Event> p_spEvent)
	{
		/* figure out mesh and texture name. */
		if (m_MeshName == "" && p_spEvent->GetMeshFileName() == "DEFAULT")
		{
			KE::Debug::print("ObjectManipulator : Loading default model = \"model55.x\"");
			m_MeshName = "model55.x";
			if (p_spEvent->GetTextureFileName() == "NONE")
				m_TextureName = "NONE";
		}
		else if (p_spEvent->GetMeshFileName() == "DEFAULT")
		{
			KE::Debug::print("ObjectManipulator : Loading last imported model.");
		}
		else
		{
			m_MeshName = p_spEvent->GetMeshFileName();
			m_TextureName = p_spEvent->GetTextureFileName();
		}
		
		/* use texture? */
		bool use_texture;
		use_texture = (m_TextureName == "NONE") ? false : true; // don't use texture if string empty.

		std::string model_tex_dir = "models-textures/";
		KG::MeshLoader model;
		model.Load(model_tex_dir+m_MeshName);
		auto mesh_list = model.GetMeshList();
		if (mesh_list.size() == 0)
		{
			KE::Debug::print("ObjectManipulator : nothing in mesh list.");
			return false;
		}

		KG::Mesh_SmartPtr mesh = mesh_list.at(0);
		assert(mesh);
		if(mesh->Loaded())
			mesh->BufferAll();

		mesh->SetRenderPass(KG::RenderPass::Dynamic);
		mesh->SetPosition(0.0,0.0,0.0);

		if (use_texture)
		{
			KG::Texture_SmartPtr texture(new KG::Texture(model_tex_dir+m_TextureName));
			mesh->SetTexture(texture);
		}

		// add entity component, and scenenode into their managers.
		KG::Graphics::Get().GetScene().AddSceneNode(mesh);
		KE::Entity_SmartPtr entity(new KE::Entity(mesh->GetEntityID()));
		KE::RenderComponent_SmartPtr r_comp(new KE::RenderComponent(entity));
		entity->AddComponent(r_comp);
		r_comp->SetSceneNode(mesh);
		KE::GetEntityManager().Add(entity);

		KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewObject_Event(entity->GetID())));
		return true;
	}

	KE::Entity_SmartPtr Make(void)
	{
		KE::Entity_SmartPtr entity(new KE::Entity);
		KE::EntityComponent_SmartPtr logic_com(new CITS::ObjectManipulator::LogicComponent(entity));
		entity->AddComponent(logic_com);
		return entity;
	}

}
}
