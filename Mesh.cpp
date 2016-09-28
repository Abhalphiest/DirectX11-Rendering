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

Mesh* Mesh::LoadObj(char* filepath, ID3D11Device* p_device)
{
	// File input object
	std::ifstream obj(filepath);

	// Check for successful open
	if (!obj.is_open())
		return nullptr;

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the y
				v4.UV.y = 1.0f - v4.UV.y;

				// Add a whole triangle
				verts.push_back(v1);
				verts.push_back(v3);
				verts.push_back(v4);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
		
	}

	// Close the file and create the actual buffers
	obj.close();
	CalcTBN(verts, indices); //for normal mapping

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the address of the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the address of the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	// - Yes, the indices are a bit redundant here (one per vertex)

	return new Mesh(&verts[0], vertCounter, &indices[0], vertCounter, p_device);
}
void Mesh::CalcTBN(std::vector<Vertex> &p_verts, std::vector<UINT> &indices)
{
	Vertex v0, v1, v2;
	DirectX::XMVECTOR dpos1, dpos2; //dpos is triangle edge, duv is uv edge
	DirectX::XMVECTOR p0, p1, p2;
	DirectX::XMFLOAT2 u0, u1, u2, duv1, duv2;
	DirectX::XMFLOAT3 tangent, binormal;
	std::vector<Vertex> vertices;
	for (int i = 0; i < indices.size(); i += 3) //assumes triangulated
	{
		//get our data
		v0 = p_verts[indices[i]];
		v1 = p_verts[indices[i + 1]];
		v2 = p_verts[indices[i + 2]];
		p0 = DirectX::XMLoadFloat3(&v0.Position);
		p1 = DirectX::XMLoadFloat3(&v1.Position);
		p2 = DirectX::XMLoadFloat3(&v2.Position);
		u0 = v0.UV;
		u1 = v1.UV;
		u2 = v2.UV;

		//get deltas
		dpos1 = DirectX::XMVectorSubtract(p1, p0);
		dpos2 = DirectX::XMVectorSubtract(p2, p0);
		DirectX::XMStoreFloat2(&duv1,DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&u1), DirectX::XMLoadFloat2(&u0)));
		DirectX::XMStoreFloat2(&duv2, DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&u2), DirectX::XMLoadFloat2(&u0)));

		float r = 1.0f / (duv1.x*duv2.y - duv1.y*duv2.x);
		DirectX::XMStoreFloat3(&tangent, DirectX::XMVectorScale(
								DirectX::XMVectorSubtract(DirectX::XMVectorScale(dpos1,duv2.y), 
								DirectX::XMVectorScale(dpos2, duv1.y))
								,r));
		DirectX::XMStoreFloat3(&binormal, DirectX::XMVectorScale(
			DirectX::XMVectorSubtract(DirectX::XMVectorScale(dpos2, duv1.x),
				DirectX::XMVectorScale(dpos1, duv2.x))
			, r));

		//set up our vertices and go
		v0.Binormal = binormal;
		v1.Binormal = binormal;
		v2.Binormal = binormal;
		v0.Tangent = tangent;
		v1.Tangent = tangent;
		v2.Tangent = tangent;
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}
	p_verts = vertices;
	IndexVertices(p_verts, indices); //we'll need to reindex since we differentiated vertices from
									 //each other.
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
	

	
	float len = p_size / 2.0f;
	DirectX::XMFLOAT3 p0 = DirectX::XMFLOAT3(-len, len, len);
	DirectX::XMFLOAT3 p1 = DirectX::XMFLOAT3(len, len, len);
	DirectX::XMFLOAT3 p2 = DirectX::XMFLOAT3(-len, -len, len);
	DirectX::XMFLOAT3 p3 = DirectX::XMFLOAT3(len, -len, len);
	DirectX::XMFLOAT3 p4 = DirectX::XMFLOAT3(-len, len, -len);
	DirectX::XMFLOAT3 p5 = DirectX::XMFLOAT3(len, len, -len);
	DirectX::XMFLOAT3 p6 = DirectX::XMFLOAT3(-len, -len, -len);
	DirectX::XMFLOAT3 p7 = DirectX::XMFLOAT3(len, -len, -len);
	Mesh::AddQuad(p0, p1, p3, p2, vertices, indices);
	Mesh::AddQuad(p1, p5, p7, p3, vertices, indices);
	Mesh::AddQuad(p4, p6, p7, p5, vertices, indices);
	Mesh::AddQuad(p0, p2, p6, p4, vertices, indices);
	Mesh::AddQuad(p0, p4, p5, p1, vertices, indices);
	Mesh::AddQuad(p2, p3, p7, p6, vertices, indices);
	
	for (unsigned int i = 0; i<indices.size(); ++i)
		std::cout << indices[i] << ' ';
	return new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), p_device);
}

Mesh* Mesh::Sphere(float p_radius, unsigned int p_subdivisions, ID3D11Device* p_device)
{
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	

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
				DirectX::XMFLOAT3(leftx*bottomrad, bottomy, leftz*bottomrad), vertices, indices);
		}

	}

	
	return new Mesh(&vertices[0], vertices.size(),&indices[0],indices.size(),p_device);
}


Mesh* Mesh::Torus(float p_innerRad, float p_outerRad, unsigned int p_subdivisions, ID3D11Device* p_device)
{
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	

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
				vertices, indices);
		}
	}
	return new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), p_device);
}

void Mesh::AddTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3,
	std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices)
{
	TruncateVector(p1); //eliminate rounding error creating duplicate vertices in our indexing
	TruncateVector(p2);
	TruncateVector(p3);
	//calculate normals
	DirectX::XMVECTOR vect1, vect2, t1, t2, t3;
	t1 = DirectX::XMLoadFloat3(&p1);
	t2 = DirectX::XMLoadFloat3(&p2);
	t3 = DirectX::XMLoadFloat3(&p3);
	vect1 = DirectX::XMVectorSubtract(t2, t1);
	vect2 = DirectX::XMVectorSubtract(t3, t1);
	DirectX::XMFLOAT3 normal;
	DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vect1, vect2)));
	TruncateVector(normal);

	//will now post-index for simplicity

	//will calculate tangents and binormals later in building

	//add the vertices
	Vertex v1 = { p1,normal,DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0) };
	Vertex v2 = { p2,normal,DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0) };
	Vertex v3 = { p3,normal,DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0) };
	
	
}

void Mesh::AddQuad(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT3 p4,
			 std::vector<Vertex> &p_verts, std::vector<unsigned int> & p_indices)
{
	AddTri(p1, p2, p3,p_verts, p_indices);
	AddTri(p1, p3, p4, p_verts,  p_indices);
}

void Mesh::IndexVertices(std::vector<Vertex> &p_vertices, std::vector<unsigned int> &p_indices)
{
	p_indices.clear(); //to be safe

	for (UINT i = 0; i < p_vertices.size(); i++)
	{
		p_indices.push_back(i); //laziness for now.. will actually implement later.
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
