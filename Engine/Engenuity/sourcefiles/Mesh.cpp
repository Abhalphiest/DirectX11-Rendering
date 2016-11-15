#include"../headers/Mesh.h"

Mesh::Mesh(Vertex* p_verts, int p_numVertices, unsigned int* p_indices,
	int p_numIndices, ID3D11Device* p_device)
{
	m_numIndices = p_numIndices;
	// Create the vertex buffer
	D3D11_BUFFER_DESC vertBufferData = {};          // Initialize to 0 to save some assignments
	vertBufferData.Usage = D3D11_USAGE_IMMUTABLE;   // We'll never change this
	vertBufferData.ByteWidth = sizeof(Vertex) * p_numVertices;
	vertBufferData.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer

														 // Get our vertex data ready to pass into a buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = p_verts;

	// Actually construct the vertex buffer
	p_device->CreateBuffer(&vertBufferData, &vertexData, &m_Vbuffer);


	// Create the index buffer
	D3D11_BUFFER_DESC indexBufferData = {};
	indexBufferData.Usage = D3D11_USAGE_IMMUTABLE;      // We'll never change this
	indexBufferData.ByteWidth = sizeof(int) * p_numIndices;
	indexBufferData.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer

														 // Get the data ready to be passed to the buffer
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = p_indices;

	// Construct the index buffer in DX11
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
	CalcTBN(verts, indices); // For normal mapping

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the address of the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the address of the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	// - Yes, the indices are a bit redundant here (one per vertex)

    Mesh* resultMesh = new Mesh(&verts[0], vertCounter, &indices[0], vertCounter, p_device);
    resultMesh->SetVertices(verts);

    return resultMesh;
}

Mesh* Mesh::LoadModel(char* filepath, ID3D11Device* p_device)
{
    // File input object
    std::ifstream obj(filepath);

    // Check for successful open
    if (!obj.is_open())
        return nullptr;

    // Variables used while reading the file
    std::vector<DirectX::XMFLOAT3> positions;   // Positions from the file
    std::vector<DirectX::XMFLOAT3> normals;     // Normals from the file
    std::vector<DirectX::XMFLOAT2> uvs;         // UVs from the file
    std::vector<DirectX::XMFLOAT3> binormals;   // Binormals from the file
    std::vector<DirectX::XMFLOAT3> tangents;    // Tangents from the file
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
        else if (chars[0] == 'v' && chars[1] == 'b')
        {
            // Read the 3 numbers directly into an XMFLOAT3
            DirectX::XMFLOAT3 binorm;
            sscanf_s(
                chars,
                "vb %f %f %f",
                &binorm.x, &binorm.y, &binorm.z);

            // Add to the list of normals
            binormals.push_back(binorm);
        }
        else if (chars[0] == 'v' && chars[1] == 'g')
        {
            // Read the 3 numbers directly into an XMFLOAT3
            DirectX::XMFLOAT3 tangent;
            sscanf_s(
                chars,
                "vg %f %f %f",
                &tangent.x, &tangent.y, &tangent.z);

            // Add to the list of normals
            tangents.push_back(tangent);
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
            unsigned int i[20];
            int facesRead = sscanf_s(
                chars,
                "f %d/%d/%d/%d/%d %d/%d/%d/%d/%d %d/%d/%d/%d/%d %d/%d/%d/%d/%d",
                &i[0], &i[1], &i[2], &i[3], &i[4],
                &i[5], &i[6], &i[7], &i[8], &i[9],
                &i[10], &i[11], &i[12], &i[13], &i[14],
                &i[15], &i[16], &i[17], &i[18], &i[19]);

            // - Create the verts by looking up
            //    corresponding data from vectors
            // - OBJ File indices are 1-based, so
            //    they need to be adusted
            Vertex v1;
            v1.Position = positions[i[0] - 1];
            v1.UV = uvs[i[1] - 1];
            v1.Normal = normals[i[2] - 1];
            v1.Binormal = binormals[i[3] - 1];
            v1.Tangent = tangents[i[4] - 1];

            Vertex v2;
            v2.Position = positions[i[5] - 1];
            v2.UV = uvs[i[6] - 1];
            v2.Normal = normals[i[7] - 1];
            v2.Binormal = binormals[i[8] - 1];
            v2.Tangent = tangents[i[9] - 1];

            Vertex v3;
            v3.Position = positions[i[10] - 1];
            v3.UV = uvs[i[11] - 1];
            v3.Normal = normals[i[12] - 1];
            v3.Binormal = binormals[i[13] - 1];
            v3.Tangent = tangents[i[14] - 1];

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
            if (facesRead == 20)
            {
                // Make the last vertex
                Vertex v4;
                v4.Position = positions[i[15] - 1];
                v4.UV = uvs[i[16] - 1];
                v4.Normal = normals[i[17] - 1];
                v4.Binormal = binormals[i[18] - 1];
                v4.Tangent = tangents[i[19] - 1];

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
    // CalcTBN(verts, indices); // For normal mapping   - Shouldn't be needed anymore

                             // - At this point, "verts" is a vector of Vertex structs, and can be used
                             //    directly to create a vertex buffer:  &verts[0] is the address of the first vert
                             //
                             // - The vector "indices" is similar. It's a vector of unsigned ints and
                             //    can be used directly for the index buffer: &indices[0] is the address of the first int
                             //
                             // - "vertCounter" is BOTH the number of vertices and the number of indices
                             // - Yes, the indices are a bit redundant here (one per vertex)

    Mesh* resultMesh = new Mesh(&verts[0], vertCounter, &indices[0], vertCounter, p_device);
    resultMesh->SetVertices(verts);

    return resultMesh;
}

void Mesh::CalcTBN(std::vector<Vertex> &p_verts, std::vector<UINT> &indices)
{
	Vertex v0, v1, v2;
	DirectX::XMVECTOR dpos1, dpos2;             // dpos is triangle edge, duv is uv edge
	DirectX::XMVECTOR p0, p1, p2;
	DirectX::XMFLOAT2 u0, u1, u2, duv1, duv2;
	DirectX::XMFLOAT3 tangent, binormal;
	std::vector<Vertex> vertices;
	for (int i = 0; i < indices.size(); i += 3) // Assumes triangulated
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
		DirectX::XMStoreFloat2(&duv1,
			DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat2(&u1),
				DirectX::XMLoadFloat2(&u0)));

		DirectX::XMStoreFloat2(&duv2,
			DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat2(&u2),
				DirectX::XMLoadFloat2(&u0)));

		float r = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x);
		DirectX::XMStoreFloat3(&binormal,
			DirectX::XMVectorScale(
				DirectX::XMVectorSubtract(
					DirectX::XMVectorScale(dpos1, duv2.y),
					DirectX::XMVectorScale(dpos2, duv1.y)),
				r));

		DirectX::XMStoreFloat3(&tangent,
			DirectX::XMVectorScale(
				DirectX::XMVectorSubtract(
					DirectX::XMVectorScale(dpos2, duv1.x),
					DirectX::XMVectorScale(dpos1, duv2.x)),
				r));

		// Set up our vertices and go
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
	IndexVertices(p_verts, indices);    // We'll need to reindex since we differentiated 
										// vertices from each other.
}
Mesh::~Mesh()
{
	if (m_Vbuffer) { m_Vbuffer->Release(); }
	if (m_Ibuffer) { m_Ibuffer->Release(); }

}

