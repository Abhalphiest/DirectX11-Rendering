#pragma once
#include"../headers/Camera.h"
#include"../headers/DataStructs.h"
#include"../headers/Lights.h"

class FirstPersonController
{
public:
	Camera* camera;
	

	FirstPersonController(float p_aspratio)
	{
		camera = new Camera(p_aspratio);
	}
	~FirstPersonController() { if (camera)delete camera;}

private:
	FirstPersonController();

};