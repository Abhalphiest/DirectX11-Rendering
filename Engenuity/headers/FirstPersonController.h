#pragma once
#include"../headers/Camera.h"
#include"../headers/DataStructs.h"
#include"../headers/Lights.h"

class FirstPersonController
{
	WorldData wdata;
	Camera* camera;
	SpotLight flashlight;
	bool lightToggle;
	Collider collider;

	FirstPersonController(float p_aspratio)
	{
		camera = new Camera(p_aspratio);
		lightToggle = true;
		//rest of initialization goes here
	}
	~FirstPersonController() { if (camera)delete camera;}

private:
	FirstPersonController();

};