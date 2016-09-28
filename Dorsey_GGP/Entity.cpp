#include"Entity.h"

using namespace DirectX;
Entity::Entity(Mesh* p_mesh, Material* p_material)
{
	m_material = p_material;
	m_material->GetInstance();
	m_mesh = p_mesh;
	m_mesh->GetInstance(); //increment our instance
	m_position = XMFLOAT3(0, 0, 0);
	m_scale = 1.0;
	m_rotation = XMFLOAT3(0, 0, 0);
	m_world = XMFLOAT4X4();
	this->GetWorld(); //update world matrix
}

XMFLOAT4X4 Entity::GetWorld()
{
	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX scale = XMMatrixScaling(m_scale, m_scale, m_scale);
	XMStoreFloat4x4(&m_world, scale*rotation*translation);
	return m_world;
}

void Entity::Move(XMFLOAT3 p_moveVector)
{
	XMVECTOR position = DirectX::XMLoadFloat3(&m_position);
	XMVECTOR moveVector = DirectX::XMLoadFloat3(&p_moveVector);
	XMStoreFloat3(&m_position, position + moveVector);
}

void Entity::Rotate(DirectX::XMFLOAT3 p_axis, float p_rad)
{
	XMVECTOR rotation = XMQuaternionRotationAxis(XMLoadFloat3(&p_axis), p_rad);
	XMVECTOR orientation = XMLoadFloat3(&m_rotation);
	XMStoreFloat3(&m_rotation, rotation+orientation);
	
}
void Entity::Scale(float p_scale)
{
	m_scale += p_scale;
}

void Entity::Draw(ID3D11DeviceContext* context, DirectX::XMFLOAT4X4 p_view, DirectX::XMFLOAT4X4 p_proj )
{
	

	DirectX::XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMLoadFloat4x4(&GetWorld()))); // transpose for hlsl
	m_material->GetVertexShader()->SetMatrix4x4("world", world);
	m_material->GetVertexShader()->SetMatrix4x4("view", p_view);
	m_material->GetVertexShader()->SetMatrix4x4("projection", p_proj);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	m_material->GetVertexShader()->CopyAllBufferData();
	m_material->GetPixelShader()->CopyAllBufferData();

	//simple shader should handle our nullptrs if we happen to pass them
	m_material->GetPixelShader()->SetSamplerState("sampleState",m_material->GetSampleState());
	m_material->GetPixelShader()->SetShaderResourceView("diffuseTexture", m_material->GetSRV());
	m_material->GetPixelShader()->SetShaderResourceView("multiplyTexture", m_material->GetMSRV());
	m_material->GetPixelShader()->SetShaderResourceView("specularTexture", m_material->GetSpecSRV());
	m_material->GetPixelShader()->SetShaderResourceView("normalTexture", m_material->GetNSRV());
	// Set our vertex and pixel shaders to use for the next draw
	m_material->GetVertexShader()->SetShader();
	m_material->GetPixelShader()->SetShader();


	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_mesh->GetVertexBuffer(), &stride, &offset);
	context->IASetIndexBuffer(m_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		m_mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}