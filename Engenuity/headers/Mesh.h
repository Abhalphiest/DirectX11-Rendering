#pragma once
#include<d3d11.h>
#include"Vertex.h"
#include <DirectXMath.h>
#include <vector>
#define _USE_MATH_DEFINES //for pi
#include <math.h>
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
	static void CalcTBN(std::vector<Vertex> &p_verts, std::vector<UINT> &indices);

	ID3D11Buffer** GetVertexBuffer() { return &m_Vbuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_Ibuffer; }
	int GetIndexCount() { return m_numIndices; }

	void GetInstance() { m_references++; }
	void RemoveInstance() { m_references--; if (m_references == 0) delete this; }

private:
	static void IndexVertices(std::vector<Vertex> &p_vertices, std::vector<unsigned int> &p_indices);
	DirectX::XMFLOAT3 Mesh::TruncateVector(const DirectX::XMFLOAT3 v);


};