void Mesh::IndexVertices(std::vector<Vertex> &p_vertices,
	std::vector<unsigned int> &p_indices)
{
	p_indices.clear(); // To be safe
	std::vector<Vertex> processedverts;
	bool found;

	for (UINT i = 0; i < p_vertices.size(); i++)
	{
		found = false;
		p_vertices[i] = TruncateVertex(p_vertices[i]);
		for (uint j = 0; j < processedverts.size(); j++)
		{
			if (processedverts[j] == p_vertices[i]) //already seen this vertex
			{
				p_indices.push_back(j);
				found = true;
				break;
			}
			
		}
		if(!found)
			p_indices.push_back(i);
		processedverts.push_back(p_vertices[i]);
	}
}

Vertex Mesh::TruncateVertex(Vertex v)
{
	
	v.Position.x = std::trunc(100000 * v.Position.x) / 100000;
	v.Position.y = std::trunc(100000 * v.Position.y) / 100000;
	v.Position.z = std::trunc(100000 * v.Position.z) / 100000;

	v.Normal.x = std::trunc(100000 * v.Normal.x) / 100000;
	v.Normal.y = std::trunc(100000 * v.Normal.y) / 100000;
	v.Normal.z = std::trunc(100000 * v.Normal.z) / 100000;

	v.Binormal.x = std::trunc(100000 * v.Binormal.x) / 100000;
	v.Binormal.y = std::trunc(100000 * v.Binormal.y) / 100000;
	v.Binormal.z = std::trunc(100000 * v.Binormal.z) / 100000;

	v.Tangent.x = std::trunc(100000 * v.Tangent.x) / 100000;
	v.Tangent.y = std::trunc(100000 * v.Tangent.y) / 100000;
	v.Tangent.z = std::trunc(100000 * v.Tangent.z) / 100000;

	v.UV.x = std::trunc(100000 * v.UV.x) / 100000;
	v.UV.y = std::trunc(100000 * v.UV.y) / 100000;
	return v;
}

