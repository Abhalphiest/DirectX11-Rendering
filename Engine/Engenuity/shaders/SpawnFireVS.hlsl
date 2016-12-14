struct VStoGS
{
    int type        : TEXCOORD0;
    float3 position	: POSITION;
    float size      : TEXCOORD1;
};

VStoGS main( VStoGS input )
{
    // Just ship it off to the SpawnFireGS
	return input;
}