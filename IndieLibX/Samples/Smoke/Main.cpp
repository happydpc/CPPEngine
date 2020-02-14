//----------------------------------------------------------------------------------
// File:   Main.cpp
// Author: Sarah Tariq and Ignacio Llamas
// Email:  sdkfeedback@nvidia.com
// 
// Copyright (c) 2007 NVIDIA_Corporation. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------

#include "resource.h"

#include "nvutPCH.h"
#include "Smoke.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "SDKmisc.h"
#include "sdkmesh_old.h"

#include "NVUTSkybox.h"

#include "Fluid.h"
#include "Voxelizer.h"
#include "SkinnedMesh.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

#define MAX_SO_BUFFERS 3
int                         g_dstSOBuffer = 0;
int                         g_srcSOBuffer = -1;
unsigned int                g_numWrittenSOBuffers = 0;
unsigned int                g_numWrittenSOBuffersSincePause = 0;

float                       g_Fovy = D3DX_PI * 0.25f;

// Window Dimensions
int                         g_Width  = 1024;
int                         g_Height = 768; 

// Misc options
bool                        g_bPause = false;
bool                        g_bRenderFluid = true;
bool                        g_bRenderMesh = true;
int                         g_currentRenderChoice = 0;
int                         g_gridWidth = 70;
int                         g_gridHeight = 70;
int                         g_gridDepth = 100;
int                         g_numJacobi = 6; 
float                       g_fTimestep = 2.0f;
bool                        g_bReinitializeFluid = false;
bool                        g_bPauseAnimation = false;
bool                        g_bRenderGlow = true;
Fluid::FLUID_TYPE           g_eFluidType = Fluid::FT_SMOKE;
bool                        g_bVoxelizeSkinnedMesh = true;
bool                        g_bStreamoutSkinnedMesh = true;

float                       g_fModelRotation = 0.0;
float                       g_fRotSpeed = 4.0;

//variables for the fire
float g_glowContribution     = 0.81f;
float g_finalIntensityScale  = 28.0f; 
float g_finalAlphaScale      = 0.95f;
float g_smokeColorMultiplier = 2.0f;   
float g_smokeAlphaMultiplier = 0.05f; 
int   g_RednessFactor        = 5; 


TCHAR* g_tstr_renderModeLabels[] = 
{ 
    TEXT("Scene"), 
    TEXT("Phi as density"),
    TEXT("Phi as level set"),
    TEXT("Gradient of phi"),
    TEXT("Velocity Field"),
    TEXT("Pressure Field"),
    TEXT("Voxelized Obstacles"),
    NULL
};

TCHAR* g_tstr_fluidTypeLabels[] = 
{
    TEXT("Smoke"),
    TEXT("Fire"),
    TEXT("Water"),
    NULL
};


// Fluid Simulation state
Fluid*                      g_fluid = NULL;
Voxelizer*                  g_voxelizer = NULL;
VolumeRenderer*             g_renderer = NULL;

// Camera state
CModelViewerCamera          g_Camera;
float                       g_zNear = 1.0f;
float                       g_zFar = 1000.0f;
D3DXVECTOR3                 g_Eye = D3DXVECTOR3( 0.0f, 0.0f, -20.0f );
D3DXVECTOR3                 g_At = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
D3DXVECTOR3                 g_Up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
D3DXMATRIX                  g_View;
D3DXMATRIX                  g_Projection;

// Scene state
//   World transforms
D3DXMATRIX                  g_gridWorld;
D3DXMATRIX                  g_meshWorld;
D3DXVECTOR3                 g_obstaclePos = D3DXVECTOR3(0.5, 0.5, 0.5);
SkinnedMesh*                g_skinnedMesh = NULL;
float                       g_meshCenter[3];
float                       g_meshRadius;
SkinnedMesh::stdStringVector    g_voxelization_includeNodes;
SkinnedMesh::stdStringVector    g_rendering_excludeNodes;

NVUTSkybox                  g_Skybox;
// Environment map
ID3D10Texture2D *           g_pEnvMap       = NULL;                    
ID3D10ShaderResourceView *  g_pEnvMapSRV    = NULL;

// Additional depth buffer (to render the scene into) 
//   so that we can sample from it in VolumeRenderer
ID3D10Texture2D*            g_pSceneDepthTex2D      = NULL;
ID3D10Texture2D*            g_pSceneDepthTex2DNonMS = NULL;
ID3D10ShaderResourceView*   g_pSceneDepthSRV        = NULL;
ID3D10RenderTargetView*     g_pSceneDepthRTV        = NULL;


// Any macros to be passed to the shaders used in this sample (for conditional compilation)
D3D10_SHADER_MACRO g_shadersMacros[] = 
{
    NULL
};
D3D10_SHADER_MACRO *g_pShadersMacros = g_shadersMacros;


// Mouse State for user-interaction with the simulation
bool                        g_bEnableMouseInteraction = false;
bool                        leftIsPressed = false;
int                         mouse_x = g_gridWidth/2;
int                         mouse_y = g_gridHeight/2;
int                         mouse_z = g_gridDepth/2;
int                         last_x  = g_gridWidth/2;
int                         last_y  = g_gridHeight/2;
int                         last_z  = g_gridDepth/2;
bool                        last_mouse = false;
bool                        mouse_active = false;
bool                        justClicked = false;

// UI state
ID3DX10Font*                g_pFont = NULL;         // Font for drawing text
ID3DX10Sprite*              g_pSprite = NULL;       // Sprite for batching text drawing
CDXUTTextHelper*            g_pTxtHelper = NULL;
bool                        g_bShowHelp = false;    // show help menu
bool                        g_bDrawUI = true;       // show UI
CDXUTDialogResourceManager  g_DialogResourceManager;// manager for shared resources of dialogs
CD3DSettingsDlg             g_D3DSettingsDlg;       // Device settings dialog
CDXUTDialog                 g_HUD;                  // manages the 3D UI
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
enum {
    IDC_TOGGLEFULLSCREEN,
    IDC_TOGGLEREF,
    IDC_CHANGEDEVICE,
    IDC_CHANGEFLUIDTYPE,
    IDC_CHANGERENDER,

    IDC_PAUSEANIMATION,
    IDC_PAUSESIMULATION,
    IDC_RENDERFLUID,
    IDC_RENDERMODEL,
    IDC_DO_INOUT_VOXELIZATION,
    IDC_DO_VEL_VOXELIZATION,

    IDC_USEMACCORMACK,
    IDC_GRAVITY,
    IDC_WATER_STREAM,

    IDC_GRIDWIDTH_STATIC,
    IDC_GRIDWIDTH_SCALE,
    IDC_GRIDHEIGHT_STATIC,
    IDC_GRIDHEIGHT_SCALE,
    IDC_GRIDDEPTH_STATIC,
    IDC_GRIDDEPTH_SCALE,
    IDC_GRIDAPPLY,

