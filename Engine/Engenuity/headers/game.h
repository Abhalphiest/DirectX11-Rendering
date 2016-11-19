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
#include <SpriteBatch.h>
#include <SpriteFont.h>
#define CAMERA_DELTA .005
#define MOVE_SCALE 3

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

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
    Game_State gs;
    DirectX::SpriteBatch* spriteBatch;
    DirectX::SpriteFont* spriteFont;

	SceneManager* scenemanager;

};

