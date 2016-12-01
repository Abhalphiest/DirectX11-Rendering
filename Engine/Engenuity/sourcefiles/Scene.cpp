#include"../headers/Scene.h"
#include"../headers/Game.h"

Scene::Scene(FirstPersonController* p_fpc, ID3D11Device* p_device, ID3D11DeviceContext* p_context)
{
	m_shadowVS = new SimpleVertexShader(p_device, p_context);
	if (!m_shadowVS->LoadShaderFile(L"Debug/ShadowVS.cso"))
		m_shadowVS->LoadShaderFile(L"ShadowVS.cso");
	m_shadowVS->GetInstance();

	m_device = p_device;
	m_fpc = p_fpc;
	m_meshList = std::vector<Mesh*>();
	m_materialList = std::vector<Material*>();
	m_colliders = std::vector<Collider>();
	m_worldDatas = std::vector<WorldData>();

	m_dlightList = std::vector<DirectionalLight>();
	m_slightList = std::vector<SpotLight>();
	m_plightList = std::vector<PointLight>();

	m_dlightShadowViewList = std::vector<DirectX::XMFLOAT4X4>();
	m_dlightShadowProjectionList = std::vector<DirectX::XMFLOAT4X4>();
	m_dlightSRVList = std::vector<ID3D11ShaderResourceView*>();
	m_shadowMapSize = 1026; //randomly chosen, may change later


	// Create the special "comparison" sampler state for shadows
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; // Could be anisotropic
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	m_device->CreateSamplerState(&shadowSampDesc, &m_shadowSampler);

	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000; // Multiplied by (smallest possible value > 0 in depth buffer)
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	m_device->CreateRasterizerState(&shadowRastDesc, &m_shadowRasterizer);

}
Scene::~Scene()
{
	for (std::vector<Mesh*>::size_type i = 0; i != m_meshList.size(); i++) {
		m_meshList[i]->RemoveInstance();
		m_materialList[i]->RemoveInstance();
	}

	//clean up shadow memory
	for (int i = 0; i < m_dlightSRVList.size();i++)
	{
		m_dlightSRVList[i]->Release();
		m_dlightDSVList[i]->Release();
	}
	m_shadowSampler->Release();
	m_shadowRasterizer->Release();
	m_shadowVS->RemoveInstance();

	
}
void Scene::Render(ID3D11DeviceContext* context,
                    std::vector<uint> p_indices,
                    std::vector<uint> p_dlights,
                    std::vector<uint> p_plights,
                    std::vector<uint> p_slights)
{

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	std::vector<SimplePixelShader*> ps_list; //to avoid pixel buffer data passing redundancy
	for (std::vector<uint>::size_type i = 0; i != p_indices.size(); i++)
	{
		SimplePixelShader* ps = m_materialList[p_indices[i]]->GetPixelShader();
		if (std::find(ps_list.begin(), ps_list.end(), ps) == ps_list.end()) //not in our list yet
		{
			for (std::vector<uint>::size_type j = 0; j != p_dlights.size(); j++)
			{
				ps->SetData(
					"dlight" + std::to_string(j),   // The name of the variable in the shader
					&m_dlightList[j],               // The address of the data to copy
					sizeof(DirectionalLight));      // The size of the data to copy
			}
			for (std::vector<uint>::size_type j = 0; j != p_plights.size(); j++)
			{
				ps->SetData(
					"plight" + std::to_string(j),   // The name of the variable in the shader
					&m_plightList[j],               // The address of the data to copy
					sizeof(PointLight));            // The size of the data to copy
			}
			for (std::vector<uint>::size_type j = 0; j != p_slights.size(); j++)
			{
				ps->SetData(
					"slight" + std::to_string(j),   // The name of the variable in the shader
					&m_slightList[j],               // The address of the data to copy
					sizeof(SpotLight));             // The size of the data to copy
			}

			ps->SetData(
				"cameraPos",                    // The name of the variable in the shader
				&m_fpc->camera->GetPosition(),  // The address of the data to copy
				sizeof(DirectX::XMFLOAT3));     // The size of the data to copy




			ps->CopyAllBufferData();
			ps_list.push_back(ps); //add it to the list
		}
		context->IASetVertexBuffers(0, 1, m_meshList[p_indices[i]]->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(m_meshList[p_indices[i]]->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		//simple shader should handle our nullptrs if we happen to pass them
		m_materialList[p_indices[i]]->GetPixelShader()->SetSamplerState("sampleState", m_materialList[p_indices[i]]->GetSampleState());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("diffuseTexture", m_materialList[p_indices[i]]->GetSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("multiplyTexture", m_materialList[p_indices[i]]->GetMSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("specularTexture", m_materialList[p_indices[i]]->GetSpecSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("normalTexture", m_materialList[p_indices[i]]->GetNSRV());

		//vertex shader

		DirectX::XMFLOAT4X4 world;
		XMStoreFloat4x4(&world, XMMatrixTranspose(XMLoadFloat4x4(&m_worldDatas[p_indices[i]].GetWorld())));

		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("world", world);
		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("view", m_fpc->camera->GetView());
		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("projection", m_fpc->camera->GetProjection());
		// Set buffers in the input assembler
		
		
		m_materialList[p_indices[i]]->GetVertexShader()->CopyAllBufferData();
		ps->CopyAllBufferData();
		// Set our vertex and pixel shaders to use for the next draw
		m_materialList[p_indices[i]]->GetVertexShader()->SetShader();
		m_materialList[p_indices[i]]->GetPixelShader()->SetShader();


		


		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			m_meshList[p_indices[i]]->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
}


void Scene::RenderShadowMaps(ID3D11DeviceContext* context)
{
	for (int i = 0; i < m_dlightList.size(); i++) //do all our directional lights
	{
		// Set up targets
		context->OMSetRenderTargets(0, 0, m_dlightDSVList[i]);
		context->ClearDepthStencilView(m_dlightDSVList[i], D3D11_CLEAR_DEPTH, 1.0f, 0);
		context->RSSetState(m_shadowRasterizer);

		// Make a viewport to match the render target size
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_shadowMapSize;
		viewport.Height = (float)m_shadowMapSize;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewport);

		// Set up our shadow VS shader
		m_shadowVS->SetShader();
		m_shadowVS->SetMatrix4x4("view", m_dlightShadowViewList[i]);
		m_shadowVS->SetMatrix4x4("projection", m_dlightShadowProjectionList[i]);

		// Turn off pixel shader
		context->PSSetShader(0, 0, 0);

		// Loop through entities and draw them
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		for (int j = 0; j < m_meshList.size(); j++)
		{
			// Grab the data from the first object's mesh
			ID3D11Buffer* vb = *m_meshList[j]->GetVertexBuffer();
			ID3D11Buffer* ib = m_meshList[j]->GetIndexBuffer();

			// Set buffers in the input assembler
			context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
			context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

			m_shadowVS->SetMatrix4x4("world", m_worldDatas[j].GetWorld());
			m_shadowVS->CopyAllBufferData();

			// Finally do the actual drawing
			context->DrawIndexed(m_meshList[j]->GetIndexCount(), 0, 0);

		}
		// Change everything back

		context->OMSetRenderTargets(1, Game::getRTV(), Game::getDSV());
		viewport.Width = (float)Game::getWidth();
		viewport.Height = (float)Game::getHeight();
		context->RSSetViewports(1, &viewport);
		context->RSSetState(0);
	}
}

//object constructors and destructors
uint Scene::CreateObject(Mesh* p_mesh, Material* p_material)
{
	p_mesh->GetInstance();
	m_meshList.push_back(p_mesh);

	p_material->GetInstance();
	m_materialList.push_back(p_material);

	WorldData worlddata = WorldData();
	m_worldDatas.push_back(worlddata);

	return m_meshList.size() - 1;
}


//accessors
void Scene::SetObjectMaterial(uint p_index, Material* p_material)
{
	m_materialList[p_index]->RemoveInstance();
	m_materialList[p_index] = p_material;
	p_material->GetInstance();
}
void Scene::SetObjectMesh(uint p_index, Mesh* p_mesh)
{
	m_meshList[p_index]->RemoveInstance();
	m_meshList[p_index] = p_mesh;
	p_mesh->GetInstance();
}


void Scene::SetObjectCollider(uint p_index)
{
    Collider collider = Collider(m_meshList[p_index]->GetVertices(), m_worldDatas[p_index]);
    m_colliders.push_back(collider);
}

//Light functions, this could be rearchitectured at a later date to be
//cleaner
uint Scene::AddDirectionalLight(DirectionalLight p_dlight)
{
	m_dlightList.push_back(p_dlight);
	

	//shadow code
	// Create the actual texture that will be the shadow map
	D3D11_TEXTURE2D_DESC shadowDesc = {};
	shadowDesc.Width = m_shadowMapSize;
	shadowDesc.Height = m_shadowMapSize;
	shadowDesc.ArraySize = 1;
	shadowDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowDesc.CPUAccessFlags = 0;
	shadowDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowDesc.MipLevels = 1;
	shadowDesc.MiscFlags = 0;
	shadowDesc.SampleDesc.Count = 1;
	shadowDesc.SampleDesc.Quality = 0;
	shadowDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* shadowTexture;
	m_device->CreateTexture2D(&shadowDesc, 0, &shadowTexture);

	ID3D11DepthStencilView* newDSV;
	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &newDSV);

	m_dlightDSVList.push_back(newDSV);


	ID3D11ShaderResourceView* newSRV;
	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_device->CreateShaderResourceView(shadowTexture, &srvDesc, &newSRV);

	m_dlightSRVList.push_back(newSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();


	DirectX::XMFLOAT4X4 shadowViewMatrix, shadowProjectionMatrix;

	DirectX::XMVECTOR lightpos;
	lightpos = DirectX::XMLoadFloat4(
			&DirectX::XMFLOAT4(p_dlight.Direction.x,p_dlight.Direction.y,p_dlight.Direction.z, 0));
	//need code to calculate shadow view and projection matrices here
	DirectX::XMMATRIX shView = DirectX::XMMatrixLookAtLH(
		lightpos,
		DirectX::XMVectorSet(0, 0, 0, 0),
		DirectX::XMVectorSet(0, 1, 0, 0));
	DirectX::XMStoreFloat4x4(&shadowViewMatrix, XMMatrixTranspose(shView));

	m_dlightShadowViewList.push_back(shadowViewMatrix);
	// Orthographic to match the directional light
	DirectX::XMMATRIX shProj = DirectX::XMMatrixOrthographicLH(100, 100, 0.1f, 100.0f);
	XMStoreFloat4x4(&shadowProjectionMatrix, XMMatrixTranspose(shProj));
	m_dlightShadowProjectionList.push_back(shadowProjectionMatrix);
	return m_dlightList.size() - 1;

}
uint Scene::AddSpotLight(SpotLight p_slight)
{
	m_slightList.push_back(p_slight);
	return m_slightList.size() - 1;
}
uint Scene::AddPointLight(PointLight p_plight)
{
	m_plightList.push_back(p_plight);
	return m_plightList.size() - 1;
}


//object relative change functions
// TODO: update m_colliders[p_index] in these as well (or just give it world data. . .?)
void Scene::MoveObject(uint p_index, DirectX::XMFLOAT3 p_moveVector)
{
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_worldDatas[p_index].m_position);
	DirectX::XMVECTOR moveVector = DirectX::XMLoadFloat3(&p_moveVector);
	DirectX::XMStoreFloat3(&m_worldDatas[p_index].m_position,
						    DirectX::XMVectorAdd(position, moveVector));
}

void Scene::RotateObject(uint p_index, DirectX::XMFLOAT3 p_axis, float p_rad)
{
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&p_axis), p_rad);
	DirectX::XMVECTOR orientation = DirectX::XMLoadFloat3(&m_worldDatas[p_index].m_orientation);
	DirectX::XMStoreFloat3(&m_worldDatas[p_index].m_orientation, 
							DirectX::XMVectorAdd(rotation, orientation));

}

void Scene::Scale(uint p_index,float p_scale)
{
	m_worldDatas[p_index].m_scale += p_scale;
}