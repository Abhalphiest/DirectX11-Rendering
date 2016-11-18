// Texture Data
TextureCube cubemap		: register(t0);
SamplerState sstate	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return cubemap.Sample(sstate, input.uvw); //cube map is not lit or anything
}