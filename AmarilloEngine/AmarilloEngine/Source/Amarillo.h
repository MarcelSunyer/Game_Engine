#pragma once

#include <functional>


class GameObject;
class ComponentTransform;

extern "C" __declspec(dllexport) class Amarillo {
public:

	virtual void Start() {};

	virtual void Update() {};

	GameObject* gameObject = nullptr;
	ComponentTransform* transform;

};
