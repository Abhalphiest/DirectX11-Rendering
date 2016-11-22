#pragma once
#include"DXCore.h"
#include"Mesh.h"
#include"SimpleShader.h"
#include<vector>
#include"Camera.h"
class Skybox
{

	//LAYERS ARE NOT YET IMPLEMENTED

private:
	std::vector<ID3D11ShaderResourceView*> skySRV; //list of skybox layer textures
	ID3D11RasterizerState* skyRastState;
	ID3D11DepthStencilState* skyDepthState;
	SimpleVertexShader* skyVS; //the shaders for vertexshaders
	SimplePixelShader* skyPS;
	std::vector<Mesh*> skyboxMesh; //list of skybox layer meshes
	std::vector<int> rotateFlags; //whether or not to rotate the skybox over time
							//negative for clockwise, positive for counter clockwise, 0 for static
	Skybox();
public:
	Skybox(Mesh* mesh, char* mappath, int rotate, ID3D11Device* device, ID3D11DeviceContext* context);
	~Skybox();
	void Render(ID3D11Device* device, ID3D11DeviceContext* context,Camera* camera);
	int AddLayer(Mesh*mesh, char* mappath, int rotate, int layernum); //layernum is where it should be in relationship to other layers - layer 0 is
																	//farthest from world, layer n is closest
	void RemoveLayer(int layernum);
	int getNumLayers() { return skySRV.size();}
	void clearSkybox();


};