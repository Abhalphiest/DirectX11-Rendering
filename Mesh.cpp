#include"Mesh.h"

Mesh::Mesh(Vertex* p_verts, int p_numVertices, unsigned int* p_indices,
	int p_numIndices, ID3D11Device* p_device)
{
	m_numIndices = p_numIndices;
	//create the vertex buffer
	D3D11_BUFFER_DESC vertBufferData = {}; //initialize to 0 to save some assignments
	vertBufferData.Usage = D3D11_USAGE_IMMUTABLE; //we'll never change this
	vertBufferData.ByteWidth = sizeof(Vertex) * p_numVertices;  
	vertBufferData.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer

	//get our vertex data ready to pass into a buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = p_verts;

	//actually construct the vertex buffer
	p_device->CreateBuffer(&vertBufferData, &vertexData, &m_Vbuffer);



	//create the index buffer
	D3D11_BUFFER_DESC indexBufferData = {};
	indexBufferData.Usage = D3D11_USAGE_IMMUTABLE; //we'll never change this
	indexBufferData.ByteWidth = sizeof(int) * p_numIndices;
	indexBufferData.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer

	//get the data ready to be passed to the buffer
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = p_indices;

	//construct the index buffer in DX11
	p_device->CreateBuffer(&indexBufferData, &indexData, &m_Ibuffer);
}

Mesh::~Mesh()
{
	if (m_Vbuffer) { m_Vbuffer->Release(); }
	if (m_Ibuffer) { m_Ibuffer->Release(); }

}

Mesh* Mesh::Cube(float p_size, ID3D11Device* p_device)
{
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	std::map<Vertex, int, VertexComparison> indexMap = std::map<Vertex, int, VertexComparison>();

	
	float len = p_size / 2.0f;
	DirectX::XMFLOAT3 p0 = DirectX::XMFLOAT3(-len, len, len);
	DirectX::XMFLOAT3 p1 = DirectX::XMFLOAT3(len, len, len);
	DirectX::XMFLOAT3 p2 = DirectX::XMFLOAT3(-len, -len, len);
	DirectX::XMFLOAT3 p3 = DirectX::XMFLOAT3(len, -len, len);
	DirectX::XMFLOAT3 p4 = DirectX::XMFLOAT3(-len, len, -len);
	DirectX::XMFLOAT3 p5 = DirectX::XMFLOAT3(len, len, -len);
	DirectX::XMFLOAT3 p6 = DirectX::XMFLOAT3(-len, -len, -len);
	DirectX::XMFLOAT3 p7 = DirectX::XMFLOAT3(len, -len, -len);
	Mesh::AddQuad(p0, p1, p3, p2, vertices, indices, indexMap);
	Mesh::AddQuad(p1, p5, p7, p3, vertices, indices, indexMap);
	Mesh::AddQuad(p4, p6, p7, p5, vertices, indices, indexMap);
	Mesh::AddQuad(p0, p2, p6, p4, vertices, indices, indexMap);
	Mesh::AddQuad(p0, p4, p5, p1, vertices, indices, indexMap);
	Mesh::AddQuad(p2, p3, p7, p6, vertices, indices, indexMap);
	
	for (int i = 0; i<indices.size(); ++i)
		std::cout << indices[i] << ' ';
	return new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), p_device);
}

Mesh* Mesh::Sphere(float p_radius, unsigned int p_subdivisions, ID3D11Device* p_device)
{
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	std::map<Vertex, int, VertexComparison> indexMap = std::map<Vertex, int, VertexComparison>();

	if (p_subdivisions < 3)
		p_subdivisions = 3;
	else if (p_subdivisions > 360)
		p_subdivisions = 360;

	float approxStep = 360.0f / p_subdivisions;
	float radstep = 180.0f / p_subdivisions;
	float heightstep = 180.0f / p_subdivisions;
	DirectX::XMFLOAT3 baseCenter = DirectX::XMFLOAT3(0, -p_radius, 0);
	DirectX::XMFLOAT3 topCenter = DirectX::XMFLOAT3(0, p_radius, 0);
	float leftx;
	float leftz;
	float rightx;
	float rightz;
	float topy;
	float bottomy;
	float bottomrad;
	float toprad;

	//taking the easy O(n^2) time solution here
	for (unsigned int i = 0; i < p_subdivisions; i++)
	{


		for (unsigned int j = 0; j < p_subdivisions; j++)
		{
			leftx = (float)cos((M_PI / 180)*(j*approxStep));
			rightx = (float)cos((M_PI / 180)*((j + 1)*approxStep));
			leftz = (float)sin((M_PI / 180)*(j*approxStep));
			rightz = (float)sin((M_PI / 180)*((j + 1)*approxStep));
			topy = (float)(baseCenter.y + cos((M_PI / 180)*((i + 1)*radstep))*p_radius);
			bottomy = (float)(baseCenter.y + cos((M_PI / 180)*((i*radstep)))*p_radius);
			bottomrad = (float)sin((M_PI / 180)*(i*radstep))*p_radius;
			toprad = (float)sin((M_PI / 180)*((i + 1)*radstep))*p_radius;

			Mesh::AddQuad(DirectX::XMFLOAT3(leftx*toprad, topy, leftz*toprad),
				DirectX::XMFLOAT3(rightx*toprad, topy, rightz*toprad),
				DirectX::XMFLOAT3(rightx*bottomrad, bottomy, rightz*bottomrad),
				DirectX::XMFLOAT3(leftx*bottomrad, bottomy, leftz*bottomrad), vertices, indices, indexMap);
		}

	}

	
	return new Mesh(&vertices[0], vertices.size(),&indices[0],indices.size(),p_device);
}


