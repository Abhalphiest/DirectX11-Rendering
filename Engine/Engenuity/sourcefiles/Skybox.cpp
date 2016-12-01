#include"../headers/Skybox.h"
#include"../headers/DXCore.h"
#include "WICTextureLoader.h" // From DirectX Tool Kit
#include "DDSTextureLoader.h"


Skybox::Skybox(Mesh* mesh, char* mappath, int rotate,ID3D11Device* device, ID3D11DeviceContext* context)
{
	mesh->GetInstance();
	skyboxMesh.push_back(mesh);
	//skybox stuff, will be moved
	skyVS = new SimpleVertexShader(device, context);
	skyVS->GetInstance();
	skyPS = new SimplePixelShader(device, context);
	skyPS->GetInstance();
	if (!skyVS->LoadShaderFile(L"../Debug/skyVS.cso"))
		skyVS->LoadShaderFile(L"../skyVS.cso");
	if (!skyPS->LoadShaderFile(L"../Debug/skyPS.cso"))
		skyPS->LoadShaderFile(L"../skyPS.cso");

	//DirectX::CreateDDSTextureFromFile(device, L"Assets/Textures/SunnyCubeMap.dds", 0, &skySRV);
	ID3D11ShaderResourceView* srv;
	DirectX::CreateDDSTextureFromFile(device, L"Assets/Textures/SunnyCubeMap.dds", 0, &srv);
	skySRV.push_back(srv);
	// Create a rasterizer state so we can render backfaces
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &skyRastState);


	// Create a depth state so that we can accept pixels
	// at a depth less than or EQUAL TO an existing depth
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Make sure we can see the sky (at max depth)
	device->CreateDepthStencilState(&dsDesc, &skyDepthState);
	rotateFlags.push_back(rotate);
}
Skybox::~Skybox()
{
	for (auto i : skySRV)
	{
		i->Release();
	}
	for (auto m : skyboxMesh)
	{
		m->RemoveInstance();
	}
	skyDepthState->Release();
	skyRastState->Release();
	skyPS->RemoveInstance();
	skyVS->RemoveInstance();

}
void Skybox::Render(ID3D11Device* device, ID3D11DeviceContext* context, Camera* camera)
{
	for (int i = 0; i < skyboxMesh.size(); i++) //will draw back to front, hopefully
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// Grab the buffers
		ID3D11Buffer* skyVB = *skyboxMesh[i]->GetVertexBuffer();
		ID3D11Buffer* skyIB = skyboxMesh[i]->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
		context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

		// Set up shaders
		skyVS->SetMatrix4x4("view", camera->GetView());
		skyVS->SetMatrix4x4("projection", camera->GetProjection());
		skyVS->CopyAllBufferData();
		skyVS->SetShader();

		skyPS->SetShaderResourceView("cubemap", skySRV[i]);
		skyPS->CopyAllBufferData();
		skyPS->SetShader();

		// Set the proper render states
		context->RSSetState(skyRastState);
		context->OMSetDepthStencilState(skyDepthState, 0);

		// Actually draw
		context->DrawIndexed(skyboxMesh[i]->GetIndexCount(), 0, 0);

		// Reset the states!
		context->RSSetState(0);
		context->OMSetDepthStencilState(0, 0);
	}
}
int Skybox::AddLayer(Mesh*mesh, char* mappath, int rotate, int layernum)
{
	return layernum;
}
void Skybox::RemoveLayer(int layernum)
{

}
void Skybox::clearSkybox()
{
	for (auto i : skySRV)
	{
		i->Release();
	}
	for (auto m : skyboxMesh)
	{
		m->RemoveInstance();
	}
	skySRV.clear();
	skyboxMesh.clear();
	rotateFlags.clear();
}