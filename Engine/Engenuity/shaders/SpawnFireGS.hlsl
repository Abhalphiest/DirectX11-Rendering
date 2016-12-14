#define ROOT 0
#define PARTICLE 1

struct VStoGS
{
    int type : TEXCOORD0;
    float3 position	    : POSITION;
    float4 color        : COLOR;
    float size : TEXCOORD1;
};

// Need some external data

// The random texture stuff is just for generating a random "seed", I guess

[maxvertexcount(2)]
void main(point VStoGS input[1], inout PointStream< VStoGS > output)
{
    input[0].age += dt;

    if (input[0].type == ROOT)
    {
        if (input[0].age >= ageToSpawn)
        {
            // Need to restart
            input[0].age = 0;

            VStoGS emit;
            emit.type = PARTICLE;
            emit.age = 0;
            emit.startPos = input[0].startPos;
            emit.startVel = input[0].startVel;
            emit.startColor = input[0].startColor;
            emit.midColor = input[0].midColor;
            emit.endColor = input[0].endColor;
            emit.sizes = input[0].sizes;

            // move/adjust velocities here, etc.

            output.Append(emit);
        }
        else if (input[0].age < maxLifetime)
        {
            output.Append(input[0]);
        }

        // All other cases, just don't add it to the output stream
    }
}