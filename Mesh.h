#pragma once
#include<d3d11.h>
#include"Vertex.h"
#include <DirectXMath.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include <fstream>
#include "DXCore.h"

class Mesh
{

	ID3D11Buffer* m_Vbuffer, *m_Ibuffer;
	int m_numIndices;
	int m_references;
	~Mesh();
	
	
public:
	Mesh(Vertex* verts, int numVertices, unsigned int* indices, 
		int numIndices, ID3D11Device* device);
	static Mesh* LoadObj(char* filepath,ID3D11Device* p_device);

	ID3D11Buffer** GetVertexBuffer() { return &m_Vbuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_Ibuffer; }
	int GetIndexCount() { return m_numIndices; }

	void GetInstance() { m_references++; }
	void RemoveInstance() { m_references--; if (m_references == 0) delete this; }

	static Mesh* Cube(float p_size, ID3D11Device* p_device);
	static Mesh* Sphere(float p_radius, unsigned int p_subdivisions, ID3D11Device* p_device);
	static Mesh* Torus(float p_innerRad, float p_outerRad, unsigned int p_subdivisions, ID3D11Device* p_device);


	static void AddTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3,
		std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices, std::map<Vertex, int, VertexComparison> &p_indexMap);
	static void AddQuad(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT3 p4,
		std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices, std::map<Vertex, int, VertexComparison> &p_indexMap);
	static void CheckVertex(DirectX::XMFLOAT3 &p, DirectX::XMFLOAT3 &n, std::vector<Vertex> &p_vertices,
		std::vector<unsigned int> &p_indices, std::map<Vertex, int, VertexComparison> &p_indexMap);
	static DirectX::XMFLOAT3 TruncateVector(const DirectX::XMFLOAT3 v);


};