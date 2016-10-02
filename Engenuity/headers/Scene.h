#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Entity.h"
#include "Lights.h"
#include "DataStructs.h"


class Scene
{
public:
	Scene();
	~Scene();
	void Render();


	//object constructors and destructors
	uint CreateObject();
	//overloads
	void RemoveObject(uint p_index);

	//accessors
	void SetEntityMaterial(uint p_entityIndex, uint p_materialIndex);
	void SetEntityMesh(uint p_entityIndex, uint p_meshIndex);


	//Light functions, this could be rearchitectured at a later date to be
	//cleaner
	uint AddDirectionalLight(DirectionalLight* p_dlight);
	uint AddSpotLight(DirectionalLight* p_slight);
	uint AddPointLight(DirectionalLight* p_plight);
	void RemoveDirectionalLight(uint p_dindex);
	void RemoveSpotLight(uint p_sindex);
	void RemovePointLight(uint p_pindex);

	
private:
	std::vector<Mesh*> m_meshList;  //holds meshes
	std::vector<Material*> m_materialList; //holds materials
	std::vector<Collider> m_colliders;  //hold bounding boxes, etc
	std::vector<WorldData> m_worldDatas; //to hold position, orientation, scale, velocity, etc..
	std::vector<SimplePixelShader*> m_pixelShaders;
	std::vector<SimpleVertexShader*> m_vertexShaders;

	std::vector<DirectionalLight*> m_dlightList; //three separate lists for lights.. for now
	std::vector<SpotLight*> m_slightList;
	std::vector<PointLight*> m_plightList;

	std::vector<Camera*> m_cameraList;

	//FirstPersonController m_fpc;

	uint m_currentCamera;

};
