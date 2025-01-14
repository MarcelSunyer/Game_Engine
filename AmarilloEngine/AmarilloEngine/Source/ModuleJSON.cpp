#include "ModuleJSON.h"
#include "Globals.h"

#include <string>
#include <list>
#include "ModuleCamera3D.h"


ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleJSON::~ModuleJSON()
{
}

bool ModuleJSON::Awake()
{
	bool ret = true;

	LOG("Loading JSON Module");

	return ret;
}

JSON_Doc* ModuleJSON::LoadJSON(const char* path)
{
	JSON_Value* user_data = json_parse_file(path);
	JSON_Object* root_object = json_value_get_object(user_data);

	if (user_data == nullptr || root_object == nullptr)
	{
		LOG("Error loading %s", path);
	}

	JSON_Doc* new_doc = new JSON_Doc(user_data, root_object);

	return new_doc;
}

JSON_Doc* ModuleJSON::CreateJSON()
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);
	JSON_Doc* new_doc = new JSON_Doc(root_value, root_object);
	return new_doc;
}

void ModuleJSON::SaveJson(JSON_Doc* json, const char* path)
{
	json_serialize_to_file_pretty(json->GetValue(), path);
}

void ModuleJSON::UnloadJSON(JSON_Doc* json)
{
	json->CleanUp();
	delete json;
}

bool ModuleJSON::CleanUp()
{
	LOG("Unloading JSON Module");

	//TODO: Pasar por todos los jsonms para ver si se han borrado correctamente

	//for (std::list<JSON_Doc>::iterator it = jsons.begin(); it != jsons.end();)
	//{
	//	(*it).CleanUp();
	//}

	//jsons.clear();

	return true;
}

JSON_Doc::JSON_Doc()
{
	this->value = json_value_init_object();
	this->object = json_value_get_object(this->value);
}

JSON_Doc::JSON_Doc(JSON_Value* _value, JSON_Object* _object)
{
	value = _value;
	object = _object;
	root = _object;
}

JSON_Doc::JSON_Doc(std::string path, JSON_Object* object, JSON_Value* value) : path(path), object(object), value(value)
{
}

JSON_Doc::JSON_Doc(const JSON_Doc& doc)
{
	value = doc.value;
	object = doc.object;
	root = object;
}

JSON_Doc::~JSON_Doc()
{
}

void JSON_Doc::SetString(const std::string& set, const char* str)
{
	json_object_dotset_string(object, set.c_str(), str);
}

void JSON_Doc::SetBool(const std::string& set, bool bo)
{
	json_object_dotset_boolean(object, set.c_str(), bo);
}

void JSON_Doc::SetNumber(const std::string& set, double nu)
{
	json_object_dotset_number(object, set.c_str(), nu);
}

void JSON_Doc::SetNumber3(const std::string& set, float3 val)
{
	SetArray(set);
	AddNumberToArray(set, val.x);
	AddNumberToArray(set, val.y);
	AddNumberToArray(set, val.z);
}

void JSON_Doc::SetNumber4(const std::string& set, float4 val)
{
	SetArray(set);
	AddNumberToArray(set, val.x);
	AddNumberToArray(set, val.y);
	AddNumberToArray(set, val.w);
	AddNumberToArray(set, val.z);
}

void JSON_Doc::SetUid(const std::string& set, uuids::uuid val)
{
	std::string string = uuids::to_string<char>(val);
	SetString(set, string.c_str());
}

void JSON_Doc::SetArray(const std::string& set)
{
	JSON_Value* va = json_value_init_array();
	JSON_Array* arr = json_value_get_array(va);

	json_object_dotset_value(object, set.c_str(), va);
}

void JSON_Doc::ClearArray(const std::string& arr)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_clear(array);
	}
}

void JSON_Doc::RemoveArrayIndex(const std::string& arr, int index)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_remove(array, index);
	}
}

const int JSON_Doc::GetArrayCount(const std::string& set) const
{
	int ret = 0;

	JSON_Array* array = json_object_get_array(object, set.c_str());

	if (array != nullptr)
	{
		ret = json_array_get_count(array);
	}

	return ret;
}

