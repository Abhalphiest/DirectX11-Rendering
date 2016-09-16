#pragma once
#include "../Mesh.h"
#include <DirectXMath.h>
#include "Material.h"

class Entity
{
public:
	Entity(Mesh* p_mesh, Material* p_material);
	~Entity() { m_mesh->RemoveInstance(); m_material->RemoveInstance(); }

	//accessors
	DirectX::XMFLOAT3 GetPosition() { return m_position; }
	void SetPosition(DirectX::XMFLOAT3 p_position) { m_position = p_position;}

	DirectX::XMFLOAT3 GetRotation() { return m_rotation; }
	void SetRotation(DirectX::XMFLOAT3 p_rotation) { m_rotation = p_rotation; }

	DirectX::XMFLOAT3 GetScale() { return m_scale; }
	void SetScale(DirectX::XMFLOAT3 p_scale) { m_scale = p_scale; }

	DirectX::XMFLOAT4X4 GetWorld();

	//movement functions
	void Move(DirectX::XMFLOAT3 p_moveVector);
	void Rotate(DirectX::XMFLOAT3 p_axis, float p_rad);
	void Scale(DirectX::XMFLOAT3 p_scale);

	//draw functions
	void Draw(ID3D11DeviceContext* context);
private:
	Mesh* m_mesh;
	Material* m_material;
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT3 m_position, m_rotation, m_scale;
	
};