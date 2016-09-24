#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;        // The normal of the vertex
	DirectX::XMFLOAT2 UV;        // The UV coordinate of the vertex

	bool operator==(const Vertex &other) const
	{
		//extremely messy, might come back and do it all right and fast sometime.
		return (Position.x == other.Position.x && Position.y == other.Position.y && Position.z == other.Position.z
			&& Normal.x == other.Normal.x && Normal.y == other.Normal.y && Normal.z == other.Normal.z
			&& UV.x == other.UV.x && UV.y == other.UV.y);
	}
	bool operator<(const Vertex& rhs) const
	{
		//extremely messy, might come back and do it all right and fast sometime.
		if (Position.x == rhs.Position.x && Position.y == rhs.Position.y && Position.z == rhs.Position.z)
		{
			return (Normal.x < rhs.Normal.x ||
				Normal.x == rhs.Normal.x && ((Normal.y < rhs.Normal.y)
					|| (Normal.y == rhs.Normal.y && Normal.z < rhs.Normal.z)));
		}
		else
			return (Position.x < rhs.Position.x ||
				Position.x == rhs.Position.x && (Position.y < rhs.Position.y
					|| Position.y == rhs.Position.y && Position.z < rhs.Position.z));
			
	}
};