const char* JSON_Doc::GetStringFromArray(const std::string& arr, int index)
{
	const char* ret = nullptr;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONString))
		{
			ret = json_array_get_string(array, index);
		}
	}

	return ret;
}

const bool JSON_Doc::GetBoolFromArray(const std::string& arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONBoolean))
		{
			ret = json_array_get_boolean(array, index);
		}
	}

	return ret;
}

const double JSON_Doc::GetNumberFromArray(const std::string& arr, int index)
{
	double ret = 0;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONNumber))
		{
			ret = json_array_get_number(array, index);
		}
	}

	return ret;
}

void JSON_Doc::AddStringToArray(const std::string& arr, const char* str)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_string(array, str);
	}
}

void JSON_Doc::AddBoolToArray(const std::string& arr, bool bo)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_boolean(array, bo);
	}
}

void JSON_Doc::AddNumberToArray(const std::string& arr, double set)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_number(array, set);
	}
}

void JSON_Doc::AddSectionToArray(const std::string& arr)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_value(array, json_value_init_object());
	}
}

bool JSON_Doc::MoveToSectionFromArray(const std::string& arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		JSON_Object* obj = json_array_get_object(array, index);

		object = obj;
		ret = true;
	}

	return ret;
}

const char* JSON_Doc::GetString(const std::string& str, const char* defaul)
{
	const char* ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONString))
		ret = json_object_dotget_string(object, str.c_str());

	return ret;
}

const bool JSON_Doc::GetBool(const std::string& str, bool defaul)
{
	bool ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONBoolean))
		ret = json_object_dotget_boolean(object, str.c_str());

	return ret;
}

const double JSON_Doc::GetNumber(const std::string& str, double defaul)
{
	double ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONNumber))
		ret = json_object_dotget_number(object, str.c_str());

	return ret;
}

const float3 JSON_Doc::GetNumber3(const std::string& fl, float3 defaul)
{
	float3 ret = float3::zero;

	ret = defaul;

	JSON_Array* array = json_object_get_array(object, fl.c_str());

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.z = GetNumberFromArray(fl, 2);
	}

	return ret;
}

const float4 JSON_Doc::GetNumber4(const std::string& fl, float4 defaul)
{
	float4 ret = float4::zero;

	ret = defaul;

	JSON_Array* array = json_object_get_array(object, fl.c_str());

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.w = GetNumberFromArray(fl, 2);
		ret.z = GetNumberFromArray(fl, 3);
	}

	return ret;
}

uuids::uuid JSON_Doc::GetUid(const std::string& set)
{
	std::string string = GetString(set);

	std::optional<uuids::uuid> optionalUid = uuids::uuid::from_string(string);

	if (!optionalUid.has_value())
	{
		return uuids::uuid();
	}

	return optionalUid.value();
}

bool JSON_Doc::MoveToSection(const std::string& set)
{
	bool ret = false;

	JSON_Object* obj = json_object_get_object(object, set.c_str());

	if (obj != nullptr)
	{
		object = obj;
		ret = true;
	}

	return ret;
}

void JSON_Doc::RemoveSection(const std::string& set)
{
	json_object_remove(object, set.c_str());
}

void JSON_Doc::MoveToRoot()
{
	object = root;
}

void JSON_Doc::AddSection(const std::string& set)
{
	json_object_set_value(object, set.c_str(), json_value_init_object());
}

JSON_Doc JSON_Doc::GetJsonNode()
{
	return JSON_Doc(*this);
}

JSON_Value* JSON_Doc::GetValue()
{
	return value;
}

void JSON_Doc::Clear()
{
	json_value_free(value);
	value = json_value_init_object();
	object = json_value_get_object(value);
	root = object;
}

//void JSON_Doc::Save()
//{
//	json_serialize_to_file_pretty(value, path.c_str());
//}

void JSON_Doc::CleanUp()
{
	if (value != nullptr)
	{
		json_value_free(value);
		value = nullptr;
		object = nullptr;
	}
}

bool JSON_Doc::FindValue(const char* str, json_value_type type)
{
	bool ret = false;

	JSON_Value* val = json_object_dotget_value(object, str);

	if (val != nullptr && json_value_get_type(val) == type)
		ret = true;

	return ret;
}

