/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/IRenderer.h>

// Copyright (c) 2011 NVIDIA Corporation. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, NONINFRINGEMENT,IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA 
// OR ITS SUPPLIERS BE  LIABLE  FOR  ANY  DIRECT, SPECIAL,  INCIDENTAL,  INDIRECT,  OR  
// CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS 
// OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY 
// OTHER PECUNIARY LOSS) ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, 
// EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
// Please direct any bugs or questions to SDKFeedback@nvidia.com
s
extern int loadTextureFromFile(std::string file, std::string shaderTextureName, stx_Effect* pEffect);

namespace {
stx_Effect* g_pStarEffect = 0;
VertexBufferID g_pStarDataVB = 0;
VertexBufferID g_pStarCornersVB = 0;
VertexFormatID g_pInputLayout = 0;

stx_Effect::stx_Technique* g_pStarDataTechnique = 0;
stx_Effect::stx_Variable* g_pWorldViewProjVar = 0;
stx_Effect::stx_Variable* g_pHeightScaleVar = 0;
stx_Effect::stx_Variable* g_pSizeScaleVar = 0;
stx_Effect::stx_Variable* g_pAspectVar = 0;
stx_Effect::stx_Variable* g_pDistortionVar = 0;
stx_Effect::stx_Variable* g_pMinSizeVar = 0;

struct StarData
{
	StarData(): gLon(0), gLat(0), magnitude(0), BminusV(0) {}
	StarData(const char* line);

