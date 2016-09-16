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
	DirectX::XMFLOAT4 Color;        // The color of the vertex
};

struct VertexComparison
{
	bool operator()(const Vertex& lhs, const Vertex& rhs) const
	{
		return lhs.Position.x < rhs.Position.x ||
			lhs.Position.x == rhs.Position.x && (lhs.Position.y < rhs.Position.y || lhs.Position.y == rhs.Position.y && lhs.Position.z < rhs.Position.z);
	}
};