bool JSON_Doc::FindArrayValue(const char* arr, int index, json_value_type type)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr);

	if (array != nullptr)
	{
		JSON_Value* val = json_array_get_value(array, index);

		if (val != nullptr && json_value_get_type(val) == type)
			ret = true;
	}

	return ret;
}


GameObject* JSON_Doc::GetGameObject(const char* key) const
{
	JSON_Value* gameObjectValue = json_object_get_value(object, key);

	if (gameObjectValue != nullptr && json_value_get_type(gameObjectValue) == JSONObject) 
	{
		JSON_Object* gameObjectObject = json_value_get_object(gameObjectValue);

		// Create a new GameObject
		GameObject* gameObject = new GameObject();
		
		// Get Name
		const char* name = json_object_get_string(gameObjectObject, "Name");
		gameObject->mName = (name != nullptr) ? name : "";

		// Get UID
		gameObject->UID = json_object_get_string(gameObjectObject, "UID");

		// Get Parent UID
		if (json_object_has_value_of_type(gameObjectObject, "Parent UID", JSONNumber)) {
			gameObject->parent->UID = json_object_get_string(gameObjectObject, "Parent UID");
		}

		// Get Children UID
		if (json_object_has_value_of_type(gameObjectObject, "Children UID", JSONArray)) {
			JSON_Array* childrenArray = json_object_get_array(gameObjectObject, "Children UID");
			size_t numChildren = json_array_get_count(childrenArray);

			for (size_t i = 0; i < numChildren; ++i) {
				std::string childUID = json_array_get_string(childrenArray, i);
				
			}

		}

		// Get Components
		if (json_object_has_value_of_type(gameObjectObject, "Components", JSONArray)) {
			JSON_Array* componentsArray = json_object_get_array(gameObjectObject, "Components");
			size_t numComponents = json_array_get_count(componentsArray);

			for (size_t i = 0; i < numComponents; ++i) {
				JSON_Value* componentValue = json_array_get_value(componentsArray, i);

				if (json_value_get_type(componentValue) == JSONObject) {
					JSON_Object* componentObject = json_value_get_object(componentValue);

					// Create a new Component
					Component* component = new Component();

					// Call the existing GetComponent function to retrieve individual Component properties
					GetComponent(componentObject, *component);

					// Add the Component to the GameObject's components vector
					gameObject->components.push_back(component);
				}
			}
		}

		return gameObject;
	}

	return nullptr;
}

Component* JSON_Doc::GetComponent(const char* key) const
{
	JSON_Value* componentValue = json_object_get_value(object, key);

	if (componentValue != nullptr && json_value_get_type(componentValue) == JSONObject) {

		JSON_Object* componentObject = json_value_get_object(componentValue);

		// Create a new Component
		Component* component = new Component();

		// Get common properties
		std::string type = json_object_get_string(componentObject, "Type");

		if (type == "Transform") {

			component->type = ComponentTypes::TRANSFORM;

		}

		if (type == "Mesh") {

			component->type = ComponentTypes::MESH;

		}

		if (type == "Texture") {

			component->type = ComponentTypes::TEXTURE;

		}

		if (type == "Camera") {

			component->type = ComponentTypes::CAMERA;

		}

		return component;
	}

	return nullptr;
}

void JSON_Doc::SetComponent(const char* key, const Component& component)
{
	JSON_Value* componentValue = json_value_init_object();
	JSON_Object* componentObject = json_value_get_object(componentValue);

	switch (component.type)
	{
	case NONE:
		// Handle NONE case (if needed)
		break;

	case TRANSFORM:
		json_object_set_string(componentObject, "Type", "Transform");
		// Additional properties specific to the Transform component can be added here
		break;

	case MESH:
		json_object_set_string(componentObject, "Type", "Mesh");
		// Additional properties specific to the Mesh component can be added here
		break;

	case TEXTURE:
		json_object_set_string(componentObject, "Type", "Texture");
		// Additional properties specific to the Material component can be added here
		break;

	case CAMERA:
		json_object_set_string(componentObject, "Type", "Camera");
		// Additional properties specific to the Camera component can be added here
		break;
	}

	// Add the component object to the main object
	json_object_set_value(object, key, componentValue);
}

