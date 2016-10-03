#include"../headers/Scene.h"

Scene::Scene(FirstPersonController* p_fpc)
{
	m_fpc = p_fpc;
	m_meshList = std::vector<Mesh*>();
	m_materialList = std::vector<Material*>();
	m_colliders = std::vector<Collider>();
	m_worldDatas = std::vector<WorldData>();
	m_pixelShaders = std::vector<SimplePixelShader*>();
	m_vertexShaders = std::vector<SimpleVertexShader*>();

	m_dlightList = std::vector<DirectionalLight*>();
	m_slightList = std::vector<SpotLight*>();
	m_plightList = std::vector<PointLight*>();

}
Scene::~Scene()
{
	for (std::vector<Mesh*>::size_type i = 0; i != m_meshList.size(); i++) {
		m_meshList[i]->RemoveInstance();
		m_materialList[i]->RemoveInstance();
		m_pixelShaders[i]->RemoveInstance();
		m_vertexShaders[i]->RemoveInstance();
	}
	for (std::vector<DirectionalLight*>::size_type i = 0; i != m_dlightList.size(); i++)
	{
		delete m_dlightList[i];
	}
	for (std::vector<PointLight*>::size_type i = 0; i != m_plightList.size(); i++)
	{
		delete m_plightList[i];
	}
	for (std::vector<SpotLight*>::size_type i = 0; i != m_slightList.size(); i++)
	{
		delete m_slightList[i];
	}
}
void Scene::Render(std::vector<uint> p_indices)
{

}


//object constructors and destructors
uint Scene::CreateObject(Mesh* p_mesh, Material* p_material, SimplePixelShader* p_pixelShader,
						SimpleVertexShader* p_vertexShader)
{
	p_mesh->GetInstance();
	m_meshList.push_back(p_mesh);

	p_material->GetInstance();
	m_materialList.push_back(p_material);

	p_vertexShader->GetInstance();
	m_vertexShaders.push_back(p_vertexShader);

	p_pixelShader->GetInstance();
	m_pixelShaders.push_back(p_pixelShader);

	Collider collider = Collider(); //need to change later
	m_colliders.push_back(collider);
	
	WorldData worlddata = WorldData();
	m_worldDatas.push_back(worlddata);

	return m_meshList.size() - 1;
}


//accessors
void Scene::SetEntityMaterial(uint p_entityIndex, Material* p_material)
{
	m_materialList[p_entityIndex]->RemoveInstance();
	m_materialList[p_entityIndex] = p_material;
	p_material->GetInstance();
}
void Scene::SetEntityMesh(uint p_entityIndex, Mesh* p_mesh)
{
	m_meshList[p_entityIndex]->RemoveInstance();
	m_meshList[p_entityIndex] = p_mesh;
	p_mesh->GetInstance();
}
void Scene::SetEntityPixelShader(uint p_entityIndex, SimplePixelShader* p_pixel)
{
	m_pixelShaders[p_entityIndex]->RemoveInstance();
	m_pixelShaders[p_entityIndex] = p_pixel;
	p_pixel->GetInstance();
}
void Scene::SetEntityVertexShader(uint p_entityIndex, SimpleVertexShader* p_vertex)
{
	m_vertexShaders[p_entityIndex]->RemoveInstance();
	m_vertexShaders[p_entityIndex] = p_vertex;
	p_vertex->GetInstance();
}


//Light functions, this could be rearchitectured at a later date to be
//cleaner
uint Scene::AddDirectionalLight(DirectionalLight* p_dlight)
{
	m_dlightList.push_back(p_dlight);
	return m_dlightList.size() - 1;
}
uint Scene::AddSpotLight(SpotLight* p_slight)
{
	m_slightList.push_back(p_slight);
	return m_slightList.size() - 1;
}
uint Scene::AddPointLight(PointLight* p_plight)
{
	m_plightList.push_back(p_plight);
	return m_plightList.size() - 1;
}