Mesh* Mesh::Torus(float p_innerRad, float p_outerRad, unsigned int p_subdivisions, ID3D11Device* p_device)
{
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	std::map<Vertex, int, VertexComparison> indexMap = std::map<Vertex, int, VertexComparison>();

	if (p_subdivisions < 3)
		p_subdivisions = 3;
	else if (p_subdivisions > 360)
		p_subdivisions = 360;

	float approxStep = 360.0f / p_subdivisions;
	float radRing = (p_outerRad - p_innerRad)*.5f;
	float centRad = (p_outerRad + p_innerRad)*.5f;
	DirectX::XMFLOAT3 leftEnd;
	DirectX::XMFLOAT3 rightEnd;
	float lefty;
	float righty;
	float leftxtop;
	float rightxtop;
	float leftztop;
	float rightztop;
	float leftzbottom;
	float rightzbottom;
	float leftxbottom;
	float rightxbottom;


	//again, easy O(n^2) solution

	for (unsigned int i = 0; i < p_subdivisions; i++)
	{
		leftEnd = DirectX::XMFLOAT3((float)cos((M_PI / 180)*(i*approxStep)), 0, (float)sin((M_PI / 180)*(i*approxStep)));
		rightEnd = DirectX::XMFLOAT3((float)cos((M_PI / 180)*((i + 1)*approxStep)), 0, (float)sin((M_PI / 180)*((i + 1)*approxStep)));
		for (unsigned int j = 0; j < p_subdivisions; j++)
		{
			righty = (float)sin((M_PI / 180)*(j*approxStep))*radRing;
			lefty = (float)sin((M_PI / 180)*((j + 1)*approxStep))*radRing;
			rightxtop = (float)(cos((M_PI / 180)*(j*approxStep))*radRing + centRad)*rightEnd.x;
			leftxtop = (float)(cos((M_PI / 180)*((j + 1)*approxStep))*radRing + centRad)*rightEnd.x;
			rightztop = (float)(cos((M_PI / 180)*(j*approxStep))*radRing + centRad)*rightEnd.z;
			leftztop = (float)(cos((M_PI / 180)*((j + 1)*approxStep))*radRing + centRad)*rightEnd.z;
			rightxbottom = (float)(cos((M_PI / 180)*(j*approxStep))*radRing + centRad)*leftEnd.x;
			leftxbottom = (float)(cos((M_PI / 180)*((j + 1)*approxStep))*radRing + centRad)* leftEnd.x;
			rightzbottom = (float)(cos((M_PI / 180)*(j*approxStep))*radRing + centRad)*leftEnd.z;
			leftzbottom = (float)(cos((M_PI / 180)*((j + 1)*approxStep))*radRing + centRad)* leftEnd.z;

			Mesh::AddQuad(DirectX::XMFLOAT3(leftxtop, lefty, leftztop),
				DirectX::XMFLOAT3(leftxbottom, lefty, leftzbottom),
				DirectX::XMFLOAT3(rightxbottom, righty, rightzbottom),
				DirectX::XMFLOAT3(rightxtop, righty, rightztop),
				vertices, indices, indexMap);
		}
	}
	return new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), p_device);
}

void Mesh::AddTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3,
	std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices, 
	std::map<Vertex, int, VertexComparison> &p_indexMap)
{
	//see if these vertices have already been used, because we're doing indexed rendering
	CheckVertex(p1, p_verts, p_indices, p_indexMap);
	CheckVertex(p2, p_verts, p_indices, p_indexMap);
	CheckVertex(p3, p_verts, p_indices, p_indexMap);
	//should check for correct winding here, but I won't.

	//add the vertices
	Vertex v1 = { p1,DirectX::XMFLOAT4(p1.x,p1.y,p1.z,1.0f) };
	Vertex v2 = { p2,DirectX::XMFLOAT4(p2.x,p2.y,p2.z,1.0f) };
	Vertex v3 = { p3,DirectX::XMFLOAT4(p3.x,p3.y,p3.z,1.0f) };
	p_indices.push_back(p_indexMap[v1]);
	p_indices.push_back(p_indexMap[v3]);
	p_indices.push_back(p_indexMap[v2]);
	
}

void Mesh::AddQuad(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT3 p4,
			 std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices, std::map<Vertex, int, VertexComparison> &p_indexMap)
{
	AddTri(p1, p2, p3,p_verts, p_indices, p_indexMap);
	AddTri(p1, p3, p4, p_verts,  p_indices, p_indexMap);
}

void Mesh::CheckVertex(DirectX::XMFLOAT3 &p, std::vector<Vertex> &p_vertices, std::vector<unsigned int> &p_indices,
	std::map<Vertex, int, VertexComparison> &p_indexMap)
{
	p = TruncateVector(p); //get rid of redundant points due to rounding error
	Vertex v = { p, DirectX::XMFLOAT4(p.x, p.y, p.z,1.0f) };
	if (p_indexMap.find(v) == p_indexMap.end()) //not in there already
	{

		p_indexMap[v] = p_vertices.size();
		p_vertices.push_back(v);

	}
}

DirectX::XMFLOAT3 Mesh::TruncateVector(const DirectX::XMFLOAT3 v)
{
	DirectX::XMFLOAT3 returnV = DirectX::XMFLOAT3();
	returnV.x = std::trunc(100000 * v.x) / 100000;
	returnV.y = std::trunc(100000 * v.y) / 100000;
	returnV.z = std::trunc(100000 * v.z) / 100000;
	return returnV;
}
