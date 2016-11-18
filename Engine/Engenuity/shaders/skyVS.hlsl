
// constant buffer for data
cbuffer externalData : register(b0)
{
	matrix view; //from the camera
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 binormal		: BINORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output
	VertexToPixel output;

	// Copy the view matrix and remove translation
	matrix skyview = view;
	skyview[0][3] = 0;
	skyview[1][3] = 0;
	skyview[2][3] = 0;

	// Calculate output position
	matrix viewProj = mul(skyview, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	// Ensure the vertex is at max depth
	//z will be divided by w in the pipeline, giving us 1 (max depth)
	output.position.z = output.position.w;

	// Use the raw vertex position as a direction
	// in space, unit cube
	output.uvw = input.position;

	return output;
}