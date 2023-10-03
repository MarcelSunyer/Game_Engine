#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR



#include "Module.h"
#include "Globals.h"
#include <vector>


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void AddFPS(float FPS);

	void ShowAboutInfo();

	void AdjustBrightness(float factor);	

	bool StyleTypes(const char* label);
	
	std::vector<float> vecFPSLog;
	const char* lastValue = "";

public:  //Windows Settings
	//Brightness value
	float brightnessFactor = 1.5f;

	
	//Show config window
	bool showWindow = true;
public:
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;
	bool IoConfirmer = false;

};


#endif // MODULE_EDITOR

