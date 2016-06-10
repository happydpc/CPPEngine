

//-----------------------------------------------------------------------------
// File: Skydome.cpp
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#include "Helper.h"
#include "Skydome.h"
#include "MManager.h"
#include "DXErrors.h"
#include "LitVertexVS2PS.h"

CSkyDome::CSkyDome() : CModel()
{
    m_mSkyID = -1;
    m_mGroundID = -1;
    m_mSkyInd = -1;
    m_mGroundInd = -1;
    m_pSkyVert = 0;
    m_pSkyVB = -1;
    m_pGroundVert = 0;
    m_pGroundVB = -1;
	m_bLoaded = false;
}

CSkyDome::~CSkyDome()
{
    SAFE_DELETE(m_pSkyVert);
    SAFE_DELETE(m_pGroundVert);
    Destroy();
}

int CSkyDome::Load()
{
	//LOG_FNLN;
	////LOG_PRINT("CFlyDemo::Load\n");
    FILE *fp;
    CHAR type[9];
    DWORD version;

    // Load sky model
    SAFE_DELETE(m_pSkyVert);

    std:string fn=stx_convertpath("/RadeonTerrainDemo/sky.dat");
	////LOG_PRINT("CSkyDome::Load:fopen:%s\n", fn.c_str());
	fp = fopen(fn.c_str(),"rb");

	fread(type, sizeof(CHAR), 8, fp);

	type[8] = '\0';
    
	fread(&version, sizeof(DWORD), 1, fp);
    
    if (strcmp(type, "SKY_FILE"))
    {
        fclose(fp);
        return 0;
    }
	//LOG_FNLN;

    
	fread(&m_dwSkyNumVert, sizeof(DWORD), 1, fp);
    
	LITVERTEX* m_pSkyVert0 = new LITVERTEX[m_dwSkyNumVert];
    
	fread(m_pSkyVert0, sizeof(LITVERTEX), m_dwSkyNumVert, fp);

	fclose(fp);

	LITVERTEX2* m_pSkyVert = new LITVERTEX2[m_dwSkyNumVert];
	for(unsigned int i = 0; i < m_dwSkyNumVert; i++)
		m_pSkyVert[i]=LITVERTEX2(m_pSkyVert0[i]);
	SAFE_DELETE(m_pSkyVert0);

    // Load lower ground model
    SAFE_DELETE(m_pGroundVert);

    
	 fn=stx_convertpath("/RadeonTerrainDemo/ground.dat");
	////LOG_PRINT("CSkyDome::Load:fopen:%s\n", fn.c_str());
	fp = fopen(fn.c_str(),"rb");
    
	fread(type, sizeof(CHAR), 8, fp);

	type[8] = '\0';
    
	fread(&version, sizeof(DWORD), 1, fp);
    
	if (strcmp(type, "GND_FILE"))
    {
        fclose(fp);
        return 0;
    }
	//LOG_FNLN;

    
	fread(&m_dwGroundNumVert, sizeof(DWORD), 1, fp);
    
	LITVERTEX* m_pGroundVert0 = new LITVERTEX[m_dwGroundNumVert];
    
	fread(m_pGroundVert0, sizeof(LITVERTEX), m_dwGroundNumVert, fp);

	fclose(fp);

	m_pGroundVert = new LITVERTEX2[m_dwGroundNumVert];
	for(unsigned int i = 0; i < m_dwGroundNumVert; i++)
		m_pGroundVert[i]=LITVERTEX2(m_pGroundVert0[i]);
	SAFE_DELETE(m_pGroundVert0);

    
	D3DFROMWINEMATERIAL9 m;
    ZeroMemory(&m, sizeof(D3DFROMWINEMATERIAL9));
    m.Diffuse.r = m.Ambient.r = 1.0f;
    m.Diffuse.g = m.Ambient.g = 1.0f;
    m.Diffuse.b = m.Ambient.b = 1.0f;
    m.Diffuse.a = m.Ambient.a = 1.0f;

    CMaterial *mtrl;
    mtrl = new CMaterial();
    m_mSkyID = mtrl->LoadMaterial(m, _T("Sky1"), false, false);

    m_mSkyInd=GameState::m_MMan->AddMaterial(mtrl);

    mtrl = new CMaterial();
    m_mGroundID = mtrl->LoadMaterial(m, _T("Ground"), true, false);

    m_mGroundInd=GameState::m_MMan->AddMaterial(mtrl);

    m_bLoaded = true;

	//LOG_FNLN;
	//LOG_PRINT("CSkyDome::Load\n");
	//LOG_PRINT("m_mSkyID = %d\n",m_mSkyID);
	//LOG_PRINT("m_mGroundID = %d\n",m_mGroundID);
//stx_exit(0);
	return 1;
}

