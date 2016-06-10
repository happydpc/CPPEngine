/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

const char* shdTxt="[Vertex shader]"
"struct VsIn {\n"
"	float2 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"float4 scaleBias=float4(0.003,-0.003,-1,1);\n"
"VsOut main(VsIn In){\n"
"	VsOut Out=(VsOut)0;\n"
"	Out.position.xy = In.position.xy * scaleBias.xy + scaleBias.zw;\n"
"	Out.position.w= WSIGN 1;\n"
"	Out.texCoord = In.texCoord;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"sampler2D Base;\n"
"float4 colorRGBA=float4(1,1,1,1);\n"
"float4 main(VsOut IN): COLOR {\n"
"	return tex2D(Base, IN.texCoord)* colorRGBA;\n"
"}\n";

ShaderID shd=-1;
TextureID texture=-1;
VertexFormatID vf=-1;

int init(unsigned int width, unsigned int height)
{
	shd = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, "main", "main");
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);

	SamplerStateID ss=IRenderer::GetRendererInstance()->GetlinearClamp();

	texture=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/NSGUI/GUIElements.png"
		,false,ss);
	return 0;
}

void RenderTex2(D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 texsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f))
{
	D3DXFROMWINEVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	TexVertex dest[4];
	float x=aposition.x;
	float y=aposition.y;
	dest[0].position = D3DXFROMWINEVECTOR2(x + asize.x, y);
	dest[0].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y);
	dest[1].position = D3DXFROMWINEVECTOR2(x + asize.x, y + asize.y);
	dest[1].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y+texsize.y);
	dest[2].position = D3DXFROMWINEVECTOR2(x, y);
	dest[2].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y);
	dest[3].position = D3DXFROMWINEVECTOR2(x, y + asize.y);
	dest[3].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y+texsize.y);
	IRenderer::GetRendererInstance()->drawTextured(PRIM_TRIANGLE_STRIP, &dest[0], 2, texture, IRenderer::GetRendererInstance()->GetblendSrcAlpha(), IRenderer::GetRendererInstance()->GetnoDepthTest(), &col);
}

void RenderTex(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 texsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f))
{
	D3DXFROMWINEVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	TexVertex dest[4];
	float x=aposition.x;
	float y=aposition.y;
	dest[0].position = D3DXFROMWINEVECTOR2(x + asize.x, y);
	dest[0].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y);
	dest[1].position = D3DXFROMWINEVECTOR2(x + asize.x, y + asize.y);
	dest[1].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y+texsize.y);
	dest[2].position = D3DXFROMWINEVECTOR2(x, y);
	dest[2].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y);
	dest[3].position = D3DXFROMWINEVECTOR2(x, y + asize.y);
	dest[3].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y+texsize.y);
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setup2DMode(0, (float) IRenderer::GetRendererInstance()->GetViewportWidth(), 0, (float) IRenderer::GetRendererInstance()->GetViewportHeight());
	IRenderer::GetRendererInstance()->setShaderConstant4f("scaleBias", IRenderer::GetRendererInstance()->GetscaleBias2D());
	IRenderer::GetRendererInstance()->setShaderConstant4f("colorRGBA", col);
	IRenderer::GetRendererInstance()->setTexture("Base", id);
	IRenderer::GetRendererInstance()->setBlendState(IRenderer::GetRendererInstance()->GetblendSrcAlpha());
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &dest[0], &dest[0], sizeof(TexVertex));
}

int render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, false, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));

	IRenderer::GetRendererInstance()->BeginScene();

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,0.0f), D3DXFROMWINEVECTOR2(100.0f,100.0f));
	

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,100.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.0f,0.57f), D3DXFROMWINEVECTOR2(0.242f,1.1f));
	

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,150.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.26f,0.57f), D3DXFROMWINEVECTOR2(0.242f,1.1f));

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,200.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.53f,0.57f), D3DXFROMWINEVECTOR2(0.121f,0.55f));

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,250.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.66f,0.57f), D3DXFROMWINEVECTOR2(0.121f,0.55f));

	RenderTex(texture, D3DXFROMWINEVECTOR2(0.0f,300.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.79f,0.57f), D3DXFROMWINEVECTOR2(0.121f,0.55f));

	D3DXFROMWINEVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	float xPos = 100.0f;
	float yTop = 100.0f;
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setBlendState(IRenderer::GetRendererInstance()->GetblendSrcAlpha());
	// Radio button
	IRenderer::GetRendererInstance()->drawCircle(xPos + 17, yTop + 20, 7, col, 2);
	IRenderer::GetRendererInstance()->drawCircle(xPos + 17, yTop + 20, 4, col);
	yTop = 200.0f;
	// Checkbox
	IRenderer::GetRendererInstance()->drawRect(xPos + 8, yTop + 11, xPos + 24, yTop + 28, col, 2);
	// Check
	IRenderer::GetRendererInstance()->drawLine(xPos + 11, yTop + 18, xPos + 15, yTop + 25, col, 2);
	IRenderer::GetRendererInstance()->drawLine(xPos + 15, yTop + 25, xPos + 21, yTop + 14, col, 2);

	RenderTex2(D3DXFROMWINEVECTOR2(200.0f,200.0f), D3DXFROMWINEVECTOR2(25.0f,100.0f), D3DXFROMWINEVECTOR2(0.53f,0.57f), D3DXFROMWINEVECTOR2(0.121f,0.55f));

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
	return 0;
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "Texture");		
	IInput*    i=STX_Service::GetInputInstance();
	init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();	
		render();
	}
	return 0;
}