void JSON_Doc::SetGameObject(const char* key, const GameObject& gameObject)
{
	JSON_Value* gameObjectValue = json_value_init_object();
	JSON_Object* gameObjectObject = json_value_get_object(gameObjectValue);

	// Set Name

	json_object_set_string(gameObjectObject, "Name", gameObject.mName.c_str());

	// Set Position

	JSON_Value* positionValue = json_value_init_array();
	JSON_Array* positionArray = json_value_get_array(positionValue);
	json_array_append_number(positionArray, gameObject.transform->world_position.x);
	json_array_append_number(positionArray, gameObject.transform->world_position.y);
	json_array_append_number(positionArray, gameObject.transform->world_position.z);
	json_object_set_value(gameObjectObject, "Position", positionValue);

	// Set Rotation

	JSON_Value* rotationValue = json_value_init_array();
	JSON_Array* rotationArray = json_value_get_array(rotationValue);
	json_array_append_number(rotationArray, gameObject.transform->world_rotation.x);
	json_array_append_number(rotationArray, gameObject.transform->world_rotation.y);
	json_array_append_number(rotationArray, gameObject.transform->world_rotation.z);
	json_object_set_value(gameObjectObject, "Rotation", rotationValue);

	// Set Scale

	JSON_Value* scaleValue = json_value_init_array();
	JSON_Array* scaleArray = json_value_get_array(scaleValue);
	json_array_append_number(scaleArray, gameObject.transform->world_scale.x);
	json_array_append_number(scaleArray, gameObject.transform->world_scale.y);
	json_array_append_number(scaleArray, gameObject.transform->world_scale.z);
	json_object_set_value(gameObjectObject, "Scale", scaleValue);

	// Set UID

	json_object_set_string(gameObjectObject, "UID", gameObject.UID.c_str());

	// Set Parent UID

	if (gameObject.parent != nullptr) {

		json_object_set_string(gameObjectObject, "Parent UID", gameObject.parent->UID.c_str());

	}

	// Set Children UID

	std::vector<std::string> childrenUID;

	for (auto& child : gameObject.children) {

		childrenUID.push_back(child->UID);

	}

	if (!childrenUID.empty()) {

		JSON_Value* childrenValue = json_value_init_array();
		JSON_Array* childrenArray = json_value_get_array(childrenValue);

		for (const auto& childUID : childrenUID) {

			json_array_append_string(childrenArray, childUID.c_str());

		}

		json_object_set_value(gameObjectObject, "Children UID", childrenValue);

	}

	// Save Components Info

	JSON_Value* componentsValue = json_value_init_array();
	JSON_Array* componentsArray = json_value_get_array(componentsValue);

	for (const auto& component : gameObject.components) {

		JSON_Value* componentValue = json_value_init_object();
		JSON_Object* componentObject = json_value_get_object(componentValue);

		// Call the existing SetGameObject function to set individual GameObject properties
		SetComponent(componentObject, *component);

		// Add the GameObject to the hierarchy array
		json_array_append_value(componentsArray, componentValue);
	}

	// Add the hierarchy array to the main object
	json_object_set_value(gameObjectObject, "Components", componentsValue);

	// Add the GameObject to the main array
	json_object_set_value(object, key, gameObjectValue);
}

