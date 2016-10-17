#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position; // The position of the vertex
	DirectX::XMFLOAT3 Normal;   // The normal of the vertex
	DirectX::XMFLOAT3 Binormal; // For normal mapping
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT2 UV;       // The UV coordinate of the vertex


	bool operator==(const Vertex& other) // copy assignment
	{
		//good opportunity for SIMD optimization later
		bool position, normal, binormal, tangent, uv;

		position = Position.x == other.Position.x && Position.y == other.Position.y
			&& Position.z == other.Position.z;
		
		normal = Normal.x == other.Normal.x && Normal.y == other.Normal.y
			&& Normal.z == other.Normal.z;

		binormal = Binormal.x == other.Binormal.x && Binormal.y == other.Binormal.y
			&& Binormal.z == other.Binormal.z;

		tangent = Tangent.x == other.Tangent.x && Tangent.y == other.Tangent.y
			&& Tangent.z == other.Tangent.z;

		uv = UV.x == other.UV.x && UV.y == other.UV.y;
		return position && normal && binormal && tangent && uv;
	}
};

