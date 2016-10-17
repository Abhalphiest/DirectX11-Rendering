#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#define uint unsigned int

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
    DirectX::XMFLOAT3 m_position;   // Center
    float m_minY, m_minX, m_maxY, m_maxX, m_minZ, m_maxZ;

    Collider()
    {
        m_position = DirectX::XMFLOAT3(0, 0, 0);
    }

    // Constructor used only by FPC
    Collider(DirectX::XMFLOAT3 p_position)
    {
		m_position = p_position;

        // For testing purposes, give FPC basic cube collider
        m_minX = -1;
        m_maxX = 1;
        m_minY = -1;
        m_maxY = 1;
        m_minZ = -1;
        m_maxZ = 1;
    }

    Collider(std::vector<Vertex> p_vertices)
    {
        m_minX = 0;
        m_maxX = 0;
        m_minY = 0;
        m_maxY = 0;
        m_minZ = 0;
        m_maxZ = 0;

        Vertex currVert;
        for (std::vector<uint>::size_type i = 0; i != p_vertices.size(); ++i)
        {
            currVert = p_vertices[i];
            if (currVert.Position.x < m_minX)
                m_minX = currVert.Position.x;
            else if (currVert.Position.x > m_maxX)
                m_maxX = currVert.Position.x;

            if (currVert.Position.y < m_minY)
                m_minY = currVert.Position.y;
            else if (currVert.Position.y > m_maxY)
                m_maxY = currVert.Position.y;

            if (currVert.Position.z < m_minZ)
                m_minZ = currVert.Position.z;
            else if (currVert.Position.z > m_maxZ)
                m_maxZ = currVert.Position.z;
        }
    }

    bool IsColliding(Collider otherObj)
    {
        bool colliding = false;

		DirectX::XMFLOAT3 p_position = otherObj.m_position;
        // Basic 3D AABB, to start
        if (m_position.x + m_minX < p_position.x + otherObj.m_maxX
            && m_position.x + m_maxX > p_position.x + otherObj.m_minX
            && m_position.y + m_minY < p_position.y + otherObj.m_maxY
            && m_position.y + m_maxY > p_position.y + otherObj.m_minY
            && m_position.z + m_minZ < p_position.z + otherObj.m_maxZ
            && m_position.z + m_maxZ > p_position.z + otherObj.m_minZ)
        {
            colliding = true;
        }

        return colliding;
    }

};