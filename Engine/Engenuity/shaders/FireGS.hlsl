#define ROOT 0
#define PARTICLE 1

// Pass these in to simplify calculations later on
cbuffer externalData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VStoGS
{
    int type        : TEXCOORD0;
    float3 position	: POSITION;
    float size      : TEXCOORD1;
};

struct GStoPS
{
    float4 position : SV_POSITION;
    float4 color	: COLOR;
    float2 uv		: TEXCOORD0;
};

[maxvertexcount(4)]
void main(point VStoGS input[1], inout TriangleStream< GStoPS > output)
{
    // Tried to draw particle used for spawning, abort
    if (input[0].type == ROOT)
    {
        return;
    }

    GStoPS outstruct;

    // Offsets for smaller triangles
    // Assumes square quads, so size is just a float
    float offsetMag = input[0].size * 05.f;
    float2 offsets[4];
    offsets[0] = float2(+offsetMag, -offsetMag);
    offsets[1] = float2(+offsetMag, +offsetMag);
    offsets[2] = float2(-offsetMag, -offsetMag);
    offsets[3] = float2(-offsetMag, +offsetMag);

    float2 uvs[4];
    uvs[0] = float2(1, 1);
    uvs[1] = float2(1, 0);
    uvs[2] = float2(0, 1);
    uvs[3] = float2(0, 0);

    // Calculate world view proj matrix - simplifies positions later on
    matrix wvp = mul(mul(world, view), projection);

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        outstruct.position = mul(float4(input[0].position, 1.0f), wvp);

        // Will also probably need to take depth into account
        outstruct.position.xy += offsets[i] * input[0].size;
        output.uv = uvs[i];

        output.Append(outstruct);
    }
}