int CSkyDome::Init()
{
	////LOG_PRINT("CFlyDemo::Init1\n");
    LPLITVERTEX pvbVertices;

    //if ((m_pSkyVert == 0) || (m_pGroundVert == 0))
        //return 0;
	////LOG_PRINT("CFlyDemo::Init2\n");
#if 0
    // Create and fill sky VB
    if (m_pSkyVB == 0)
    {
        GameState::m_pd3dDevice->CreateVertexBuffer(m_dwSkyNumVert*sizeof(LITVERTEX),
                               D3DUSAGE_WRITEONLY,
                               D3DFVF_LVERTEX1,
                               /*D3DPOOL_MANAGED*/D3DPOOL_DEFAULT,
                               &m_pSkyVB,
                               0 );
    }

    if (SUCCEEDED(m_pSkyVB->Lock( 0, 4*sizeof(LITVERTEX), (void**)&pvbVertices, 0 )))
    {
        unsigned long i;
        for(i = 0; i < m_dwSkyNumVert; i++)
    {
            pvbVertices[i] = m_pSkyVert[i];
    }
        m_pSkyVB->Unlock();
    }

    // Create and fill lower ground VB
    if (m_pGroundVB == 0)
    {
        GameState::m_pd3dDevice->CreateVertexBuffer(m_dwGroundNumVert*sizeof(LITVERTEX),
                               D3DUSAGE_WRITEONLY,
                               D3DFVF_LVERTEX1,
                               /*D3DPOOL_MANAGED*/D3DPOOL_DEFAULT,
                               &m_pGroundVB,
                               0 );
    }

    if (SUCCEEDED(m_pGroundVB->Lock( 0, m_dwGroundNumVert*sizeof(LITVERTEX), (void**)&pvbVertices, 0 )))
    {
        unsigned long i;
        for(i = 0; i < m_dwGroundNumVert; i++)
    {
            pvbVertices[i] = m_pGroundVert[i];
    }
        m_pGroundVB->Unlock();
    }
#else
	m_pSkyVB = IRenderer::GetRendererInstance()->addVertexBuffer(m_dwSkyNumVert * sizeof(LITVERTEX2), STATIC, m_pSkyVert);//,m_pSkyVB);
	m_pGroundVB =  IRenderer::GetRendererInstance()->addVertexBuffer(m_dwGroundNumVert * sizeof(LITVERTEX2), STATIC, m_pGroundVert);//,m_pGroundVB);
	////LOG_PRINT("m_pSkyVB=%x\n", m_pSkyVB);
	////LOG_PRINT("m_pGroundVB=%x\n", m_pGroundVB);
#endif
    return 1;
}

void CSkyDome::Destroy()
{
    //SAFE_RELEASE(m_pSkyVB);
    //SAFE_RELEASE(m_pGroundVB);
}

