#include"../headers/Scene.h"

Scene::Scene()
{

}
Scene::~Scene()
{

}
void Scene::Render()
{

}


//object constructors and destructors
uint Scene::CreateObject()
{
	return 1;
}
//overloads
void Scene::RemoveObject(uint p_index)
{

}

//accessors
void Scene::SetEntityMaterial(uint p_entityIndex, uint p_materialIndex)
{

}
void Scene::SetEntityMesh(uint p_entityIndex, uint p_meshIndex)
{

}


//Light functions, this could be rearchitectured at a later date to be
//cleaner
uint Scene::AddDirectionalLight(DirectionalLight* p_dlight)
{
	return 1;
}
uint Scene::AddSpotLight(DirectionalLight* p_slight)
{
	return 1;
}
uint Scene::AddPointLight(DirectionalLight* p_plight)
{
	return 1;
}
void Scene::RemoveDirectionalLight(uint p_dindex)
{
	
}
void Scene::RemoveSpotLight(uint p_sindex)
{

}
void Scene::RemovePointLight(uint p_pindex)
{

}
