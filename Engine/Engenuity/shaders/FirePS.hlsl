struct GStoPS
{
    float4 position : SV_POSITION;
    float4 color	: COLOR;
    float2 uv		: TEXCOORD0;
};

// Need particle textures and sampler state

float4 main() : SV_TARGET
{
    //return sampling of particle texture
}