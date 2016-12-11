#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include <string>
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

        // For testing purposes, give FPC basic cube(-ish) collider
        m_minX = -0.1;
        m_maxX = 0.1;
        m_minY = -0.5;
        m_maxY = 0.1;
        m_minZ = -0.1;
        m_maxZ = 0.1;
    }

    Collider(std::vector<Vertex> p_vertices, WorldData p_worlddata)
    {
        m_position = p_worlddata.m_position;

        m_minX = 0;
        m_maxX = 0;
        m_minY = 0;
        m_maxY = 0;
        m_minZ = 0;
        m_maxZ = 0;

        DirectX::XMFLOAT3 currPos;
        DirectX::XMVECTOR transformedPos;
        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(p_worlddata.m_orientation.x, p_worlddata.m_orientation.y, p_worlddata.m_orientation.z);
        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(p_worlddata.m_scale, p_worlddata.m_scale, p_worlddata.m_scale);
        DirectX::XMMATRIX transformer = scale * rotation;

        for (std::vector<uint>::size_type i = 0; i != p_vertices.size(); ++i)
        {
            currPos = p_vertices[i].Position;
            transformedPos = DirectX::XMLoadFloat3(&currPos);
            transformedPos = DirectX::XMVector3Transform(transformedPos, transformer);
            DirectX::XMStoreFloat3(&currPos, transformedPos);

            if (currPos.x < m_minX)
                m_minX = currPos.x;
            else if (currPos.x > m_maxX)
                m_maxX = currPos.x;

            if (currPos.y < m_minY)
                m_minY = currPos.y;
            else if (currPos.y > m_maxY)
                m_maxY = currPos.y;

            if (currPos.z < m_minZ)
                m_minZ = currPos.z;
            else if (currPos.z > m_maxZ)
                m_maxZ = currPos.z;
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

struct Door {
    uint index;
    std::string destScene;
    uint destIndex;

    Door(uint i, std::string ds, uint di)
    {
        index = i;
        destScene = ds;
        destIndex = di;
    }

    uint getIndex()
    {
        return index;
    }

    std::string getDestScene()
    {
        return destScene;
    }

    uint getDestIndex()
    {
        return destIndex;
    }
};