void JSON_Doc::SetComponent(JSON_Object* componentObject, const Component& component)
{
	if (component.type == ComponentTypes::NONE) {

		// Handle NONE case (if needed)

	}
	else if (component.type == ComponentTypes::TRANSFORM) {

		json_object_set_string(componentObject, "Type", "Transform");

		ComponentTransform* transform = (ComponentTransform*)&component;

		json_object_set_number(componentObject, "Active", transform->active);

		// Translation

		JSON_Value* translationArrayValue = json_value_init_array();
		JSON_Array* translationArray = json_value_get_array(translationArrayValue);

		json_array_append_number(translationArray, transform->GetPosition().x);	
		json_array_append_number(translationArray, transform->GetPosition().y);
		json_array_append_number(translationArray, transform->GetPosition().z);

		json_object_set_value(componentObject, "Translation", translationArrayValue);

		// Rotation

		JSON_Value* rotationArrayValue = json_value_init_array();
		JSON_Array* rotationArray = json_value_get_array(rotationArrayValue);

		json_array_append_number(rotationArray, transform->GetRotation().x);	
		json_array_append_number(rotationArray, transform->GetRotation().y);
		json_array_append_number(rotationArray, transform->GetRotation().z);

		json_object_set_value(componentObject, "Rotation", rotationArrayValue);

		// Scale

		JSON_Value* scaleArrayValue = json_value_init_array();
		JSON_Array* scaleArray = json_value_get_array(scaleArrayValue);

		json_array_append_number(scaleArray, transform->GetScale().x);
		json_array_append_number(scaleArray, transform->GetScale().y);
		json_array_append_number(scaleArray, transform->GetScale().z);

		json_object_set_value(componentObject, "Scale", scaleArrayValue);

	}
	else if (component.type == ComponentTypes::MESH) {

		json_object_set_string(componentObject, "Type", "Mesh");

		ComponentMesh* mesh = (ComponentMesh*)&component;

		json_object_set_number(componentObject, "Active", mesh->active);

		json_object_set_number(componentObject, "Vertex Count", mesh->mesh_->ourVertex.size());		
		json_object_set_number(componentObject, "Index Count", mesh->mesh_->indices.size());		

	}
	else if (component.type == ComponentTypes::TEXTURE) {

		json_object_set_string(componentObject, "Type", "Texture");

		ComponentTexture* texture = (ComponentTexture*)&component;

		json_object_set_number(componentObject, "Active", texture->active);

	}
	else if (component.type == ComponentTypes::CAMERA) {

		json_object_set_string(componentObject, "Type", "Camera");

		ComponentCamera* camera = (ComponentCamera*)&component;

		json_object_set_number(componentObject, "Active", camera->active);

		// Pos (Aqui faltan cosas)

		JSON_Value* posArrayValue = json_value_init_array();
		JSON_Array* posArray = json_value_get_array(posArrayValue);

		json_array_append_number(posArray, camera->camera->GetPosition().x);
		json_array_append_number(posArray, camera->camera->GetPosition().y);
		json_array_append_number(posArray, camera->camera->GetPosition().z);

		json_object_set_value(componentObject, "Position", posArrayValue);

		// FOV

		json_object_set_number(componentObject, "FOV", camera->camera->GetVerticalFOV());

		// Near Plane

		json_object_set_number(componentObject, "Near Plane", camera->camera->GetNearPlaneDistance());

		// Far Plane

		json_object_set_number(componentObject, "Far Plane", camera->camera->GetFarPlaneDistance());

	}

}

void JSON_Doc::GetComponent(const JSON_Object* componentObject, Component& component) const {

	// Get common properties
	std::string type = json_object_get_string(componentObject, "Type");

	if (type == "Transform") {

		component.type = ComponentTypes::TRANSFORM;

	}
	else if (type == "Mesh") {

		component.type = ComponentTypes::MESH;
		
	}
	else if (type == "Texture") {

		component.type = ComponentTypes::TEXTURE;

	}
	else if (type == "Camera") {

		component.type = ComponentTypes::CAMERA;

	}

}
JSON_Arraypack* JSON_Doc::GetArray(const std::string& name)
{
	JSON_Array* arr = nullptr;
	while (arr == nullptr) { arr = json_object_dotget_array(object, name.data()); }
	JSON_Value* value = nullptr;
	while (value == nullptr) { value = json_array_get_value(arr, 0); }
	JSON_Arraypack* array_pack = new JSON_Arraypack(arr, value);
	arrays.push_back(array_pack);
	return array_pack;
}

JSON_Arraypack::~JSON_Arraypack()
{
	if (!arrays.empty()) {
		std::vector<JSON_Arraypack*>::iterator item = arrays.begin();
		for (; item != arrays.end(); ++item) {
			delete* item;
		}
		arrays.clear();
	}
}

void JSON_Arraypack::SetNumber(const std::string& name, const double& number)
{
	json_object_dotset_number(json_value_get_object(value), name.data(), number);
	json_array_append_value(arr, value);
}

double JSON_Arraypack::GetNumber(const std::string& name)
{
	return json_object_dotget_number(json_value_get_object(value), name.data());
}

