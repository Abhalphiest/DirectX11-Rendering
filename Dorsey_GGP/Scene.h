#pragma once
#include <DirectXMath.h>
#include "../Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Entity.h"
#include "Lights.h"


class Scene
{
public:
	Scene();
	~Scene();
	void Render();

	//material list functions
	uint CreateMaterial(Material* p_material);
	//removal can fail if the material still has references
	bool RemoveMaterial(uint p_materialIndex);

	//mesh list functions
	uint CreateMaterial(Material* p_material);
	//removal can fail if the material still has references
	bool RemoveMaterial(uint p_materialIndex);

	//entity list functions
	uint CreateEntity();
	uint CreateEntity(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT3 p_orientation);
	uint CreateEntity(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT3 p_orientation, uint p_MeshIndex, uint p_MaterialIndex);
	void RemoveEntity(uint p_index);
	void SetEntityMaterial(uint p_entityIndex, uint p_materialIndex);
	void SetEntityMesh(uint p_entityIndex, uint p_meshIndex);

	//camera functions
	uint AddCamera(Camera* p_camera);
	void RemoveCamera(uint p_index);
	void SetCamera(uint p_index);

	//Light functions, this could be rearchitectured at a later date to be
	//cleaner
	uint AddDirectionalLight(DirectionalLight* p_dlight);
	uint AddSpotLight(DirectionalLight* p_slight);
	uint AddPointLight(DirectionalLight* p_plight);
	void RemoveDirectionalLight(uint p_dindex);
	void RemoveSpotLight(uint p_sindex);
	void RemovePointLight(uint p_pindex);

	
private:
	std::vector<Mesh*> m_meshList;
	std::vector<Material*> m_materialList;
	std::vector<Entity*> m_entityList;
	std::vector<DirectionalLight*> m_dlightList; //three separate lists for lights.. for now
	std::vector<SpotLight*> m_slightList;
	std::vector<PointLight*> m_plightList;
	std::vector<Camera*> m_cameraList;

	uint m_currentCamera;

};
