#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Entity.h"
#include "Lights.h"
#include "DataStructs.h"
#include "FirstPersonController.h"


class Scene
{
public:
	Scene(FirstPersonController* p_fpc);
	~Scene();
	void Render(std::vector<uint> p_indices);


	//object "constructor"
	uint CreateObject(Mesh* p_mesh, Material* p_material, SimplePixelShader* p_pixelShader,
		SimpleVertexShader* p_vertexShader);

	//accessors
	void SetEntityMaterial(uint p_entityIndex, Material* p_material);
	void SetEntityMesh(uint p_entityIndex, Mesh* p_mesh);
	void SetEntityPosition(uint p_entityIndex, DirectX::XMFLOAT3 p_position){}
	void SetEntityOrientation(uint p_entityIndex, DirectX::XMFLOAT3 p_orientation){}
	void SetEntityScale(uint p_entityIndex, float p_scale){}
	void SetEntityPixelShader(uint p_entityIndex, SimplePixelShader* p_pixel);
	void SetEntityVertexShader(uint p_entityIndex, SimpleVertexShader* p_vertex);
	//need getters


	//Light functions, this could be rearchitectured at a later date to be
	//cleaner
	uint AddDirectionalLight(DirectionalLight* p_dlight);
	uint AddSpotLight(SpotLight* p_slight);
	uint AddPointLight(PointLight* p_plight);

	//need getters and setters for lights still

	
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

	FirstPersonController* m_fpc;


};
