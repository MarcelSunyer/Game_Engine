#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Module.h"
#include "Globals.h"
#include "../External/Parson/parson.h"

#include "../External/MathGeoLib/include/MathBuildConfig.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

#include <string>
#include <list>

class JSON_Doc
{
public:
	JSON_Doc();
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	JSON_Doc(JSON_Doc& doc);
	~JSON_Doc();

	void SetString(const std::string& set, const char* str);
	void SetBool(const std::string& set, bool bo);
	void SetNumber(const std::string& set, double nu);
	void SetNumber3(const std::string& set, float3 val);
	void SetNumber4(const std::string& set, float4 val);
	const char* GetString(const std::string& str, const char* defaul = "");
	const bool GetBool(const std::string& bo, bool defaul = false);
	const double GetNumber(const std::string& nu, double defaul = 0);
	const float3 GetNumber3(const std::string& fl, float3 defaul = float3(0, 0, 0));
	const float4 GetNumber4(const std::string& fl, float4 defaul = float4(0, 0, 0, 0));

	void SetArray(const std::string& set);
	void ClearArray(const std::string& arr);
	void RemoveArrayIndex(const std::string& arr, int index);
	void AddStringToArray(const std::string& arr, const char* str);
	void AddBoolToArray(const std::string& arr, bool bo);
	void AddNumberToArray(const std::string& arr, double set);
	void AddSectionToArray(const std::string& arr);
	bool MoveToSectionFromArray(const std::string& arr, int index);
	const int GetArrayCount(const std::string& set) const;
	const char* GetStringFromArray(const std::string& arr, int index);
	const bool GetBoolFromArray(const std::string& arr, int index);
	const double GetNumberFromArray(const std::string& arr, int index);

	bool MoveToSection(const std::string& set);
	void RemoveSection(const std::string& set);
	void MoveToRoot();
	void AddSection(const std::string& set);

	JSON_Doc GetJsonNode();

	void Clear();

	std::string GetPath();
	void Save();
	void CleanUp();

private:
	bool FindValue(const char* str, json_value_type type);
	bool FindArrayValue(const char* array, int index, json_value_type type);

private:
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;
	std::string path;
};

class ModuleJSON : public Module
{
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	bool Awake();
	bool CleanUp();

	bool TextCmp(const char* text1, const char* text2);

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char* path, const char* name, const char* extension);
	JSON_Doc* CreateJSON(const char* path);
	void UnloadJSON(JSON_Doc* path);

private:
	std::list<JSON_Doc*> jsons;
};

#endif // !_JSONLOADER_H_