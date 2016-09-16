#include"Entity.h"

using namespace DirectX;
Entity::Entity(Mesh* p_mesh, Material* p_material)
{
	m_material = p_material;
	m_material->GetInstance();
	m_mesh = p_mesh;
	m_mesh->GetInstance(); //increment our instance
	m_position = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);
	m_world = XMFLOAT4X4();
	this->GetWorld(); //update world matrix
}

XMFLOAT4X4 Entity::GetWorld()
{
	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMStoreFloat4x4(&m_world, translation*rotation*scale);
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
void Entity::Scale(DirectX::XMFLOAT3 p_scale)
{
	m_scale.x += p_scale.x;
	m_scale.y += p_scale.y;
	m_scale.z += p_scale.z;
}

void Entity::Draw(ID3D11DeviceContext* context)
{
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