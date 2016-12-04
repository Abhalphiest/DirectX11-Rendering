
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
    float3 binormal		: BINORMAL;
    float3 tangent		: TANGENT;
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
    float4 Direction_Angle; //w coordinate is angle of light emittance
    float3 Position;
};

cbuffer shaderData : register(b0)
{
    DirectionalLight dlight0;
    DirectionalLight dlight1;
    PointLight plight0;
    SpotLight slight0;
    SpotLight fpcLight;

    float3 cameraPos;
};

//texture variables
Texture2D diffuseTexture	: register(t0);
Texture2D multiplyTexture	: register(t1);
Texture2D specularTexture	: register(t2);
Texture2D normalTexture		: register(t3);
SamplerState sampleState	: register(s0);


//inline lighting functions

float4 directionallight(DirectionalLight dlight, float3 mapNormal, VertexToPixel input)
{
    float3 toLight = normalize(-dlight.Direction);
    float lightAmount = saturate(dot(toLight, mapNormal)); //already normalized
    return lightAmount*dlight.DiffuseColor + dlight.AmbientColor;

}

float4 pointlight(PointLight plight, float3 mapNormal, VertexToPixel input)
{
    float3 toLight = -normalize(input.worldpos - plight.Position);
    float dropoffRatio = length(input.worldpos - plight.Position);
    if (dropoffRatio < 1) dropoffRatio = 1;
    float lightAmount = 1 / dropoffRatio*saturate(dot(toLight, mapNormal));
    return lightAmount*plight.DiffuseColor + plight.AmbientColor;
}


float4 spotlight(SpotLight slight, float3 mapNormal, VertexToPixel input)
{
    float3 toLight = -normalize(input.worldpos - slight.Position);
    float NdotL = max(dot(input.normal, toLight), 0.0);
    float spotEffect = 0;
    if (NdotL > 0.0)
    {
        float slAngle = slight.Direction_Angle.w;
        spotEffect = dot(toLight, (float3)slight.Direction_Angle);
        if (spotEffect > cos(slAngle))
        {
            spotEffect = pow(spotEffect, 0.5);
            // Alt calculation - gave dim results, but much nicer fallof
            // spotEffect = 1 - pow(clamp(spotEffect / cos(slAngle), 0, 1), 0.5);
        }
        else
        {
            spotEffect = 0;
        }
    }
    return spotEffect * slight.DiffuseColor + slight.AmbientColor;
}

float spec_d(DirectionalLight light, float3 mapNormal, VertexToPixel input)
{
    float3 toLight = normalize(-light.Direction);
    float3 toCamera = normalize(cameraPos - input.worldpos);
    float3 refl = reflect(-toLight, mapNormal);
    return pow(max(dot(refl, toCamera), 0), 32);

}

float spec_p(PointLight light, float3 mapNormal, VertexToPixel input)
{
    //should we add dropoff to this, too?
    float3 toLight = -normalize(input.worldpos - light.Position);
    float3 toCamera = normalize(cameraPos - input.worldpos);
    float3 refl = reflect(-toLight, mapNormal);
    return pow(max(dot(refl, toCamera), 0), 32);

}
float spec_s(SpotLight light, float3 mapNormal, VertexToPixel input)
{
    //will probably need to be modified to account for light angle
    float3 toLight = -normalize(input.worldpos - light.Position);
    float3 toCamera = normalize(cameraPos - input.worldpos);
    float3 refl = reflect(-toLight, mapNormal);
    return pow(max(dot(refl, toCamera), 0), 32);

}

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

    //normalize our normals (heh)
    //normal mapping
    float3 tangentNormal = normalTexture.Sample(sampleState, input.uv).rgb; //lose the alpha channel
    tangentNormal = (tangentNormal*2.0f) - 1.0f; //take from (0,1) to (-1,1)
    float3 mapNormal = (tangentNormal.x*input.tangent) + (tangentNormal.y*input.binormal)
        + (tangentNormal.z*input.normal);
    mapNormal = normalize(mapNormal);
	

	
    //directional lights
    float4 lightCompute1 = directionallight(dlight0, mapNormal, input);
    float4 lightCompute2 = directionallight(dlight1, mapNormal, input);

    //point light and specular
    float4 lightCompute3 = pointlight(plight0, mapNormal, input);
    float specular = spec_p(plight0, mapNormal, input);

    float4 lightCompute4 = spotlight(slight0, mapNormal, input);

    float4 fpcLightCompute = spotlight(fpcLight, mapNormal, input);

    //textures
    float4 textureColor = diffuseTexture.Sample(sampleState, input.uv);
    float4 multColor = multiplyTexture.Sample(sampleState, input.uv);
    float specColor = specularTexture.Sample(sampleState, input.uv).r;  //all channels should be equal

	//return textureColor;
    return (lightCompute1 + lightCompute4 + fpcLightCompute) * textureColor;
}