void JSON_Arraypack::SetBoolean(const std::string& name, const bool& boolean)
{
	json_object_dotset_boolean(json_value_get_object(value), name.data(), boolean);
}

bool JSON_Arraypack::GetBoolean(const std::string& name)
{
	return json_object_dotget_boolean(json_value_get_object(value), name.data());
}

void JSON_Arraypack::SetColor(const std::string& name, const Color& color)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(json_value_get_object(value), name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, color.r);
	json_array_append_number(arr, color.g);
	json_array_append_number(arr, color.b);
	json_array_append_number(arr, color.a);
}

Color JSON_Arraypack::GetColor(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());

	Color color;
	color.r = json_array_get_number(arr, 0);
	color.g = json_array_get_number(arr, 1);
	color.b = json_array_get_number(arr, 2);
	color.a = json_array_get_number(arr, 3);

	return color;
}

void JSON_Arraypack::SetFloat3(const std::string& name, const float3& numbers)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(json_value_get_object(value), name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
}

float3 JSON_Arraypack::GetFloat3(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());

	float3 numbers;
	numbers.x = json_array_get_number(arr, 0);
	numbers.y = json_array_get_number(arr, 1);
	numbers.z = json_array_get_number(arr, 2);

	return numbers;
}

void JSON_Arraypack::SetQuat(const std::string& name, const Quat& numbers)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(json_value_get_object(value), name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
	json_array_append_number(arr, numbers.w);
}

Quat JSON_Arraypack::GetQuat(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());

	Quat quat;
	quat.x = json_array_get_number(arr, 0);
	quat.y = json_array_get_number(arr, 1);
	quat.z = json_array_get_number(arr, 2);
	quat.w = json_array_get_number(arr, 3);

	return quat;
}

void JSON_Arraypack::SetAnotherNode()
{
	// I hope when destroying the core value of the file everything is deleted :) 
	value = json_value_init_object();
	json_array_append_value(arr, value);
}

bool JSON_Arraypack::GetAnotherNode()
{
	++index;
	if (index < json_array_get_count(arr)) {
		value = json_array_get_value(arr, index);
		return true;
	}
	else {
		return false;
	}
}

void JSON_Arraypack::GetFirstNode()
{
	index = 0;
	value = json_array_get_value(arr, index);
}

void JSON_Arraypack::GetNode(const uint& index)
{
	this->index = index;
	value = json_array_get_value(arr, index);
}

uint JSON_Arraypack::GetArraySize()
{
	return json_array_get_count(arr);
}

void JSON_Arraypack::SetString(const std::string& name, const std::string& string_parameter)
{
	json_object_dotset_string(json_value_get_object(value), name.data(), string_parameter.data());
}

const char* JSON_Arraypack::GetString(const std::string& name)
{
	return json_object_dotget_string(json_value_get_object(value), name.data());
}

JSON_Arraypack* JSON_Arraypack::InitNewArray(const std::string& name)
{
	JSON_Value* val = json_value_init_array();
	json_object_dotset_value(json_value_get_object(value), name.data(), val);

	JSON_Arraypack* array_pack = new JSON_Arraypack(json_value_get_array(val), json_value_init_object());
	arrays.push_back(array_pack);

	return array_pack;
}

JSON_Arraypack* JSON_Arraypack::GetArray(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(json_value_get_object(value), name.data());
	JSON_Value* value = json_array_get_value(arr, 0);
	JSON_Arraypack* array_pack = new JSON_Arraypack(arr, value);
	arrays.push_back(array_pack);

	return array_pack;
}

void JSON_Doc::StartSave()
{
	save_value = json_parse_file(path.data());
	save_object = json_object(save_value);
}

void JSON_Doc::FinishSave()
{
	json_serialize_to_file_pretty(save_value, path.data());
	json_value_free(save_value);
	save_value = nullptr;
	save_object = nullptr;
}

JSON_Arraypack* JSON_Doc::InitNewArray(const std::string& name)
{
	JSON_Value* val = json_value_init_array();
	json_object_dotset_value(save_object, name.data(), val);

	JSON_Arraypack* array_pack = new JSON_Arraypack(json_value_get_array(val), json_value_init_object());
	arrays.push_back(array_pack);

	return array_pack;
}