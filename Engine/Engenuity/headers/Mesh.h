#pragma once
#include<d3d11.h>
#include"Vertex.h"
#include <DirectXMath.h>
#include <vector>
#define _USE_MATH_DEFINES //for pi
#include <math.h>
#include <fstream>
#include "DXCore.h"
#include<algorithm>

class Mesh
{
	ID3D11Buffer* m_Vbuffer, *m_Ibuffer;
    std::vector<Vertex> m_vertices;
    int m_numIndices;
	int m_references;
	~Mesh();
	
public:
	Mesh(Vertex* verts, int numVertices, unsigned int* indices, 
		    int numIndices, ID3D11Device* device);

    /**
    * LoadObj takes the file name of a mesh, reads the file in, parses the
    *  file, and creates the appropriate mesh and returns a pointer to it.
    *
    * @param filepath - char* holding the name & path of the mesh file
    * @param p_device - ID3D11Device* to create the Mesh instance with
    * @return - Mesh* to newly created Mesh instance
    */
	static Mesh* LoadObj(char* filepath,ID3D11Device* p_device);

    static Mesh* LoadModel(char* filepath, ID3D11Device* p_device);

    /**
    * CalcTBN takes the provided vertex and index vector lists and calculates
    *  the mesh's Tangent Binormal matrix with them, then calls IndexVertices
    *  to re-index verts as needed.
    *
    * @param p_verts - vector<Vertex> of. . .??? This mesh's specific vertices?
    * @param indices - vector<UNIT> of. . .??? This mesh's specific indices?
    */
	static void CalcTBN(std::vector<Vertex> &p_verts, std::vector<UINT> &indices);

    void SetVertices(std::vector<Vertex> p_vertices) { m_vertices = p_vertices; }

	ID3D11Buffer** GetVertexBuffer() { return &m_Vbuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_Ibuffer; }
    std::vector<Vertex> GetVertices() { return m_vertices; }
	int GetIndexCount() { return m_numIndices; }

	void GetInstance() { m_references++; }
	void RemoveInstance() { m_references--; if (m_references == 0) delete this; }

private:
    /*
    * IndexVertices is a helper function for re-indexing vertices after
    *  the mesh's TBN has been recalculated.
    *
    * @param p_vertices - vertices to be re-indexed
    * @param p_indices  - indices of the verts
    */
	static void IndexVertices(std::vector<Vertex> &p_vertices, 
                                std::vector<unsigned int> &p_indices);

    /*
    * TruncateVector is a helper function that takes in a XMFLOAT3 and
    *  truncates each of its components to 5 digits of precision.
    *
    * @param v - vector to be truncated
    * @return - XMFLOAT3 truncated to 5 digits of precision
    */
	static Vertex Mesh::TruncateVertex(Vertex v);

};