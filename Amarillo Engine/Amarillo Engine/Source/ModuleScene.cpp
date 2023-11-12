#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	app = app;
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	root_object = new GameObject("Scene");
	game_objects.push_back(root_object);

	//Per a poder afegir fills al game objects ho fas amb el child, pero dona problemes per al render de la mesh i l atextura, dirli al profe pq em cago al damunt
	child = new GameObject("Camera");
	child->AddComponent(ComponentTypes::CAMERA);
	root_object->AddChildren(child);


	return true;
}

update_status ModuleScene::Update()
{
	return update_status();
}

bool ModuleScene::CleanUp()
{

	return false;
}

GameObject* ModuleScene::CreateGameObject(std::string name)
{
	GameObject* newObject = new GameObject(name);
	game_objects.push_back(newObject);
	newObject->SetParent(root_object);
	return newObject;
}

GameObject* ModuleScene::CreateChildObject(std::string name)
{
	//Falta hacer que se pushee en el vector de game_objects para que pille el render preguntaprofe
	GameObject* newObject = new GameObject(name);
	root_object->AddChildren(newObject);
	newObject->SetParent(root_object);
	return newObject;
}

std::vector<GameObject*> ModuleScene::GetGameObjects()
{
	return game_objects;
}