int CSkyDome::Render()
{
	//LOG_PRINT("CSkyDome::Render\n");
	//LOG_PRINT("m_bLoaded = %d\n",m_bLoaded?1:0);
	if (!m_bLoaded)
		return 0;
#if 0
    GameState::m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
#endif

    CMaterial *mtrl;
#if 0
    GameState::m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, true);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

    float start = 50000.0f;
    float end = 400000.0f;
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, 0x00ADB4C7);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(unsigned long*)(&start));
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(unsigned long*)(&end));
#endif
#if 0
    GameState::m_pd3dDevice->DrawPrimitiveVB(D3DPT_TRIANGLELIST, m_pGroundVB, 0, m_dwGroundNumVert, 0L);
    GameState::m_pd3dDevice->SetFVF(D3DFVF_LVERTEX1);
    GameState::m_pd3dDevice->SetStreamSource( 0, m_pGroundVB, 0, sizeof(LITVERTEX) );
    GameState::m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,  0, m_dwGroundNumVert/3 );
#else

	//LOG_PRINT("m_dwGroundNumVert = %x\n",m_dwGroundNumVert);
	if(m_dwGroundNumVert)
	{
	LitVertexVS2PS::Begin();
	LitVertexVS2PS::SetTexture(m_mGroundID);//???36);
#if 0
    mtrl = GameState::m_MMan->GetMaterial(m_mGroundInd);
    //LOG_PRINT("mtrl = %x\n",mtrl);
	if (mtrl != 0)
        mtrl->SetMaterial();
#endif
#if 1
	//LOG_PRINT("CSkyDome::Render:Ground\n");
	//LOG_PRINT("VF=%x\n", LitVertexVS2PS::LVERTEX1VF);
	//LOG_PRINT("VB=%x\n", m_pGroundVB);
	//LOG_PRINT("TexID=%d\n", m_mGroundID);
	//LOG_PRINT("VertNum=%x\n", m_dwGroundNumVert);
	
#endif	
      	IRenderer::GetRendererInstance()->setVertexFormat(LitVertexVS2PS::LVERTEX1VF);
	IRenderer::GetRendererInstance()->setVertexBuffer(0,m_pGroundVB);
#endif
		
	
	//LOG_FNLN;
      	//LOG_PRINT("DrawPrimitive\n");
	IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLES,	0, m_dwGroundNumVert/3/* , RESET_VB|RESET_IB */);
	
	
	LitVertexVS2PS::End();
	}
	LitVertexVS2PS::Begin();
	LitVertexVS2PS::SetTexture(m_mSkyID);//???35);
#if 0
    mtrl = GameState::m_MMan->GetMaterial(m_mSkyInd);
	//LOG_PRINT("mtrl = %x\n",mtrl);
    if (mtrl != 0)
        mtrl->SetMaterial();

    //GameState::m_pd3dDevice->DrawPrimitiveVB(D3DPT_TRIANGLELIST, m_pSkyVB, 0, m_dwSkyNumVert, 0L);
    GameState::m_pd3dDevice->SetStreamSource( 0, m_pSkyVB, 0, sizeof(LITVERTEX) );
    LitVertexVS2PS::m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,  0, m_dwSkyNumVert/3 );
#else
#if 1
	//LOG_PRINT("CSkyDome::Render:Sky\n");
	//LOG_PRINT("VF=%x\n", LitVertexVS2PS::LVERTEX1VF);
	//LOG_PRINT("VB=%x\n", m_pSkyVB);
	//LOG_PRINT("TexID=%x\n", m_mSkyID);
	//LOG_PRINT("m_dwSkyNumVert = %x\n",m_dwSkyNumVert);
#endif	
	
	if(m_dwSkyNumVert==0)
		return 1;

	IRenderer::GetRendererInstance()->setVertexFormat(LitVertexVS2PS::LVERTEX1VF);
	IRenderer::GetRendererInstance()->setVertexBuffer(0,m_pSkyVB);
		//IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->cullBack);
	    
	//LOG_FNLN;
      	//LOG_PRINT("DrawPrimitive\n");
	IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLES,	0, m_dwSkyNumVert/3/* , RESET_VB|RESET_IB */);
	
	LitVertexVS2PS::End();
#endif

#if 0
    GameState::m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    GameState::m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
#endif
    return 1;
}

unsigned long CSkyDome::CountPolys()
{
	if (!m_bLoaded)
		return 0;
    return (m_dwSkyNumVert + m_dwGroundNumVert)/ 3;
}
