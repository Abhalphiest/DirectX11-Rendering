#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "DataStructs.h"

using std::vector;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;

vector<vec3> positions;
vector<vec2> uvs;
vector<vec3> normals;
vector<vec3> binormals;
vector<vec3> tangents;
vector<newFace> resultFaces;
vector<oldFace> faces;

const char* outpath;

vec2 TruncateVec2(vec2 v)
{
    v.x = std::trunc(100000 * v.x) / 100000;
    v.y = std::trunc(100000 * v.y) / 100000;

    return v;
}

vec3 TruncateVec3(vec3 v)
{
    v.x = std::trunc(100000 * v.x) / 100000;
    v.y = std::trunc(100000 * v.y) / 100000;
    v.z = std::trunc(100000 * v.z) / 100000;

    return v;
}

void WriteModel()
{
    // File input object
    ofstream model(outpath);

    // Check for successful open
    if (!model.is_open())
        return;

    vec3 currPos;
    for (int i = 0; i < positions.size(); ++i)
    {
        currPos = TruncateVec3(positions[i]);
        model << "v " << currPos.x << " " << currPos.y << " " << currPos.z << "\n";
    }

    model << "\n";

    vec2 currUv;
    for (int i = 0; i < uvs.size(); ++i)
    {
        currUv = TruncateVec2(uvs[i]);
        model << "vt " << currUv.x << " " << currUv.y << "\n";
    }

    model << "\n";

    vec3 currNorm;
    for (int i = 0; i < normals.size(); ++i)
    {
        currNorm = TruncateVec3(normals[i]);
        model << "vn " << currNorm.x << " " << currNorm.y << " " << currNorm.z << "\n";
    }

    model << "\n";

    vec3 currBinorm;
    for (int i = 0; i < binormals.size(); ++i)
    {
        currBinorm = TruncateVec3(binormals[i]);
        // vb for Vertex Binormals
        model << "vb " << currBinorm.x << " " << currBinorm.y << " " << currBinorm.z << "\n";
    }

    model << "\n";

    vec3 currTangent;
    for (int i = 0; i < tangents.size(); ++i)
    {
        currTangent = TruncateVec3(tangents[i]);
        // vg for Vertex tanGents (vt was already taken, okay?)
        model << "vg " << currTangent.x << " " << currTangent.y << " " << currTangent.z << "\n";
    }

    model << "\n";

    newFace currFace;
    for (int i = 0; i < resultFaces.size(); ++i)
    {
        model << "f";
        currFace = resultFaces[i];
        // Loop through each "vertex"
        for (int j = 0; j < currFace.indices.size(); ++j)
        {
            model << " ";
            // Loop through each index in each "vertex" - there will always be 5
            for (int k = 0; k < 5; ++k)
            {
                model << currFace.indices[j][k];

                // Don't print the slash on the last index
                if (k != 4)
                {
                    model << "/";
                }
            }
        }
        model << "\n";
    }

    model.close();
}

void CalcTBN(vector<oldVertex> p_vertices, vector<unsigned int> p_indices)
{
    oldVertex v0, v1, v2;
    vec3 dpos1, dpos2;          // dpos is triangle edge, duv is uv edge
    vec3 p0, p1, p2;
    vec2 u0, u1, u2, duv1, duv2;
    vec3 tangent, binormal;
    
    unsigned int faceIndex = 0;
    for (int i = 0; i < p_indices.size(); i += 3) // Assumes triangulated
    {
        // Because everything started at 0, faceIndex *should* just sync up with i
        oldFace currentFace = faces[faceIndex];
        
        //get our data
        v0 = p_vertices[p_indices[i]];
        v1 = p_vertices[p_indices[i + 1]];
        v2 = p_vertices[p_indices[i + 2]];
        p0 = v0.pos;
        p1 = v1.pos;
        p2 = v2.pos;
        u0 = v0.uv;
        u1 = v1.uv;
        u2 = v2.uv;

        //get deltas
        dpos1 = p1 - p0;
        dpos2 = p2 - p0;

        duv1 = u1 - u0;
        duv2 = u2 - u0;

        float r = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x);
        binormal = ((dpos1 * duv2.y) - (dpos2 * duv1.y)) * r;

        tangent = ((dpos2 * duv1.x) - (dpos1 * duv2.x)) * r;

        // Indices should line up with faceIndex
        binormals.push_back(binormal);
        tangents.push_back(tangent);

        // Get position, uv, & normal indices from oldFace
        newFace nf;
        nf.indices.push_back({ currentFace.indices[0][0],
                                currentFace.indices[0][1],
                                currentFace.indices[0][2],
                                faceIndex,
                                faceIndex });

        nf.indices.push_back({ currentFace.indices[0][0],
                                currentFace.indices[1][1],
                                currentFace.indices[1][2],
                                faceIndex,
                                faceIndex });

        nf.indices.push_back({ currentFace.indices[2][0],
                                currentFace.indices[2][1],
                                currentFace.indices[2][2],
                                faceIndex,
                                faceIndex });

        resultFaces.push_back(nf);

        // Get next face
        faceIndex += 1;
    }

    WriteModel();
}

