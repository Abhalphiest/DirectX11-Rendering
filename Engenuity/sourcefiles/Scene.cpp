#include"../headers/Scene.h"

Scene::Scene(FirstPersonController* p_fpc)
{
	m_fpc = p_fpc;
	m_meshList = std::vector<Mesh*>();
	m_materialList = std::vector<Material*>();
	m_colliders = std::vector<Collider>();
	m_worldDatas = std::vector<WorldData>();

	m_dlightList = std::vector<DirectionalLight>();
	m_slightList = std::vector<SpotLight>();
	m_plightList = std::vector<PointLight>();

}

Scene::~Scene()
{
	for (std::vector<Mesh*>::size_type i = 0; i != m_meshList.size(); i++) {
		m_meshList[i]->RemoveInstance();
		m_materialList[i]->RemoveInstance();
	}

}

void Scene::Render(ID3D11DeviceContext* context,
	std::vector<uint> p_indices,
	std::vector<uint> p_dlights,
	std::vector<uint> p_plights,
	std::vector<uint> p_slights)
{
	bool result;

	// We make a LOT of redundant calls here, but it will have to be fixed towards 
	// the end of the project.
	for (std::vector<uint>::size_type i = 0; i != p_indices.size(); i++)
	{

		for (std::vector<uint>::size_type j = 0; j != p_dlights.size(); j++)
		{
			result = m_materialList[p_indices[i]]->GetPixelShader()->SetData(
				"dlight" + std::to_string(j),   // The name of the variable in the shader
				&m_dlightList[j],               // The address of the data to copy
				sizeof(DirectionalLight));      // The size of the data to copy
		}
		for (std::vector<uint>::size_type j = 0; j != p_plights.size(); j++)
		{
			m_materialList[p_indices[i]]->GetPixelShader()->SetData(
				"plight" + std::to_string(j),   // The name of the variable in the shader
				&m_plightList[j],               // The address of the data to copy
				sizeof(PointLight));            // The size of the data to copy
		}
		for (std::vector<uint>::size_type j = 0; j != p_slights.size(); j++)
		{
			m_materialList[p_indices[i]]->GetPixelShader()->SetData(
				"slight" + std::to_string(j),   // The name of the variable in the shader
				&m_slightList[j],               // The address of the data to copy
				sizeof(SpotLight));             // The size of the data to copy
		}

		m_materialList[p_indices[i]]->GetPixelShader()->SetData(
			"cameraPos",                        // The name of the variable in the shader
			&m_fpc->camera->GetPosition(),      // The address of the data to copy
			sizeof(DirectX::XMFLOAT3));         // The size of the data to copy


		DirectX::XMFLOAT4X4 world;
		// Transpose for hlsl
		XMStoreFloat4x4(&world, XMMatrixTranspose(XMLoadFloat4x4(&m_worldDatas[p_indices[i]].GetWorld())));

		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("world", world);
		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("view", m_fpc->camera->GetView());
		m_materialList[p_indices[i]]->GetVertexShader()->SetMatrix4x4("projection", m_fpc->camera->GetProjection());

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		m_materialList[p_indices[i]]->GetVertexShader()->CopyAllBufferData();
		m_materialList[p_indices[i]]->GetPixelShader()->CopyAllBufferData();

		// Simple shader should handle our nullptrs if we happen to pass them
		m_materialList[p_indices[i]]->GetPixelShader()->SetSamplerState("sampleState", m_materialList[p_indices[i]]->GetSampleState());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("diffuseTexture", m_materialList[p_indices[i]]->GetSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("multiplyTexture", m_materialList[p_indices[i]]->GetMSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("specularTexture", m_materialList[p_indices[i]]->GetSpecSRV());
		m_materialList[p_indices[i]]->GetPixelShader()->SetShaderResourceView("normalTexture", m_materialList[p_indices[i]]->GetNSRV());

		// Set our vertex and pixel shaders to use for the next draw
		m_materialList[p_indices[i]]->GetVertexShader()->SetShader();
		m_materialList[p_indices[i]]->GetPixelShader()->SetShader();

		// Set buffers in the input assembler
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, m_meshList[p_indices[i]]->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(m_meshList[p_indices[i]]->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			m_meshList[p_indices[i]]->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
}


// Object constructors and destructors (where are the destructors?)
// Does this handle adding to reference counters?
uint Scene::CreateObject(Mesh* p_mesh, Material* p_material)
{
	p_mesh->GetInstance();
	m_meshList.push_back(p_mesh);

	p_material->GetInstance();
	m_materialList.push_back(p_material);


	Collider collider = Collider();     // Need to change later (why?)
	m_colliders.push_back(collider);

	WorldData worlddata = WorldData();
	m_worldDatas.push_back(worlddata);

	return m_meshList.size() - 1;
}


// Accessors
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

// Light functions, this could be rearchitectured at a later date to be cleaner
uint Scene::AddDirectionalLight(DirectionalLight p_dlight)
{
	m_dlightList.push_back(p_dlight);
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


// Object relative change functions
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

void Scene::Scale(uint p_index, float p_scale)
{
	m_worldDatas[p_index].m_scale += p_scale;
}