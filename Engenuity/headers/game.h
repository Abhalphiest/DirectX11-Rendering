#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Lights.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "FirstPersonController.h"
#include "Scene.h"
#include "SceneManager.h"
#define CAMERA_DELTA .005
#define MOVE_SCALE 9

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

    // This can be moved to a more appropriate place later
    enum Game_State
    {
        BEGIN,
        PLAYING,
        OVER
    };

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry();

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

    Game_State gs;

	SceneManager* scenemanager;
	/*
	Mesh* mesh1;
	
	FirstPersonController* fpc;
	//Camera for assignment 4
	Camera* camera;

	//stuff for our material, assignment 4
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	

	//light for assignment 4
	DirectionalLight dlight, dlight2;
	PointLight plight;

	//textures for assignment 5
	ID3D11ShaderResourceView* armchairTextureSRV, *armchairNormalSRV, *armchairSpecSRV;
	
	ID3D11ShaderResourceView* defaultSRV,*defaultNSRV; //for the materials with no particular specs, normals, or mults
	ID3D11SamplerState* sampler;

	Scene* scene;
	uint light1, light2, light3;
	uint object1, object2, object3, object4;
	*/
};

