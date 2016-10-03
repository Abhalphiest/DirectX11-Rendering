#pragma once
#include<DirectXMath.h>
struct WorldData
{
	
	DirectX::XMFLOAT3 m_position, m_orientation;
	float m_scale; //no longer allowing non-uniform scaling, to simplify shader code
	DirectX::XMFLOAT3 m_velocity, m_acceleration;
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT4X4 GetWorld() //should be called when you want the updated world matrix
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_orientation.x, m_orientation.y, m_orientation.z);
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);
		XMStoreFloat4x4(&m_world, scale*rotation*translation);
		return m_world;
	}
	WorldData()
	{
		m_position = DirectX::XMFLOAT3(0, 0, 0);
		m_orientation = DirectX::XMFLOAT3(0, 0, 0);
		m_velocity = DirectX::XMFLOAT3(0, 0, 0);
		m_acceleration = DirectX::XMFLOAT3(0, 0, 0);
		m_scale = 1.0f;
	}
};

struct Collider
{

};