    IDC_NUMJACOBI_STATIC,
    IDC_NUMJACOBI_SCALE,
    
    IDC_TOGGLEPRESET1,
    
    IDC_RENDERGLOW,
    IDC_GLOWCONTRIBUTION_STATIC,
    IDC_GLOWCONTRIBUTION_SCALE,

    IDC_FINALINTENSITY_STATIC,
    IDC_FINALINTENSITY_SCALE,
    IDC_FINALALPHA_STATIC,
    IDC_FINALALPHA_SCALE,
    IDC_SMOKECOLOR_STATIC,
    IDC_SMOKECOLOR_SCALE,
    IDC_SMOKEALPHA_STATIC,
    IDC_SMOKEALPHA_SCALE,
    IDC_REDNESS_STATIC,
    IDC_REDNESS_SCALE,

    IDC_XYVELOCITY_STATIC,
    IDC_XYVELOCITY_SCALE,
    IDC_ZVELOCITY_STATIC,
    IDC_ZVELOCITY_SCALE,

};

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
bool    CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnD3D10DestroyDevice( void* pUserContext );
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext );

void    CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    RenderText();


HRESULT CreateMesh(ID3D10Device* pd3dDevice);
HRESULT InitializeFluidState(ID3D10Device* pd3dDevice);
HRESULT ReinitWindowSizeDependentRenderTargets(ID3D10Device* pd3dDevice);

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                       int xPos, int yPos, void* pUserContext );

void resetVariablesToPreset1();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available system depending on which D3D callbacks set below

    // Set DXUT callbacks
    DXUTSetCallbackD3D10DeviceAcceptable( IsD3D10DeviceAcceptable );
    DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10SwapChainReleasing( OnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( OnD3D10DestroyDevice );
    DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );

    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackMouse( OnMouse, true );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    
    DXUTCreateWindow( TEXT(" Fluid Simulation Sample") );
    DXUTCreateDevice( true, g_Width, g_Height );
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
//  
//--------------------------------------------------------------------------------------

