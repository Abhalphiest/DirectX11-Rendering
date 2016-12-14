struct VSInput
{
    // Same stuff in here as VStoGS in spawn shaders
    int type        : TEXCOORD0;
    float3 position	: POSITION;
    float size      : TEXCOORD1;
};

struct VStoGS
{
    int type        : TEXCOORD0;
    float3 position : POSITION;
    float size      : TEXCOORD1;
};

cbuffer externalData : register(b0)
{
    float4 color;
    float maxLifetime;
}

VStoGS main(VSInput input)
{
    VStoGS output;
    output.type = input.type;
    float t = input.age;
    output.position;    // = some intelligent movement function for fire

    // Change around color & size (probably just concerned about size for fire)
    //  based on an age percentage (like (t / maxLifetime) or somesuch)

	return output;
}