void LoadObj(char* filepath, char* outfilepath)
{
    outpath = outfilepath;
    // File input object
    std::ifstream obj(filepath);

    // Check for successful open
    if (!obj.is_open())
    {
        cout << "Bad file name" << endl;
        return;
    }

    // Variables used while reading the file
    vector<oldVertex> verts;       // Verts we're assembling
    vector<unsigned int> indices;  // Indices of these verts
    unsigned int vertCounter = 0;       // Count of vertices/indices
    char chars[100];                    // String for line reading

                                        // Still good?
    while (obj.good())
    {
        // Get the line (100 characters should be more than enough)
        obj.getline(chars, 100);

        // Check the type of line
        if (chars[0] == 'v' && chars[1] == 'n')
        {
            // Read the 3 numbers directly into an XMFLOAT3
            vec3 norm;
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
            vec2 uv;
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
            vec3 pos;
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
            oldVertex v1;
            v1.pos = positions[i[0] - 1];
            v1.uv = uvs[i[1] - 1];
            v1.normal = normals[i[2] - 1];

            oldVertex v2;
            v2.pos = positions[i[3] - 1];
            v2.uv = uvs[i[4] - 1];
            v2.normal = normals[i[5] - 1];

            oldVertex v3;
            v3.pos = positions[i[6] - 1];
            v3.uv = uvs[i[7] - 1];
            v3.normal = normals[i[8] - 1];

            // Flip the UV's since they're probably "upside down"
            v1.uv.y = 1.0f - v1.uv.y;
            v2.uv.y = 1.0f - v2.uv.y;
            v3.uv.y = 1.0f - v3.uv.y;

            // Add the verts to the vector
            verts.push_back(v1);
            verts.push_back(v2);
            verts.push_back(v3);

            // Add three more indices
            indices.push_back(vertCounter); vertCounter += 1;
            indices.push_back(vertCounter); vertCounter += 1;
            indices.push_back(vertCounter); vertCounter += 1;
            
            // Add the faces - will need these later when we store them back in
            vector<vector<unsigned int>> f;
            f.push_back({ i[0], i[1], i[2] });
            f.push_back({ i[3], i[4], i[5] });
            f.push_back({ i[6], i[7], i[8] });

            // Was there a 4th face?
            if (facesRead == 12)
            {
                // Make the last vertex
                oldVertex v4;
                v4.pos = positions[i[9] - 1];
                v4.uv = uvs[i[10] - 1];
                v4.normal = normals[i[11] - 1];

                // Flip the y
                v4.uv.y = 1.0f - v4.uv.y;

                // Add a whole triangle
                verts.push_back(v1);
                verts.push_back(v3);
                verts.push_back(v4);

                // Add three more indices
                indices.push_back(vertCounter); vertCounter += 1;
                indices.push_back(vertCounter); vertCounter += 1;
                indices.push_back(vertCounter); vertCounter += 1;

                f.push_back({ i[9], i[10], i[11] });
            }

            faces.push_back({ f });
        }

    }

    // Close the file and create the actual buffers
    obj.close();
    CalcTBN(verts, indices); // For normal mapping
}

int main()
{
    char inpath[256];
    char outpath[256];
    cout << "Please enter the path of the .obj file you would like to load (including the .obj extension):" << endl;
    cin >> inpath;

    cout << endl << "Please enter the path of the .model file you would like to write to (be sure to inlcude the .model extension):" << endl;
    cin >> outpath;

    LoadObj(inpath, outpath);

    cout << endl << "Press any key to exit:" << endl;
    cin.ignore();

    return 0;
}