void resetGUIControls()
{
    TCHAR sz[100];

    g_SampleUI.RemoveAllControls();

    int iX = g_Width - 155;
    int iY = 10;

    int iDefWidth  = 125;
    int iSldrWidth = 100;
    int iDefHeight = 22;
    int iDefYgap = 24;
    int iSldrYgap = 20;
    int iSldrXgap = 20;

    g_SampleUI.AddButton(IDC_TOGGLEPRESET1,TEXT("Preset"), iX+20, iY+=iDefYgap, iDefWidth, iDefHeight, VK_F4, false);

    iY+=20;

    // Simulation options
    g_SampleUI.AddComboBox( IDC_CHANGEFLUIDTYPE, iX, iY += iDefYgap, 150, 22 );
    for(int i=0; g_tstr_fluidTypeLabels[i] != NULL; i++)
        g_SampleUI.GetComboBox( IDC_CHANGEFLUIDTYPE )->AddItem( g_tstr_fluidTypeLabels[i], NULL );
    g_SampleUI.GetComboBox( IDC_CHANGEFLUIDTYPE )->SetSelectedByIndex(g_eFluidType);

    g_SampleUI.AddCheckBox( IDC_PAUSEANIMATION, TEXT("Pause Animation"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_bPauseAnimation );
    g_SampleUI.AddCheckBox( IDC_PAUSESIMULATION, TEXT("Pause Simulation"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_bPause );
    g_SampleUI.AddCheckBox( IDC_DO_INOUT_VOXELIZATION, TEXT("Do In/Out Voxelization"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_voxelizer ? g_voxelizer->GetEnableInOutVoxelize() : true );
    g_SampleUI.AddCheckBox( IDC_DO_VEL_VOXELIZATION, TEXT("Do Velocity Voxelization"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_voxelizer ? g_voxelizer->GetEnableVelocityVoxelize() : true );
    g_SampleUI.AddCheckBox( IDC_USEMACCORMACK,  TEXT("MacCormack Advection"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_fluid ? g_fluid->GetUseMACCORMACK() : true );
    g_SampleUI.AddCheckBox( IDC_GRAVITY,  TEXT("Apply Gravity"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_fluid ? g_fluid->GetEnableGravity() : false );
    g_SampleUI.AddCheckBox( IDC_WATER_STREAM,  TEXT("Enable Liquid Stream"), iX, iY += iDefYgap, iDefWidth, iDefHeight, g_fluid ? g_fluid->GetEnableLiquidStream() : false );
    StringCchPrintf( sz, 100, TEXT("Jacobi Iterations: %0.2f"), float(g_numJacobi) ); 
    g_SampleUI.AddStatic( IDC_NUMJACOBI_STATIC, sz  , iX, iY += iDefYgap, iDefWidth, iDefHeight );
    g_SampleUI.AddSlider( IDC_NUMJACOBI_SCALE       , iX+iSldrXgap, iY += iSldrYgap, iSldrWidth, iDefHeight, 1, 128, g_numJacobi );

    // Rendering options
    g_SampleUI.AddComboBox( IDC_CHANGERENDER, iX, iY += (2*iDefYgap), 150, 22 );
    for(int i=0; g_tstr_renderModeLabels[i] != NULL; i++)
        g_SampleUI.GetComboBox( IDC_CHANGERENDER )->AddItem( g_tstr_renderModeLabels[i], NULL );
    g_SampleUI.GetComboBox( IDC_CHANGERENDER )->SetSelectedByIndex(g_currentRenderChoice);

    g_SampleUI.AddCheckBox( IDC_RENDERFLUID, TEXT("Render Fluid")          , iX, iY += iDefYgap, iDefWidth, iDefHeight, g_bRenderFluid );
    g_SampleUI.AddCheckBox( IDC_RENDERMODEL, TEXT("Render Model")          , iX, iY += iDefYgap, iDefWidth, iDefHeight, g_bRenderMesh );
    g_SampleUI.AddCheckBox( IDC_RENDERGLOW, TEXT("Render Glow")          , iX, iY += iDefYgap, iDefWidth, iDefHeight, g_bRenderGlow );

    // Simulation grid size
    iY += 10;
    StringCchPrintf( sz, 100, TEXT("Grid Width: %0.2f"), float(g_gridWidth) ); 
    g_SampleUI.AddStatic( IDC_GRIDWIDTH_STATIC, sz  , iX, iY += iDefYgap, iDefWidth, iDefHeight );
    g_SampleUI.AddSlider( IDC_GRIDWIDTH_SCALE       , iX+iSldrXgap, iY += iSldrYgap, iSldrWidth, iDefHeight, 32, 2048, g_gridWidth );

    StringCchPrintf( sz, 100, TEXT("Grid Height: %0.2f"), float(g_gridHeight) ); 
    g_SampleUI.AddStatic( IDC_GRIDHEIGHT_STATIC, sz , iX, iY += iDefYgap, iDefWidth, iDefHeight );
    g_SampleUI.AddSlider( IDC_GRIDHEIGHT_SCALE      , iX+iSldrXgap, iY += iSldrYgap, iSldrWidth, iDefHeight, 32, 2048, g_gridHeight );

    StringCchPrintf( sz, 100, TEXT("Grid Depth: %0.2f"), float(g_gridDepth) ); 
    g_SampleUI.AddStatic( IDC_GRIDDEPTH_STATIC, sz  , iX, iY += iDefYgap, iDefWidth, iDefHeight );
    g_SampleUI.AddSlider( IDC_GRIDDEPTH_SCALE       , iX+iSldrXgap, iY += iSldrYgap, iSldrWidth, iDefHeight, 32, 2048, g_gridDepth );

    iY += 35;
    g_SampleUI.AddButton( IDC_GRIDAPPLY, TEXT("Change Grid Dimensions"), iX, iY, iDefWidth, iDefHeight );



    if(g_eFluidType == Fluid::FT_FIRE)
    {
        iX = 15;
        iY = 10;

        //fire controls
        StringCchPrintf( sz, 100, TEXT("Glow Contribution: %0.2f"), float(g_glowContribution) ); 
        g_SampleUI.AddStatic( IDC_GLOWCONTRIBUTION_STATIC, sz   , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_GLOWCONTRIBUTION_SCALE        , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 0, 100, (int)(g_glowContribution*100) );

        StringCchPrintf( sz, 100, TEXT("Final Intensity Scale: %0.2f"), float(g_finalIntensityScale) ); 
        g_SampleUI.AddStatic( IDC_FINALINTENSITY_STATIC, sz     , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_FINALINTENSITY_SCALE          , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 1, 60, (int)g_finalIntensityScale );

        StringCchPrintf( sz, 100, TEXT("Final Alpha Scale: %0.2f"), float(g_finalAlphaScale) ); 
        g_SampleUI.AddStatic( IDC_FINALALPHA_STATIC, sz         , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_FINALALPHA_SCALE              , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 0, 100, (int)(g_finalAlphaScale*100) );

        StringCchPrintf( sz, 100, TEXT("Smoke Color Scale: %0.2f"), float(g_smokeColorMultiplier) ); 
        g_SampleUI.AddStatic( IDC_SMOKECOLOR_STATIC, sz         , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_SMOKECOLOR_SCALE              , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 0, 200, (int)(g_smokeColorMultiplier*100) );

        StringCchPrintf( sz, 100, TEXT("Smoke Alpha Scale: %0.2f"), float(g_smokeAlphaMultiplier) ); 
        g_SampleUI.AddStatic( IDC_SMOKEALPHA_STATIC, sz         , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_SMOKEALPHA_SCALE              , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 0, 50, (int)(g_smokeAlphaMultiplier*100) );

        StringCchPrintf( sz, 100, TEXT("Redness Scale: %0.2f"), float(g_RednessFactor) ); 
        g_SampleUI.AddStatic( IDC_REDNESS_STATIC, sz            , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_REDNESS_SCALE                 , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 1, 5, g_RednessFactor );

        StringCchPrintf( sz, 100, TEXT("XY Velocity Scale: %0.2f"), g_xyVelocityScale ); 
        g_SampleUI.AddStatic( IDC_XYVELOCITY_STATIC, sz         , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_XYVELOCITY_SCALE              , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 1, 10, (int)g_xyVelocityScale );

        StringCchPrintf( sz, 100, TEXT("Z Velocity Scale: %0.2f"), g_zVelocityScale ); 
        g_SampleUI.AddStatic( IDC_ZVELOCITY_STATIC, sz          , iX, iY += iDefYgap, iDefWidth, iDefHeight );
        g_SampleUI.AddSlider( IDC_ZVELOCITY_SCALE               , iX, iY += iSldrYgap, iSldrWidth, iDefHeight, 1, 10, (int)g_zVelocityScale );
    }
}


//--------------------------------------------------------------------------------------
// Reject any D3D10 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC *pBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr(S_OK);

    g_D3DSettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );
    g_HUD.SetCallback( OnGUIEvent );
    g_SampleUI.SetCallback( OnGUIEvent ); 

    int iY = 10; 
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, TEXT("Toggle full screen"), 15, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, TEXT("Toggle REF (F3)"), 15, iY += 24, 125, 22, VK_F3 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, TEXT("Change device (F2)"), 15, iY += 24, 125, 22, VK_F2 );


    V_RETURN(DXUTSetMediaSearchPath(TEXT("..\\Source\\Smoke")));

    V_RETURN(g_DialogResourceManager.OnD3D10CreateDevice( pd3dDevice ));
    V_RETURN(g_D3DSettingsDlg.OnD3D10CreateDevice( pd3dDevice ));

    V_RETURN(D3DX10CreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                                TEXT("Arial"), &g_pFont ));
    V_RETURN(D3DX10CreateSprite( pd3dDevice, 512, &g_pSprite ));
    g_pTxtHelper = new CDXUTTextHelper( NULL, NULL, g_pFont, g_pSprite, 15 );
    if(!g_pTxtHelper)
        return E_OUTOFMEMORY;

    // Initialize skybox and environment map
    TCHAR str[MAX_PATH];
    ID3D10Resource * pRes = NULL;
    V( NVUTFindDXSDKMediaFileCchT( str, MAX_PATH, L"..\\..\\Media\\VolumetricWater\\cubemap.dds" ) );
    V( D3DX10CreateTextureFromFile( pd3dDevice, str, NULL, NULL, &pRes, NULL ) );
    if(pRes)
    {
        V( pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&g_pEnvMap ));
        if (g_pEnvMap)
        {
            D3D10_TEXTURE2D_DESC desc;
            g_pEnvMap->GetDesc( &desc );
            D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
            ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
            SRVDesc.Format = desc.Format;
            SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
            SRVDesc.TextureCube.MipLevels = desc.MipLevels;
            V( pd3dDevice->CreateShaderResourceView( g_pEnvMap, &SRVDesc, &g_pEnvMapSRV ));
        }
        SAFE_RELEASE(pRes);
    }
    V_RETURN( g_Skybox.OnCreateDevice( pd3dDevice ) );
    g_Skybox.SetTexture( g_pEnvMapSRV );

    V_RETURN(CreateMesh(pd3dDevice));

    V_RETURN(InitializeFluidState(pd3dDevice));

    // Initialize the view matrix
    g_Camera.SetViewParams( &g_Eye, &g_At );
    g_Camera.SetEnablePositionMovement(true);
    g_Camera.SetScalers(0.004f, 20.0f);
    g_View = *g_Camera.GetViewMatrix();
    g_Projection = *g_Camera.GetProjMatrix();

    resetVariablesToPreset1();
    resetGUIControls();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Load a mesh to put inside the fluid
//--------------------------------------------------------------------------------------
HRESULT CreateMesh(ID3D10Device* pd3dDevice)
{
    HRESULT hr(S_OK);

    // try loading skinned mesh
    TCHAR fullMeshPath[MAX_PATH];

#if 1
    V_RETURN(NVUTFindDXSDKMediaFileCchT( fullMeshPath, MAX_PATH, TEXT("..\\..\\Media\\Gargoyle\\gargoyle.dae") ));
    g_voxelization_includeNodes.push_back("GargoyleCollision");
    g_rendering_excludeNodes.push_back("GargoyleCollision");
#else
    //V_RETURN(NVUTFindDXSDKMediaFileCchT( fullMeshPath, MAX_PATH, TEXT("..\\..\\Media\\bunny.dae") ));
    //V_RETURN(NVUTFindDXSDKMediaFileCchT( fullMeshPath, MAX_PATH, TEXT("..\\..\\Media\\dragon.dae") ));
    //V_RETURN(NVUTFindDXSDKMediaFileCchT( fullMeshPath, MAX_PATH, TEXT("..\\..\\Media\\happy.dae") ));
#endif
    g_skinnedMesh = SkinnedMesh::CreateD3D10FromColladaFile(fullMeshPath, pd3dDevice, MAX_SO_BUFFERS);
    if(!g_skinnedMesh)
        return E_FAIL;

    g_skinnedMesh->GetBoundingSphere(g_meshCenter, &g_meshRadius);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Initialize fluid and voxelizer states
//--------------------------------------------------------------------------------------
HRESULT InitializeFluidState(ID3D10Device* pd3dDevice)
{
    HRESULT hr(S_OK);

    SAFE_DELETE(g_fluid);
    SAFE_DELETE(g_voxelizer);
	SAFE_DELETE(g_renderer);


    // Initialize fluid state
    g_fluid = new Fluid(pd3dDevice);
    if( !g_fluid ) 
        return E_OUTOFMEMORY;
	g_renderer = new VolumeRenderer( pd3dDevice );
    if( !g_renderer ) 
	{        
		SAFE_DELETE(g_renderer);
		return E_OUTOFMEMORY;
	}

    V_RETURN(g_fluid->Initialize(g_gridWidth, g_gridHeight, g_gridDepth, g_eFluidType));

	VolumeRenderer::RENDER_MODE renderMode(VolumeRenderer::RM_SMOKE);
    switch( g_eFluidType )
    {
        default:
		case Fluid::FT_SMOKE:
            renderMode = VolumeRenderer::RM_SMOKE;
            break;
		case Fluid::FT_FIRE:
            renderMode = VolumeRenderer::RM_FIRE;
            break;
		case Fluid::FT_LIQUID:
            renderMode = VolumeRenderer::RM_LEVELSET;
            break;
    }
	
	V_RETURN(g_renderer->Initialize( g_fluid->GetDimX(), g_fluid->GetDimY(), g_fluid->GetDimZ(), renderMode));
    V_RETURN(g_renderer->SetScreenSize(g_Width, g_Height));

    if( g_SampleUI.GetCheckBox( IDC_USEMACCORMACK ) )
        g_fluid->SetUseMACCORMACK(g_SampleUI.GetCheckBox( IDC_USEMACCORMACK )->GetChecked());

    // Initialize voxelizer state
    g_voxelizer = new Voxelizer();
    if( !g_voxelizer )
        return E_OUTOFMEMORY;

    if( g_SampleUI.GetCheckBox( IDC_DO_INOUT_VOXELIZATION ) )
        g_voxelizer->SetEnableInOutVoxelize(g_SampleUI.GetCheckBox( IDC_DO_INOUT_VOXELIZATION )->GetChecked());
    if( g_SampleUI.GetCheckBox( IDC_DO_VEL_VOXELIZATION ) )
        g_voxelizer->SetEnableVelocityVoxelize(g_SampleUI.GetCheckBox( IDC_DO_VEL_VOXELIZATION )->GetChecked());

    V_RETURN(g_voxelizer->SetDestination(pd3dDevice, 
        g_fluid->Get3DTexture(Fluid::RENDER_TARGET_OBSTACLES),
        g_fluid->Get3DTexture(Fluid::RENDER_TARGET_OBSTVELOCITY)));

    if( g_eFluidType == Fluid::FT_FIRE )
        g_voxelizer->SetVelocityMultiplier(0.2f);
  

    g_bStreamoutSkinnedMesh = !g_bPauseAnimation;


    // Re-initialize the UI controls
    resetGUIControls();

    g_bReinitializeFluid = false;

    return hr;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr(S_OK);
    
    pd3dDevice->OMSetRenderTargets( 0, NULL, NULL );

    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBufferSurfaceDesc ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBufferSurfaceDesc ) );

    g_Width = pBufferSurfaceDesc->Width;
    g_Height = pBufferSurfaceDesc->Height;

    // Setup the projection parameters again
    float fAspect = static_cast<float>( pBufferSurfaceDesc->Width )/static_cast<float>( pBufferSurfaceDesc->Height );
    g_Camera.SetProjParams( g_Fovy , fAspect, g_zNear, g_zFar);
    g_Camera.SetWindow( pBufferSurfaceDesc->Width, pBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks(NULL, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON );

    g_HUD.SetLocation( pBufferSurfaceDesc->Width-170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( 0, 80 );
    g_SampleUI.SetSize( 170, 300 );

    V_RETURN( ReinitWindowSizeDependentRenderTargets(pd3dDevice));
    if( g_renderer ) 
       V_RETURN( g_renderer->SetScreenSize(g_Width, g_Height) );

    g_Skybox.OnResizedSwapChain(pd3dDevice, pBufferSurfaceDesc);

    resetGUIControls();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Initialize any textures that must match the window size
//--------------------------------------------------------------------------------------
HRESULT ReinitWindowSizeDependentRenderTargets(ID3D10Device* pd3dDevice)
{
    HRESULT hr(S_OK);

    // Create resources to enable writing the scene depth using MRT, as well as to 
    //  enable reading as a shader resource
    ID3D10RenderTargetView *pRTV = DXUTGetD3D10RenderTargetView();
    ID3D10Resource *pRTVResource;
    pRTV->GetResource(&pRTVResource);
    ID3D10Texture2D *pRTVTex2D = static_cast<ID3D10Texture2D*>(pRTVResource);
    assert(pRTVTex2D);
    D3D10_TEXTURE2D_DESC pRTVTex2DDesc;
    pRTVTex2D->GetDesc(&pRTVTex2DDesc);
    pRTVResource->Release();    

    SAFE_RELEASE(g_pSceneDepthTex2DNonMS);
    SAFE_RELEASE(g_pSceneDepthTex2D);
    SAFE_RELEASE(g_pSceneDepthRTV);
    SAFE_RELEASE(g_pSceneDepthSRV);

    D3D10_TEXTURE2D_DESC desc;
    desc.ArraySize = 1;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MipLevels = 1;
    desc.MiscFlags = 0;
    desc.SampleDesc = pRTVTex2DDesc.SampleDesc;
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.Width = g_Width;
    desc.Height = g_Height;
    desc.Format = DXGI_FORMAT_R32_FLOAT;
    V_RETURN(pd3dDevice->CreateTexture2D(&desc,NULL,&g_pSceneDepthTex2D));

    // We need a Non-Multisampled texture2D resource of the same dimensions to read from in shaders
    if(pRTVTex2DDesc.SampleDesc.Count > 1)
    {
        desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        V_RETURN(pd3dDevice->CreateTexture2D(&desc,NULL,&g_pSceneDepthTex2DNonMS));
    }
    else
    {
        g_pSceneDepthTex2DNonMS = g_pSceneDepthTex2D;
        g_pSceneDepthTex2DNonMS->AddRef();
    }

    // Create the render target view for the potentially Multisampled texture2D resource
    D3D10_RENDER_TARGET_VIEW_DESC descRTV;
    descRTV.Format = DXGI_FORMAT_R32_FLOAT;
    if( pRTVTex2DDesc.SampleDesc.Count <= 1 )
    {
        descRTV.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
        descRTV.Texture2D.MipSlice = 0;
    }
    else
    {
        descRTV.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DMS;
    }
    V_RETURN( pd3dDevice->CreateRenderTargetView( g_pSceneDepthTex2D, &descRTV, &g_pSceneDepthRTV ) );

    // Create a shader resource view for a Non-MS texture
    D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = DXGI_FORMAT_R32_FLOAT;
    descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MipLevels = 1;
    descSRV.Texture2D.MostDetailedMip = 0;
    V_RETURN( pd3dDevice->CreateShaderResourceView(g_pSceneDepthTex2DNonMS, &descSRV, &g_pSceneDepthSRV) );

    return S_OK;

}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    if( !g_bPauseAnimation )
    {
        // animation moves at the same rate as the fluid simulation
        float fTimestep = g_fTimestep * 0.025f;

        g_fModelRotation += fTimestep * g_fRotSpeed/10; 

        if( g_skinnedMesh )
            g_skinnedMesh->Update(fTimestep);
    }


    // Get the render target and depth buffer, and clear them
    ID3D10RenderTargetView *pRTV = DXUTGetD3D10RenderTargetView();
    ID3D10DepthStencilView *pDSV = DXUTGetD3D10DepthStencilView();
    
    if(g_eFluidType == Fluid::FT_FIRE)
    {
        float color[4] = {40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 0 };
        pd3dDevice->ClearRenderTargetView( pRTV, color );
    }
    else
    {
        float color[4] = {0, 0, 0, 0 };
        pd3dDevice->ClearRenderTargetView( pRTV, color );
    }
    float colorZ[4] = { g_zFar,0,0,0};
    pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH , 1.0, 0 );
    pd3dDevice->ClearRenderTargetView( g_pSceneDepthRTV, colorZ );
    
    // Create a viewport to match the screen size
    D3D10_VIEWPORT rtViewport;
    rtViewport.TopLeftX = 0;
    rtViewport.TopLeftY = 0;
    rtViewport.MinDepth = 0;
    rtViewport.MaxDepth = 1;
    rtViewport.Width = g_Width;
    rtViewport.Height = g_Height;

    // If the settings dialog is being shown, then
    //  render it instead of rendering the app's scene
    if( g_D3DSettingsDlg.IsActive() )
    {
        // Set the viewport
        pd3dDevice->RSSetViewports(1,&rtViewport);
        // Render the scene to the screen
        pd3dDevice->OMSetRenderTargets( 1, &pRTV , pDSV ); 
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        pd3dDevice->OMSetRenderTargets( 0, NULL, NULL );
        return;
    }

    // If the grid size changed we need to reinitialize the fluid
    if(g_bReinitializeFluid)
    {
        HRESULT hr(S_OK);
        V(InitializeFluidState(pd3dDevice));
    }

    if(g_fluid == NULL)
    {
        pd3dDevice->OMSetRenderTargets( 0, NULL, NULL );
        return;
    }
    
    
    // Update the simulation one time step
    if(!g_bPause)
    {
        g_fluid->SetNumJacobiIterations(g_numJacobi);
        g_fluid->Update( g_fTimestep);
    }

    // Initialize the world matrices for the simulation Grid and the obstacle Mesh
    //   Grid
    D3DXMATRIX gridScale, gridRotate;
    D3DXMatrixScaling(&gridScale, 20.0f, 20.0f, 20.0f );
    D3DXMatrixRotationX(&gridRotate, 3.0f*3.1416f/2.0f);
    g_gridWorld = gridScale * gridRotate;
    //   Mesh
    D3DXMATRIX meshScale, meshRotate, meshTranslate;
    D3DXVECTOR3 meshT(0,0,0);
    float meshS(1);
    if(g_eFluidType == Fluid::FT_FIRE) 
    {   
        meshS = 10.6f / g_meshRadius;
        meshT = D3DXVECTOR3(-g_meshCenter[0]*meshS, -g_meshCenter[1]*meshS -4.0f, -g_meshCenter[2]*meshS);
    }
    else
    {
        meshS = 10.0f / g_meshRadius;
        meshT = D3DXVECTOR3(-g_meshCenter[0]*meshS, -g_meshCenter[1]*meshS, -g_meshCenter[2]*meshS);
    }
    D3DXMatrixTranslation(&meshTranslate, meshT .x, meshT.y, meshT.z );
    D3DXMatrixScaling(&meshScale, meshS, meshS, meshS);
    D3DXMatrixRotationY(&meshRotate, 3.146f);
    g_meshWorld = meshScale * meshRotate * meshTranslate;
    

    if(g_voxelizer && (g_voxelizer->GetEnableInOutVoxelize() || g_voxelizer->GetEnableVelocityVoxelize()) )
    {
        if( g_bStreamoutSkinnedMesh )
        {
            // Streamout the skinned mesh, it's needed for voxelization only
            D3DXMATRIX identity;
            D3DXMatrixIdentity(&identity);
            g_skinnedMesh->SetRenderFilterExclude(NULL);
            g_skinnedMesh->SetRenderFilterInclude(NULL);
            g_skinnedMesh->RenderToSO((float*)&identity, g_dstSOBuffer);

            // Advance the streamout buffer counters
            g_numWrittenSOBuffers++;
            
            // handle potential overflow
            if(g_numWrittenSOBuffers == 0)
                g_numWrittenSOBuffers = 2;
            
            g_srcSOBuffer = g_dstSOBuffer;
            g_dstSOBuffer++;
            g_dstSOBuffer %= MAX_SO_BUFFERS;

            // stop streamout after streaming out 3 times since paused
            if( g_bPauseAnimation )
            {
                g_numWrittenSOBuffersSincePause++;
                if( g_numWrittenSOBuffersSincePause > 2)
                    g_bStreamoutSkinnedMesh = false;
            }
        }

        // Voxelize the mesh if needed
        if(g_numWrittenSOBuffers > 1)
        {
            // Compute mesh-to-grid xform
            D3DXMATRIX gridWorldInv;
            D3DXMatrixInverse(&gridWorldInv, NULL, &g_gridWorld);
            D3DXMATRIX meshToGridXForm = g_meshWorld * gridWorldInv;

            int prevSrcSOBuffer = g_srcSOBuffer-1;
            if(prevSrcSOBuffer < 0) prevSrcSOBuffer = MAX_SO_BUFFERS-1;

            if( g_voxelization_includeNodes.size() > 0 )
                g_skinnedMesh->SetRenderFilterInclude(&g_voxelization_includeNodes);
            //if( g_rendering_excludeNodes.size() > 0 )
            //    g_skinnedMesh->SetRenderFilterExclude(&g_rendering_excludeNodes);
            

            g_voxelizer->Voxelize(meshToGridXForm, g_skinnedMesh, 
                g_srcSOBuffer, prevSrcSOBuffer, g_fTimestep);

            g_skinnedMesh->SetRenderFilterInclude(NULL);
            g_skinnedMesh->SetRenderFilterExclude(NULL);
        }

    }

    if(g_currentRenderChoice > 0) 
    {
        // Draw one of the simulation textures
        g_fluid->Draw( g_currentRenderChoice );
    } 
    else
    {
        ID3D10RenderTargetView *pCnDRTs[2] = { pRTV, g_pSceneDepthRTV };
        pd3dDevice->RSSetViewports(1,&rtViewport);
        pd3dDevice->OMSetRenderTargets( 2, pCnDRTs, pDSV );

        // Compute and set modelview matrix
        g_Projection = *(g_Camera.GetProjMatrix());
        g_View = *(g_Camera.GetViewMatrix());

        // Render the skybox if needed
        if( g_eFluidType == Fluid::FT_LIQUID )
        {
            g_Skybox.OnFrameRender(g_View * g_Projection);
        }

        // Render the mesh
        if(g_bRenderMesh)
        {
            if( g_rendering_excludeNodes.size() > 0 )
                g_skinnedMesh->SetRenderFilterExclude(&g_rendering_excludeNodes);

            // Render the scene color and depth (so that we can sample from it in VolumeRenderer)
            g_skinnedMesh->Render((float*)&g_meshWorld, (float*)&g_View, (float*)&g_Projection);

            g_skinnedMesh->SetRenderFilterExclude(NULL);
        }

        // Resolve the MS depth buffer into a single-sampled one if needed
        if(g_pSceneDepthTex2DNonMS != g_pSceneDepthTex2D)
        {
            pd3dDevice->ResolveSubresource(g_pSceneDepthTex2DNonMS, 0, 
                g_pSceneDepthTex2D, 0, DXGI_FORMAT_R32_FLOAT);
        }

        // Render the fluid
        if(g_bRenderFluid)
            g_renderer->Draw( g_fluid->getCurrentShaderResourceView());

    }



    //-------------------------------------------------------------------------------------
    // HUD
    //-------------------------------------------------------------------------------------
    pd3dDevice->RSSetViewports(1,&rtViewport);
    pd3dDevice->OMSetRenderTargets( 1, &pRTV , pDSV ); 
    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, TEXT("HUD / Stats") );
    if( g_bDrawUI )
    {
        RenderText();
        g_SampleUI.OnRender( fElapsedTime ); 
        g_HUD.OnRender( fElapsedTime );
    }
    DXUT_EndPerfEvent();

    pd3dDevice->OMSetRenderTargets( 0, NULL, NULL );

}


void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 2, 0 );
    g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats(true) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );

    // Draw help
    if( g_bShowHelp )
    {
        UINT nBackBufferHeight = ( DXUTIsAppRenderingWithD3D9() ) ? DXUTGetD3D9BackBufferSurfaceDesc()->Height : DXUTGetDXGIBackBufferSurfaceDesc()->Height;
        g_pTxtHelper->SetInsertionPos( 2, nBackBufferHeight-30*6 );
        g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
        g_pTxtHelper->DrawTextLine( TEXT("Controls:") );

        g_pTxtHelper->SetInsertionPos( 20, nBackBufferHeight-30*5 );
        g_pTxtHelper->DrawTextLine( TEXT("Rotate camera:  Right mouse button\n")
                                    TEXT("Zoom camera:  Mouse wheel scroll\n")
                                    TEXT("Push fluid around: CTRL + Left mouse button\n")
                                    );

        g_pTxtHelper->SetInsertionPos( 20, nBackBufferHeight-30*2 );
        g_pTxtHelper->DrawTextLine( TEXT("Hide help: F1\n") 
                                    TEXT("Quit: ESC\n" ));
    }
    else
    {
        g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        g_pTxtHelper->DrawTextLine( TEXT("Press F1 for help") );
    }

    g_pTxtHelper->End();
}



