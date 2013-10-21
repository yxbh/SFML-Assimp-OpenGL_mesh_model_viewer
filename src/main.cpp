#include "Engine/Engine.hpp"
#include "OpenGL/objects.hpp"
#include "Engine/Graphics/OpenGL/Scene.hpp"
#include "Engine/Graphics/OpenGL/OGLEntityRenderComponent.hpp"
#include "Engine/Entity/Entity.hpp"
#include "Engine/Graphics/OpenGL_Graphics.hpp"

#include "CITSProject/InputController.hpp"
#include "CITSProject/Avatar.hpp"
#include "CITSProject/Ground.hpp"
#include "CITSProject/ObjectManipulator.hpp"
#include "CITSProject/GUI.hpp"
#include "CITSProject/CITSEvents.hpp"

static void AddControlPanelGUI(KE::Engine & p_rEngine)
{
	std::shared_ptr<KE::Entity> entity(new KE::Entity());
	std::shared_ptr<CITS::ControlPanelGUI> control_panel(new CITS::ControlPanelGUI(entity));
	entity->AddComponent(control_panel);
	p_rEngine.GetRenderSystem().GetGUISystem().GetSFGDesktop().Add(control_panel->m_wndmain);
	KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewGUICreated_Event(control_panel)));
}

static void AddManipulator(KE::Engine & p_rEngine)
{
	KE::Entity_SmartPtr man = CITS::ObjectManipulator::Make();
	p_rEngine.GetEntityManager().Add(man);

	KE::Debug::print("Added ObjectManipulator.");
}

static void AddLight(KE::Engine & p_rEngine)
{
	KG::Light_SmartPtr global(new KG::Light());
	global->SetType(KG::Light::Type::Directional)
		.SetAmbient(glm::dvec3(0.1, 0.1, 0.1))
		.SetColor(glm::dvec3(0.6, 0.6, 0.6));
	KG::Graphics::Get().GetScene().AddSceneNode(global);
	KG::Graphics::Get().GetLightManager().AddLight(global);

	KG::Light_SmartPtr spot(new KG::Light());
	spot->SetType(KG::Light::Type::SpotLight)
		.SetAmbient(glm::dvec3(0.1, 0.1, 0.5))
		.SetAttenuationQuadratic(0.01f)
		.SetOuterConeAngle(45).SetInnerConeAngle(15)
		.SetColor(glm::dvec3(1.0, 0.0, 0.5))
		.OffsetPitch(-35.0);
	spot->SetPosition(3.0, 7.0, -4.0);
	KG::Graphics::Get().GetScene().AddSceneNode(spot);
	KG::Graphics::Get().GetLightManager().AddLight(spot);
	KE::Debug::print("Added lights.");
}

static void AddController(KE::Engine & p_rEngine)
{
	std::shared_ptr<KE::Entity> entity(new KE::Entity());

	std::shared_ptr<CITS::Input::InputController> controller(new CITS::Input::InputController(entity));
	entity->AddComponent(controller);
	p_rEngine.GetEntityManager().Add(entity);

	KE::Debug::print("Added controller.");
}

static void AddAvatar(KE::Engine & p_rEngine)
{
	std::shared_ptr<KE::Entity> entity(new KE::Entity());

	std::shared_ptr<CITS::Avatar::LogicComponent> logic(new CITS::Avatar::LogicComponent(entity));
	entity->AddComponent(logic);

	std::shared_ptr<CITS::Avatar::InputComponent> input(new CITS::Avatar::InputComponent(entity));
	entity->AddComponent(input);

	std::shared_ptr<CITS::Avatar::RenderComponent> render(new CITS::Avatar::RenderComponent(entity));
	entity->AddComponent(render);

	KG::Camera_SmartPtr camera(new KG::Camera());
	render->SetSceneNode(camera);
	
	p_rEngine.GetEntityManager().Add(entity);
	p_rEngine.GetRenderSystem().GetScene().SetCamera(camera);

	KE::Debug::print("Added avatar.");
}

static void AddGround(KE::Engine & p_rEngine)
{
	/*std::shared_ptr<CITS::Ground::Mesh> ground(new CITS::Ground::Mesh());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(ground);*/

	const char * path = "models-textures/model0.x";
	KG::MeshLoader loader;
	auto mesh_list = loader.Load(path)->GetList();
	if (mesh_list.size() == 0)
	{
		KE::Debug::print("nothing in mesh list.");
		return;
	}
	
	KG::Mesh_SmartPtr mesh = mesh_list.at(0);
	assert(mesh);
	if(mesh->Loaded())
		mesh->BufferAll();

	mesh->SetRenderPass(KG::RenderPass::Static);
	mesh->OffsetPosition(0.0, -2.0, 0.0).OffsetOrientation(90.0, 0.0, 0.0).SetScale(50.0, 50.0, 50.0);
	mesh->SetName("Model");

	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(mesh);/**/

	KE::Entity_SmartPtr entity(new KE::Entity(mesh->GetEntityID()));
	KE::RenderComponent_SmartPtr r_comp(new KE::RenderComponent(entity));
	entity->AddComponent(r_comp);
	r_comp->SetSceneNode(mesh);
	p_rEngine.GetEntityManager().Add(entity);

	KE::Debug::print("Added ground.");
}

