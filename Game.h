#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Dorsey_GGP\Lights.h"
#include "Mesh.h"
#include "Dorsey_GGP\Entity.h"
#include "Dorsey_GGP\Camera.h"
#include "Dorsey_GGP\Material.h"
#define CAMERA_DELTA .005
#define MOVE_SCALE 10

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

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();



	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//Entities for assignment 3
	Entity* e1, *e2, *e3;
	Mesh* mesh1, *mesh2, *mesh3;

	//Camera for assignment 4
	Camera* camera;

	//stuff for our material, assignment 4
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//light for assignment 4
	DirectionalLight dlight, dlight2;
	PointLight plight;

	//textures for assignment 5
	ID3D11ShaderResourceView* metalTextureSRV;
	ID3D11ShaderResourceView* woodTextureSRV;
	ID3D11SamplerState* sampler;
};

