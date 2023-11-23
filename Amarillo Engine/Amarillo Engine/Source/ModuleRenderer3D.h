#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"
#include <string.h>
#include "../External/Glew/include/glew.h"
#include "../External/ImGui/imgui.h"
#include "ModuleConsole.h"
#include "ModuleMesh.h"
#include "ModuleTexture.h"

#include "..\External\MathGeoLib/include/Math/float3x3.h"
#include "..\External\MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include "../External/Devil/include/il.h"
#include "../External/Devil/include/ilu.h"
#include "../External/Devil/include/ilut.h"


//todo: REMOVE this before 1st delivery!!
#include "glmath.h"



#define MAX_LIGHTS 8

//Checkers specs
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true); 
	~ModuleRenderer3D();

	bool Init();
	void BindBuffers();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void InitDevil();

	void DrawBoundingBox(float3* vertices, float3 color);

	void CreateMainBuffer();
	void DeleteMainBuffer();
	GLuint GetSceneRenderTexture();
	GLuint GetGameRenderTexture();

	void RenderFromCamera(Camera3D* camera, bool debug_draw_enabled);
	bool RenderOnFrustrum(const Camera3D* camera, const AABB& aabb);


	void DebugDrawBox(const float3* corners, Color color, bool lines, const float& line_size);
	void DebugDraw(const Frustum& frustum, Color color, bool lines, const float& line_size);



public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	/*LoadFBX loader;
	LoadFBX* myModel = nullptr;*/
	Primitive* PrimitiveTest = nullptr;

	const char* myModel_path = nullptr;
	const char* myModel_texture_path = nullptr;

	uint VBO;
	uint EBO;
	uint VAO;

	bool activeWire = false;
	bool activeNormals = false;
	
	//Creat the scene on the window
	GLuint checkTexture;
	GLuint textureWidth, textureHeight;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	RenderTexture* scene_render_texture;
	RenderTexture* game_render_texture;

};