//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10ReleasingSwapChain();
    g_Skybox.OnReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10DestroyDevice();
    g_D3DSettingsDlg.OnD3D10DestroyDevice();

    g_Skybox.OnDestroyDevice();
    SAFE_RELEASE(g_pEnvMapSRV);
    SAFE_RELEASE(g_pEnvMap);

    SAFE_RELEASE( g_pFont );
    SAFE_RELEASE( g_pSprite );
    SAFE_DELETE( g_pTxtHelper );


    SAFE_DELETE(g_skinnedMesh);

    SAFE_RELEASE(g_pSceneDepthTex2DNonMS);
    SAFE_RELEASE(g_pSceneDepthTex2D);
    SAFE_RELEASE(g_pSceneDepthRTV);
    SAFE_RELEASE(g_pSceneDepthSRV);


    SAFE_DELETE(g_fluid);
    SAFE_DELETE(g_voxelizer);
	SAFE_DELETE(g_renderer);
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    //pDeviceSettings->d3d10.SyncInterval = 0; //turn off vsync
    return true;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    if(!g_bEnableMouseInteraction) 
        g_Camera.FrameMove( fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass messages to settings dialog if its active
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    

    // Pass all remaining windows messages to camera so it can respond to user input
    if(!g_bEnableMouseInteraction) 
        g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    float moveStep = 1.0f / (g_gridWidth * g_fTimestep);

    switch(nChar) {
        case 'Y':
            g_obstaclePos.x += moveStep;
            g_obstaclePos.y += moveStep;
            if( g_obstaclePos.x > 1.0 ) g_obstaclePos.x = 1.0;
            if( g_obstaclePos.y > 1.0 ) g_obstaclePos.y = 1.0;
            g_fluid->SetObstaclePositionInNormalizedGrid( 
                g_obstaclePos.x, g_obstaclePos.y, g_obstaclePos.z);
            break;
        case 'H':
            g_obstaclePos.x -= moveStep;
            g_obstaclePos.y -= moveStep;
            if( g_obstaclePos.x < 0.0 ) g_obstaclePos.x = 0.0;
            if( g_obstaclePos.y < 0.0 ) g_obstaclePos.y = 0.0;
            g_fluid->SetObstaclePositionInNormalizedGrid( 
                g_obstaclePos.x, g_obstaclePos.y, g_obstaclePos.z);
            break;

        case VK_CONTROL:
            if( bKeyDown )
                g_bEnableMouseInteraction = true;
            else
                g_bEnableMouseInteraction = false;
            break;

        default:
            if( bKeyDown )
            {
                switch( nChar )
                {
                    case VK_F1: 
                         g_bShowHelp = !g_bShowHelp; 
                         break;
                    case VK_F8: 
                         g_bDrawUI = !g_bDrawUI; 
                         break;
                }
            }
        break;
    }
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, 
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, 
                       int x, int y, void* pUserContext )
{

    int z;

    if(!g_bEnableMouseInteraction)
        return;

    // Handle left-mouse click-and-drag input to create a temporary user-driven emitter
    if(bLeftButtonDown)
    {
        if( !leftIsPressed )
        {  
            leftIsPressed = true;
            justClicked = true;
        }
        else
        {
            if( !justClicked ) 
            {
               g_fluid->SetMouseDown(true);
            }

            x = x * g_gridWidth  / g_Width;
            y = (g_Height-y) * g_gridHeight / g_Height;
            z = last_z;
      
            if( last_mouse &&
                x >= 0 && x < g_gridWidth &&
                y >= 0 && y < g_gridHeight )
            {
               float dx, dy, dz, mag;

               dx = float( x - last_x );
               dy = float( y - last_y );
               dz = float( z - last_z );
               mag = sqrt( dx*dx + dy*dy + dz*dz );
               if( mag > 0.01f )
               {
                   dx /= 2.0f;
                   dy /= 2.0f;
                   dz /= 2.0f;
                  g_fluid->Impulse( x, z, y, dx, dz, dy );
               }
            }

            last_x = x;
            last_y = y;
            last_z = z;
            last_mouse = true;
            justClicked = false;
        }
    }
    else
    {
        leftIsPressed = false;
        last_mouse = false;
        g_fluid->SetMouseDown(false);
    }
}


