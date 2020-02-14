//-----------------------------------------------------------------------------
// File: SkyBox.fx
//
// Desc: 
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 g_mInvWorldViewProjection;

texture g_EnvironmentTexture;

sampler EnvironmentSampler = sampler_state
{ 
    Texture = (g_EnvironmentTexture);
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};


//-----------------------------------------------------------------------------
// Skybox stuff
//-----------------------------------------------------------------------------
struct SkyboxVS_Input
{
    float4 Pos : POSITION;
};

struct SkyboxVS_Output
{
    float4 Pos : POSITION;
    float3 Tex : TEXCOORD0;
};

SkyboxVS_Output SkyboxVS( SkyboxVS_Input Input )
{
    SkyboxVS_Output Output;
    
    Output.Pos = Input.Pos;
    Output.Tex = normalize( mul(Input.Pos, g_mInvWorldViewProjection) );
    
    return Output;
}

float4 SkyboxPS( SkyboxVS_Output Input ) : COLOR
{
    float4 color = texCUBE( EnvironmentSampler, Input.Tex );
    return color;
}

[techniques]
<Textures>
	<Texture sampler="Base" file="/test.bmp" dimension="2" mipmap="false" SamplerState="linear" />
</Textures>
<Techniques>
<technique name="Skybox">
    <pass name="p0">
       <VertexShader profile="vs_2_0" name="SkyboxVS" />
       <FragmentShader  profile="ps_2_0" name="SkyboxPS" /> 
    </pass>
</technique>
</Techniques>