	float gLon, gLat;
	float magnitude;
	float BminusV;
};

struct StarCorner
{
	float x,y;
};

const int N_STARS = 9110;	// Fixed number of lines in the catalog.
StarData g_stars[N_STARS];

float StrToFloat(const char* pS, int start, int end)
{
	const char* pFirst = pS+start-1;
	if (*pFirst == '+')
	{
		++start;
		++pFirst;
	}

	std::string str(pFirst, end-start+1);
	std::istringstream istr(str);
	float f;
	istr >> f;

	// A few lines in the catalog are blank.  Deal with that gracefully.
	return (istr.fail())? 0: f;
}

StarData::StarData(const char* line): gLon(0), gLat(0), magnitude(0), BminusV(0)
{
	// Fields in the catalog don't necessarily have delimiting spaces.  We need
	// to pluck out hardcoded character ranges.  From the readme, we want these:
	// 91- 96  F6.2   deg     GLON     ?Galactic longitude (1)
	// 97-102  F6.2   deg     GLAT     ?Galactic latitude (1)
	//103-107  F5.2   mag     Vmag     ?Visual magnitude (1)
	//    108  A1     ---   n_Vmag    *[ HR] Visual magnitude code
	//    109  A1     ---   u_Vmag     [ :?] Uncertainty flag on V
	//110-114  F5.2   mag     B-V      ? B-V color in the UBV system
	gLon      = (StrToFloat(line,  91, 96) / 180.0f) * D3DXFROMWINE_PI;
	gLat      = (StrToFloat(line,  97,102) / 180.0f) * D3DXFROMWINE_PI;
	magnitude = StrToFloat(line, 103,107);
	BminusV   = StrToFloat(line, 110,114);
}

void ReadBrightStarCatalog(const char* fName)
{
	int hr = 0;
    char str[MAX_PATH];
    (DXUTFindDXSDKMediaFileCch(str, MAX_PATH, fName));

	// FYI:
	//  424 = Polaris (should be approximately up, if our alginment were correct).
	// 2061 = Betelgeuse
	// 2491 = Sirius (brightest)
	int count = 0;
	std::ifstream istr(str);
	while (istr.good() && count <= N_STARS)
	{
		const int LINE_LEN = 198;	// from definition of data file
		char line[LINE_LEN];
		istr.getline(line, LINE_LEN);
		g_stars[count++] = StarData(line);
	}
	assert(count-1 == N_STARS);

	// Normalize all B-V values to [0,1].
	float minBmV = -0.3f;
	float maxBmV =  2.0f;
	const float divisor = 1.0f / (maxBmV - minBmV);
	for (int i=0; i!=N_STARS; ++i)
		g_stars[i].BminusV = (g_stars[i].BminusV - minBmV) * divisor;
}

void CreateStarDataVB()
{
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
    initData.pSysMem = g_stars;

    D3D11_BUFFER_DESC vbDesc =
    {
        sizeof(StarData) * N_STARS,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };

    int hr;
    (->CreateBuffer(&vbDesc, &initData, &g_pStarDataVB));
}

void CreateStarCornersVB()
{
	// Simple per-vertex data for instancing.  Defines a 2D quad.
	StarCorner corners[4] =
	{
		 1,-1,
		-1,-1,
		 1, 1,
	    -1, 1,
	};

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
    initData.pSysMem = corners;

    D3D11_BUFFER_DESC vbDesc =
    {
        sizeof(StarCorner) * 4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };

    int hr;
    (->CreateBuffer(&vbDesc, &initData, &g_pStarCornersVB));
}

int CreateStarEffect()
{
    int hr;

	// Load effect from file
    char path[MAX_PATH];
    ID3DXBuffer* pShader = 0;
    if ( (hr = DXUTFindDXSDKMediaFileCch(path, MAX_PATH, "Stars.fx"))) {
        LOG_PRINT (0, "Stars.fx cannot be found.", "Error", 0);
        return hr;
    }

    ID3DXBuffer* pErrors = 0;
    if (D3DXFROMWINECompileShaderFromFile(path, 0, 0, 0, "fx_5_0", 0, &pShader, &pErrors, 0) != 0)
    {
        MessageBoxA(0, (char *)pErrors->GetBufferPointer(), "Compilation errors", 0);
        return hr;
    }

    if (D3DXFROMWINE11CreateEffectFromMemory(pShader->GetBufferPointer(), pShader->GetBufferSize(), 0, &g_pStarEffect) != 0)
    {
        MessageBoxA(0, "Failed to create effect", "Effect load error", 0);
        return hr;
    }

    g_pStarDataTechnique  = g_pStarEffect->GetTechniqueByName("StarDataTechnique");
    g_pWorldViewProjVar     = g_pStarEffect->GetVariableByName("g_WorldViewProj");
	g_pHeightScaleVar     = g_pStarEffect->GetVariableByName("g_HeightScale");
	g_pSizeScaleVar       = g_pStarEffect->GetVariableByName("g_SizeScale");
	g_pAspectVar          = g_pStarEffect->GetVariableByName("g_AspectRatio");
	g_pDistortionVar      = g_pStarEffect->GetVariableByName("g_Distortion");
	g_pMinSizeVar         = g_pStarEffect->GetVariableByName("g_minClipSize");
	

	(loadTextureFromFile("../../Media/TerrainTessellation/StarColours.jpg", "g_StarColours", g_pStarEffect));
	(loadTextureFromFile("../../Media/TerrainTessellation/StarShape1.dds",  "g_StarShape",   g_pStarEffect));
	(loadTextureFromFile("fBm5Octaves.dds", "g_fBmTexture",  g_pStarEffect));

	return 0;
}

void CreateInputLayout()
{
	int hr;

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "CORNER_OFFSET",   0, FORMAT_RG32f,   0, 0,  D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "GALACTIC_LATLON", 0, FORMAT_RG32f,	1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MAGNITUDE",       0, FORMAT_R32f,		1, 8,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "B_MINUS_V",       0, FORMAT_R32f,		1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	const size_t N_ELEMS = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	assert(g_pStarDataTechnique != 0);	// Load this first.
	D3DXFROMWINE11_PASS_DESC passDesc;
	(g_pStarDataTechnique->GetPassByIndex(0)->GetDesc(&passDesc));

	(->CreateInputLayout(layout, N_ELEMS, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &g_pInputLayout));
}
}	// anonymous namespace

void ReadStars()
{
	const char* file = "TerrainTessellation\\BrightStarCatalog.txt";
	ReadBrightStarCatalog(file);
}

void CreateStars()
{
	 CreateStarDataVB();
	 CreateStarCornersVB();
	 CreateStarEffect();
	 CreateInputLayout();
}