static void AddTriangle(KE::Engine & p_rEngine)
{
	std::shared_ptr<object::Triangle> triangle(new object::Triangle());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(triangle);
	KE::Debug::print("Added triangle.");
}

static void AddTriangle2(KE::Engine & p_rEngine)
{
	std::shared_ptr<object::Triangle2D> triangle(new object::Triangle2D());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(triangle);
	KE::Debug::print("Added triangle.");
}

static void AddRectiangle2D(KE::Engine & p_rEngine)
{
	std::shared_ptr<object::Rectangle2D> rectangle(new object::Rectangle2D());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(rectangle);
	KE::Debug::print("Added 2D rectangle.");
}

static void AddCube(KE::Engine & p_rEngine)
{
	std::shared_ptr<object::Cube3D> cube3d(new object::Cube3D());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(cube3d);
	KE::Debug::print("Added 3D cube.");
}

static void TestAsset(KE::Engine & p_rEngine)
{
	std::string model_tex_dir = "models-textures/";
	std::string mesh_file = "";
	std::ifstream fin("CurrentMesh.txt");
	if (fin) fin >> mesh_file;
	KG::MeshLoader loader;
	KG::Meshes_SmartPtr meshes = loader.Load(model_tex_dir+mesh_file);
	KG::Mesh_SP_List mesh_list;
	if (meshes)
	{
		mesh_list = meshes->GetList();
	}
	else
	{
		KE::Debug::print("nothing in mesh list.");
		return;
	}
	
	KG::Mesh_SmartPtr mesh = mesh_list.at(0);
	assert(mesh);
	if(mesh->Loaded())
		mesh->BufferAll();
	KE::Debug::check_for_GL_error();

	KG::ShaderResourceList shader_resource_list;
	shader_resource_list.push_back(KG::ShaderResource(GL_VERTEX_SHADER, "shaders/generic.V.shader"));
	shader_resource_list.push_back(KG::ShaderResource(GL_FRAGMENT_SHADER, "shaders/generic.F.shader"));
	KG::ShaderProgram_SmartPtr shader_program(new KG::ShaderProgram());
	shader_program->SetGLHandle(KG::ShaderProgramFactory::MakeFromFiles(shader_resource_list));
	mesh->SetShaderProgram(shader_program);

	mesh->SetRenderPass(KG::RenderPass::Static);
	//mesh->SetModelMatrix(glm::scale(glm::dvec3(0.05, 0.05, 0.05)));
	mesh->SetPosition(6.0,2.0,0.0);
	mesh->SetName("Model");

	// texture
	std::string texture_path = "";
	std::ifstream texture_fin("CurrentTexture.txt");
	if (texture_fin)
		texture_fin >> texture_path;
	KG::Texture_SmartPtr texture(new KG::Texture(KG::Texture::DType::Tex2D, model_tex_dir+texture_path));
	//mesh->SetTexture(texture);
	mesh->SetMaterial(KG::Material());
	p_rEngine.GetRenderSystem().GetScene().AddSceneNode(mesh);/**/

	KE::Entity_SmartPtr entity(new KE::Entity(mesh->GetEntityID()));
	KE::RenderComponent_SmartPtr r_comp(new KE::RenderComponent(entity));
	entity->AddComponent(r_comp);
	r_comp->SetSceneNode(mesh);
	p_rEngine.GetEntityManager().Add(entity);

	KE::Event::Get().QueueEvent(KE::Event_SmartPtr(new CITS::NewObject_Event(entity->GetID())));
	
	KE::Debug::print("Added model.");
}


int main()
{
	KE::Debugger::print("Hello World.");

	{
		KE::Engine kengine;
		kengine.Init();
		
		TestAsset(kengine);

		AddCube(kengine);
		AddTriangle(kengine);
		AddTriangle2(kengine);
		AddRectiangle2D(kengine);
		
		AddGround(kengine);
		AddController(kengine);
		AddAvatar(kengine);
		AddManipulator(kengine);
		AddControlPanelGUI(kengine);
		AddLight(kengine);

		kengine.Run(1);
		kengine.ShutDown();
	}

	//system("pause");
	return 0;
}
