// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view; //from perspective of light
	matrix projection;
};

// only need position for shadows
struct VertexShaderInput
{
	float3 position		: POSITION;
};

// Only thing we need to output for shadow map creation
struct VertexToPixel
{
	float4 position		: SV_POSITION;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world,view),projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	return output;
}