void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{    
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() ); break;
        
        case IDC_CHANGEFLUIDTYPE:
        {
            // Get the new fluid type
            g_eFluidType = Fluid::FLUID_TYPE(g_SampleUI.GetComboBox( IDC_CHANGEFLUIDTYPE )->GetSelectedIndex());
            g_SampleUI.GetComboBox( IDC_CHANGEFLUIDTYPE )->SetSelectedByIndex( g_eFluidType );

            g_bReinitializeFluid = true;
            //set all variables to preset 1
            resetVariablesToPreset1();

            break;
        }

        case IDC_CHANGERENDER:
        {
            // Get the new RT format
            g_currentRenderChoice = g_SampleUI.GetComboBox( IDC_CHANGERENDER )->GetSelectedIndex();
            g_SampleUI.GetComboBox( IDC_CHANGERENDER )->SetSelectedByIndex( g_currentRenderChoice );
            break;
        }

        case IDC_PAUSESIMULATION:       g_bPause = g_SampleUI.GetCheckBox( IDC_PAUSESIMULATION )->GetChecked(); break;
        case IDC_PAUSEANIMATION:   
        {
            g_bPauseAnimation = g_SampleUI.GetCheckBox( IDC_PAUSEANIMATION )->GetChecked(); 
            if(!g_bPauseAnimation)
                // re-enable streamout
                g_bStreamoutSkinnedMesh = true;
            else 
                g_numWrittenSOBuffersSincePause = 0;
           break;
        }

        case IDC_RENDERFLUID:       g_bRenderFluid = g_SampleUI.GetCheckBox( IDC_RENDERFLUID )->GetChecked(); break;
        case IDC_RENDERMODEL:       g_bRenderMesh = g_SampleUI.GetCheckBox( IDC_RENDERMODEL )->GetChecked(); break;
        case IDC_RENDERGLOW:        g_bRenderGlow = g_SampleUI.GetCheckBox( IDC_RENDERGLOW )->GetChecked(); break;

        case IDC_DO_INOUT_VOXELIZATION: 
        {
            if( g_voxelizer )
                g_voxelizer->SetEnableInOutVoxelize( g_SampleUI.GetCheckBox( IDC_DO_INOUT_VOXELIZATION )->GetChecked() ); 
            break;
        }
        case IDC_DO_VEL_VOXELIZATION: 
        {
            if( g_voxelizer )
                g_voxelizer->SetEnableVelocityVoxelize( g_SampleUI.GetCheckBox( IDC_DO_VEL_VOXELIZATION )->GetChecked() ); 
            break;
        }

        case IDC_USEMACCORMACK:     g_fluid->SetUseMACCORMACK(g_SampleUI.GetCheckBox( IDC_USEMACCORMACK )->GetChecked()); break;
        case IDC_GRAVITY:           g_fluid->SetEnableGravity(g_SampleUI.GetCheckBox( IDC_GRAVITY )->GetChecked()); break;
        case IDC_WATER_STREAM :     g_fluid->SetEnableLiquidStream(g_SampleUI.GetCheckBox( IDC_WATER_STREAM)->GetChecked()); break;

        case IDC_TOGGLEPRESET1:
        {   
            //set all variables to preset 1
            resetVariablesToPreset1();             
            //redraw the controls
            resetGUIControls();
            break;
        }

        case IDC_GRIDWIDTH_SCALE:
        {
            TCHAR sz[100];
            g_gridWidth = g_SampleUI.GetSlider( IDC_GRIDWIDTH_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Grid Width: %0.2f"), float(g_gridWidth) ); 
            g_SampleUI.GetStatic( IDC_GRIDWIDTH_STATIC )->SetText( sz );
            break;
        }
        case IDC_GRIDHEIGHT_SCALE:
        {
            TCHAR sz[100];
            g_gridHeight = g_SampleUI.GetSlider( IDC_GRIDHEIGHT_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Grid Height: %0.2f"), float(g_gridHeight) ); 
            g_SampleUI.GetStatic( IDC_GRIDHEIGHT_STATIC )->SetText( sz );
            break;
        }
        case IDC_GRIDDEPTH_SCALE:
        {
            TCHAR sz[100];
            g_gridDepth = g_SampleUI.GetSlider( IDC_GRIDDEPTH_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Grid Depth: %0.2f"), float(g_gridDepth) ); 
            g_SampleUI.GetStatic( IDC_GRIDDEPTH_STATIC )->SetText( sz );
            break;
        }
        case IDC_GRIDAPPLY:
        {   
            g_bReinitializeFluid = true;
            break;
        }

        case IDC_NUMJACOBI_SCALE:
        {
            TCHAR sz[100];
            g_numJacobi = g_SampleUI.GetSlider( IDC_NUMJACOBI_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Jacobi Iterations: %0.2f"), float(g_numJacobi) );
            g_SampleUI.GetStatic( IDC_NUMJACOBI_STATIC )->SetText( sz );
            break;
        }

        case IDC_SMOKEALPHA_SCALE:
        {
            TCHAR sz[100];
            g_smokeAlphaMultiplier = g_SampleUI.GetSlider( IDC_SMOKEALPHA_SCALE )->GetValue()/100.0f;
            StringCchPrintf( sz, 100, TEXT("Smoke Alpha Scale: %0.2f"), float(g_smokeAlphaMultiplier) ); 
            g_SampleUI.GetStatic( IDC_SMOKEALPHA_STATIC )->SetText( sz );
            break;
        }
        case IDC_GLOWCONTRIBUTION_SCALE:
        {
            TCHAR sz[100];
            g_glowContribution = g_SampleUI.GetSlider( IDC_GLOWCONTRIBUTION_SCALE )->GetValue()/100.0f;
            StringCchPrintf( sz, 100, TEXT("Glow Contribution: %0.2f"), float(g_glowContribution) ); 
            g_SampleUI.GetStatic( IDC_GLOWCONTRIBUTION_STATIC )->SetText( sz );
            break;
        }
        case IDC_FINALINTENSITY_SCALE:
        {
            TCHAR sz[100];
            g_finalIntensityScale = (float) g_SampleUI.GetSlider( IDC_FINALINTENSITY_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Final Intensity Scale: %0.2f"), float(g_finalIntensityScale) ); 
            g_SampleUI.GetStatic( IDC_FINALINTENSITY_STATIC )->SetText( sz );
            break;
        }
        case IDC_FINALALPHA_SCALE:
        {
            TCHAR sz[100];
            g_finalAlphaScale = g_SampleUI.GetSlider( IDC_FINALALPHA_SCALE )->GetValue()/100.0f;
            StringCchPrintf( sz, 100, TEXT("Final Alpha Scale: %0.2f"), float(g_finalAlphaScale) ); 
            g_SampleUI.GetStatic( IDC_FINALALPHA_STATIC )->SetText( sz );
            break;
        }
        case IDC_SMOKECOLOR_SCALE:
        {
            TCHAR sz[100];
            g_smokeColorMultiplier = g_SampleUI.GetSlider( IDC_SMOKECOLOR_SCALE )->GetValue()/100.0f;
            StringCchPrintf( sz, 100, TEXT("Smoke Color Scale: %0.2f"), float(g_smokeColorMultiplier) ); 
            g_SampleUI.GetStatic( IDC_SMOKECOLOR_STATIC )->SetText( sz );
            break;
        }

        case IDC_REDNESS_SCALE:
        {
            TCHAR sz[100];
            g_RednessFactor = g_SampleUI.GetSlider( IDC_REDNESS_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Redness Scale: %0.2f"), float(g_RednessFactor) ); 
            g_SampleUI.GetStatic( IDC_REDNESS_STATIC )->SetText( sz );
            break;
        }
        case IDC_XYVELOCITY_SCALE:
        {
            TCHAR sz[100];
            g_xyVelocityScale = (float) g_SampleUI.GetSlider( IDC_XYVELOCITY_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("XY Velocity Scale: %0.2f"), float(g_xyVelocityScale) ); 
            g_SampleUI.GetStatic( IDC_XYVELOCITY_STATIC )->SetText( sz );
            break;
        }
        case IDC_ZVELOCITY_SCALE:
        {
            TCHAR sz[100];
            g_zVelocityScale = (float) g_SampleUI.GetSlider( IDC_ZVELOCITY_SCALE )->GetValue();
            StringCchPrintf( sz, 100, TEXT("Z Velocity Scale: %0.2f"), float(g_zVelocityScale) ); 
            g_SampleUI.GetStatic( IDC_ZVELOCITY_STATIC )->SetText( sz );
            break;
        }
    }
}


HRESULT loadTextureFromFile(ID3D10Device* pD3DDevice, LPCTSTR pFileName, ID3D10Effect* pEffect, LPCTSTR pEffectPath, LPCSTR pShaderTextureVarName)
{
    HRESULT hr(S_OK);

    TCHAR str[MAX_PATH];
    V_RETURN(NVUTFindDXSDKMediaFileCchT(str, MAX_PATH, pFileName));

    D3DX10_IMAGE_INFO SrcInfo;
    hr = D3DX10GetImageInfoFromFile(str, NULL, &SrcInfo, &hr);

    D3DX10_IMAGE_LOAD_INFO texLoadInfo;
    texLoadInfo.Width          = SrcInfo.Width;
    texLoadInfo.Height         = SrcInfo.Height;
    texLoadInfo.Depth          = SrcInfo.Depth;
    texLoadInfo.FirstMipLevel  = 0;
    texLoadInfo.MipLevels      = SrcInfo.MipLevels;
    texLoadInfo.Usage          = D3D10_USAGE_DEFAULT;
    texLoadInfo.BindFlags      = D3D10_BIND_SHADER_RESOURCE;
    texLoadInfo.CpuAccessFlags = 0;
    texLoadInfo.MiscFlags      = SrcInfo.MiscFlags;
    texLoadInfo.Format         = SrcInfo.Format;
    texLoadInfo.Filter         = D3DX10_FILTER_TRIANGLE;
    texLoadInfo.MipFilter      = D3DX10_FILTER_TRIANGLE;
    texLoadInfo.pSrcInfo       = &SrcInfo;
    ID3D10Resource *pRes = NULL;

    V_RETURN( D3DX10CreateTextureFromFile(pD3DDevice, str, &texLoadInfo, NULL, &pRes, &hr ) );
    if( pRes )
    {
        ID3D10Texture3D* texture;


        pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&texture );
        D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
        SRVDesc.Format = texLoadInfo.Format;
        SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MostDetailedMip = 0;
        SRVDesc.Texture2D.MipLevels = texLoadInfo.MipLevels;

        ID3D10ShaderResourceView* textureRview(NULL);
        V_RETURN (pD3DDevice->CreateShaderResourceView( texture, &SRVDesc, &textureRview));
        
        ID3D10EffectShaderResourceVariable* textureRVar(NULL);
        V_GET_VARIABLE_RET(pEffect, pEffectPath, textureRVar, pShaderTextureVarName, AsShaderResource);
        
        textureRVar->SetResource( textureRview );

        SAFE_RELEASE( texture );
        SAFE_RELEASE(textureRview);
     }

    SAFE_RELEASE(pRes);
    return S_OK;
}

void resetVariablesToPreset1()
{
    g_glowContribution     = 0.81f;
    g_finalIntensityScale  = 28.0f;
    g_finalAlphaScale      = 0.95f;
    g_smokeColorMultiplier = 2.0f;
    g_smokeAlphaMultiplier = 0.05f;
    g_RednessFactor        = 5;
    g_zVelocityScale       = 4.8f;
    g_xyVelocityScale      = 4.0f;

}
