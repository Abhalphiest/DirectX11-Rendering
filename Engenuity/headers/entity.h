#pragma once
#include "../headers/Mesh.h"
#include <DirectXMath.h>
#include "../headers/Material.h"

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

	float GetScale() { return m_scale; }
	void SetScale(float p_scale) { m_scale = p_scale; }

	DirectX::XMFLOAT4X4 GetWorld();

	//movement functions
	void Move(DirectX::XMFLOAT3 p_moveVector);
	void Rotate(DirectX::XMFLOAT3 p_axis, float p_rad);
	void Scale(float p_scale);

	//draw functions
	void Draw(ID3D11DeviceContext* context, DirectX::XMFLOAT4X4 p_view, DirectX::XMFLOAT4X4 p_proj);
private:
	Mesh* m_mesh;
	Material* m_material;
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT3 m_position, m_rotation;
	float m_scale; //no longer allowing non-uniform scaling, to simplify shader code
	
};