void ReleaseStars()
{
	//SAFE_RELEASE(g_pStarCornersVB);
	//SAFE_RELEASE(g_pStarDataVB);
	//SAFE_RELEASE(g_pInputLayout);
	//SAFE_RELEASE(g_pStarEffect);
}

D3DXFROMWINEMATRIX StarWorldMatrix(float starRotation = 0)
{
	// This world matrix moves Polaris very approximately overhead.  So we get a view
	// a bit like the northern hemisphere sky.
	D3DXFROMWINEMATRIX mWorld1, mWorld2, mWorld3;
	D3DXFROMWINEMatrixRotationY(&mWorld1, -123.3f / 180.0f * D3DXFROMWINE_PI);
	D3DXFROMWINEMatrixRotationX(&mWorld2,   54.0f / 180.0f * D3DXFROMWINE_PI);
	D3DXFROMWINEMatrixRotationY(&mWorld3, starRotation / 180.0f * D3DXFROMWINE_PI);
	D3DXFROMWINEMATRIX mWorld = mWorld1 * mWorld2 * mWorld3;
	return mWorld;
}

void RenderStars( const D3DXFROMWINEMATRIX& mView, const D3DXFROMWINEMATRIX& mProj, const D3DXFROMWINEVECTOR2& screenSize)
{
	IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLE_STRIP);
	IRenderer::GetRendererInstance()->setVertexFormat(g_pInputLayout);

    exBufferIDvertBuffers[2] = {g_pStarCornersVB, g_pStarDataVB};

    unsigned int uiVertexStrides[2] = {sizeof(StarCorner), sizeof(StarData)};
    unsigned int uOffsets[2] = {0,0};

	IRenderer::GetRendererInstance()->setVertexBuffer(0,2, vertBuffers, uiVertexStrides, uOffsets);

    D3DXFROMWINEMATRIX mWorldViewProj = StarWorldMatrix() * mView * mProj;
    g_pWorldViewProjVar->SetMatrix((float*)&mWorldViewProj);

	const float aspectRatio = screenSize.x / screenSize.y;
	g_pAspectVar->SetFloat(aspectRatio);
	D3DXFROMWINEVECTOR2 minClipSize(1.0f / screenSize.x, 1.0f / screenSize.y);
	g_pMinSizeVar->SetFloatVector(minClipSize);

	g_pHeightScaleVar->SetFloat(1);
	g_pSizeScaleVar->SetFloat(1);
	g_pDistortionVar->SetFloat(0);
	g_pStarDataTechnique->GetPassByIndex(0)->Apply(0);
	IRenderer::GetRendererInstance()->DrawInstanced(4, N_STARS, 0, 0);

	// Put an extra band of faint stars around the equator.  We rerender all stars and scale down 
	// their vertical coordinate to put them on the equator.  The equator is the galactic plane  
	// here.  This is phycisally bogus, but the result looks a bit like the Milky Way.  We add some
	// fBm distortion to the position, otherwise, it is too regular.
	g_pDistortionVar->SetFloat(0.1f);
	g_pHeightScaleVar->SetFloat(0.06f);
	g_pSizeScaleVar->SetFloat(0.3f);
	g_pStarDataTechnique->GetPassByIndex(0)->Apply(0);
	IRenderer::GetRendererInstance()->DrawInstanced(4, N_STARS, 0, 0);

	// Repeat the entire set again, at a randomly offset rotation and very small size, just to fill
	// in lots of tiny ones.  The min size stop them disappearing, no matter how small g_pSizeScaleVar.
    D3DXFROMWINEMATRIX mWorldViewProj2 = StarWorldMatrix(30) * mView * mProj;
    g_pWorldViewProjVar->SetMatrix((float*)&mWorldViewProj2);

	g_pDistortionVar->SetFloat(0.1f);
	g_pHeightScaleVar->SetFloat(1);
	g_pSizeScaleVar->SetFloat(0.1f);
	g_pStarDataTechnique->GetPassByIndex(0)->Apply(0);
	IRenderer::GetRendererInstance()->DrawInstanced(4, N_STARS, 0, 0);
}

