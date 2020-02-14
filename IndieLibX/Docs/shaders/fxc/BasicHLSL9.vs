
struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float4 Diffuse    : COLOR0;
    float2 TextureUV  : TEXCOORD0;
};
float4x4 g_mWorldViewProjection;

VS_OUTPUT RenderSceneVS( float4 vPos : POSITION,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform bool bTexture)
{
    VS_OUTPUT Output;
    Output.Position = mul(vPos, g_mWorldViewProjection);

    if( bTexture ) 
        Output.TextureUV = vTexCoord0; 
    else
        Output.TextureUV = 0; 

	Output.Diffuse=float4(1,0,0,0);
    
    return Output;    
}
