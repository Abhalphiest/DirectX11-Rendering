
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 worldpos		: WORLD_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: UV;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};
struct PointLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Position;
};

struct SpotLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 Direction_Angle; //w coordinate is angle of light
	float3 Position;
};

cbuffer shaderData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
	PointLight light3;
	float3 cameraPos;
};

//texture variables
Texture2D diffuseTexture	: register(t0);
Texture2D multiplyTexture	: register(t1);
Texture2D specularTexture	: register(t2);
Texture2D normalTexture		: register(t3);
SamplerState sampleState	: register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(input.uv,0, 1);

	//normalize our normal (heh)
	input.normal = normalize(input.normal);
	
	//directional lights
	float3 toLight = normalize(-light.Direction);
	float lightAmount = saturate(dot(toLight, input.normal)); //already normalized
	float4 lightCompute1 = lightAmount*light.DiffuseColor + light.AmbientColor;

	toLight = normalize(-light2.Direction);
	lightAmount = saturate(dot(toLight, input.normal)); //already normalized
	float4 lightCompute2 = lightAmount*light2.DiffuseColor + light2.AmbientColor;

	toLight = -normalize(input.worldpos - light3.Position);
	float dropoffRatio = length(input.worldpos - light3.Position);
	if (dropoffRatio < 1) dropoffRatio = 1;
	lightAmount = 1/dropoffRatio*saturate(dot(toLight, input.normal)); //already normalized
	float4 lightCompute3 = lightAmount*light3.DiffuseColor + light3.AmbientColor;

	//specular for the point light
	float3 toCamera = normalize(cameraPos - input.worldpos);
	float3 refl = reflect(-toLight, input.normal);
	float spec = pow(max(dot(refl, toCamera), 0), 32);
	//textures
	float4 textureColor = diffuseTexture.Sample(sampleState, input.uv);
	float4 multColor = multiplyTexture.Sample(sampleState, input.uv);
	float specColor = specularTexture.Sample(sampleState, input.uv).r; //all channels should be equal
	float4 nColor = normalTexture.Sample(sampleState, input.uv);
	return (lightCompute1
		+ lightCompute2
		+ lightCompute3)*textureColor*multColor + spec*specColor;
	//return float4(.52,.008,.008,1);
}