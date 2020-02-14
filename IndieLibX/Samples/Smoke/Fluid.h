//----------------------------------------------------------------------------------
// File:   Fluid.h
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

#ifndef FLUID3D_FLUID_H
#define FLUID3D_FLUID_H

#pragma warning(disable:4995)
#include <fstream>
#include "VolumeRenderer.h"
#include "Grid.h"

class Fluid
{
public:
    enum RENDER_TARGET
    {
        RENDER_TARGET_VELOCITY0,
        RENDER_TARGET_VELOCITY1,
        RENDER_TARGET_PRESSURE,
        RENDER_TARGET_SCALAR0,
        RENDER_TARGET_SCALAR1,
        RENDER_TARGET_OBSTACLES,
        RENDER_TARGET_OBSTVELOCITY,
        RENDER_TARGET_TEMPSCALAR,
        RENDER_TARGET_TEMPVECTOR,
        RENDER_TARGET_TEMPVECTOR1,
        NUM_RENDER_TARGETS
    };

    enum FLUID_TYPE
    {
        FT_SMOKE =0,
        FT_FIRE  =1,
        FT_LIQUID=2
    };

    Fluid                   ( ID3D10Device* pd3dDevice );
    virtual ~Fluid          ( void );

    HRESULT Initialize      ( int width, int height, int depth, FLUID_TYPE fluidType );

    void Update             ( float timestep, bool bUseMACCORMACKVelocity=true, bool bUseSuppliedParameters = false, float fInputVorticityConfinementScale=0, float fInputDensityDecay=0.9999f, float fInputDensityViscosity=0, float fInputFluidViscosity=0,float fInputImpulseSize=0, float randCenterScale = 1.0);



    void Render3D           ( void );
    HRESULT Draw            ( int field );
    void Impulse            ( int x, int y, int z, float dX, float dY, float dZ );
    void Reset              ( void );
    int GetTextureWidth     ( void );
    int GetTextureHeight    ( void );
    int GetGridCols         ( void );
    int GetGridRows         ( void );
    ID3D10Texture3D* Get3DTexture(RENDER_TARGET rt) {return m_p3DTextures[rt];};
    void SetObstaclePositionInNormalizedGrid( float x, float y, float z );

    void SetUseMACCORMACK       (bool b)    { m_bUseMACCORMACK = b; }
    void SetNumJacobiIterations (int i)     { m_nJacobiIterations = i;    }
    void SetMouseDown           (bool b)    { m_bMouseDown = b;     }
    void SetEnableGravity       (bool b)    { m_bGravity = b;       }
    void SetEnableLiquidStream  (bool b)    { m_bLiquidStream = b;   }

    bool GetUseMACCORMACK       ( void )    { return m_bUseMACCORMACK;  }
    int  GetNumJacobiIterations ( void )    { return m_nJacobiIterations;     }
    bool GetMouseDown           ( void )    { return m_bMouseDown;      }
    bool GetEnableGravity       ( void )    { return m_bGravity;        }
    bool GetEnableLiquidStream  ( void )    { return m_bLiquidStream;    }
	int GetDimX                 ( void )    { if(!m_pGrid) return 0; return m_pGrid->GetDimX(); };
	int GetDimY                 ( void )    { if(!m_pGrid) return 0; return m_pGrid->GetDimY(); };
	int GetDimZ                 ( void )    { if(!m_pGrid) return 0; return m_pGrid->GetDimZ(); };

	DXGI_FORMAT GetFormat       ( RENDER_TARGET rt ) {return m_RenderTargetFormats[rt];};  
	ID3D10ShaderResourceView* getCurrentShaderResourceView();
	void drawGridSlices         ( void )    {m_pGrid->DrawSlices(); };


protected:
    Fluid                         ( void ) {};

    HRESULT LoadShaders           ( void );

    void DrawObstacleTestBox      ( RENDER_TARGET dstObst, RENDER_TARGET dstObstVel );
    void DrawObstacleBoundaryBox( RENDER_TARGET dstObst, RENDER_TARGET dstObstVel );
    void AdvectMACCORMACK         ( float timestep, float decay, bool bAsLiquidVelocity, bool bAsTemperature, RENDER_TARGET dstPhi, RENDER_TARGET srcVel, RENDER_TARGET srcPhi, RENDER_TARGET srcPhi_hat, RENDER_TARGET srcObst, RENDER_TARGET srcLevelSet );
    void Advect                   ( float timestep, float decay, bool bAsLiquidVelocity, bool bAsTemperature, RENDER_TARGET dstPhi, RENDER_TARGET srcVel, RENDER_TARGET srcPhi, RENDER_TARGET srcObst, RENDER_TARGET srcLevelSet );
    void Diffuse                  ( float timestep, float viscosity, RENDER_TARGET dstPhi, RENDER_TARGET srcPhi, RENDER_TARGET tmpPhi, RENDER_TARGET srcObst );
    void ComputeVorticity         ( RENDER_TARGET dstVorticity, RENDER_TARGET srcVel );
    void ApplyVorticityConfinement( float timestep, RENDER_TARGET dstVel, RENDER_TARGET srcVorticity, RENDER_TARGET srcObst, RENDER_TARGET srcLevelSet );
    void AddNewMatter             ( float timestep, RENDER_TARGET dstPhi, RENDER_TARGET srcObst );
    void AddExternalForces        ( float timestep, RENDER_TARGET dstVel, RENDER_TARGET srcObst, RENDER_TARGET srcLevelSet, float impulseSize, float randCenterScale );
    void ComputeVelocityDivergence( RENDER_TARGET dstDivergence, RENDER_TARGET srcVel, RENDER_TARGET srcObst, RENDER_TARGET srcObstVel );
    void ComputePressure          ( RENDER_TARGET dstAndSrcPressure, RENDER_TARGET tmpPressure, RENDER_TARGET srcDivergence, RENDER_TARGET srcObst, RENDER_TARGET srcLevelSet );
    void ProjectVelocity          ( RENDER_TARGET dstVel, RENDER_TARGET srcPressure, RENDER_TARGET srcVel, RENDER_TARGET srcObst, RENDER_TARGET srcObstVel, RENDER_TARGET srcLevelSet );
    void RedistanceLevelSet       ( RENDER_TARGET dstLevelSet, RENDER_TARGET srcLevelSet, RENDER_TARGET tmpLevelSet );
    void ExtrapolateVelocity      ( RENDER_TARGET dstAndSrcVel, RENDER_TARGET tmpVel, RENDER_TARGET srcObst, RENDER_TARGET srcLeveSet );

    //D3D10 helper functions
    HRESULT CreateTextureAndViews(int rtIndex, D3D10_TEXTURE3D_DESC TexDesc);
    void SetRenderTarget(RENDER_TARGET rtIndex, ID3D10DepthStencilView * optionalDSV = NULL );


    // Internal State
    //===============
   
    // Grid is used to draw quads for all the slices in the 3D simulation grid
    Grid                                *m_pGrid;
    

    // D3D10 device
    ID3D10Device                        *m_pD3DDevice;

    // Textures, rendertarget views and shader resource views
    ID3D10Texture3D                     *m_p3DTextures                [ NUM_RENDER_TARGETS ];
    ID3D10ShaderResourceView            *m_pShaderResourceViews       [ NUM_RENDER_TARGETS ];
    ID3D10RenderTargetView              *m_pRenderTargetViews         [ NUM_RENDER_TARGETS ];
    DXGI_FORMAT                         m_RenderTargetFormats         [ NUM_RENDER_TARGETS ];

    // Effect (simulation shaders)
    TCHAR                               m_effectPath[MAX_PATH];
    ID3D10Effect                        *m_pEffect;

    // Effect Techniques
    ID3D10EffectTechnique               *m_etAdvect;
    ID3D10EffectTechnique               *m_etAdvectMACCORMACK;
    ID3D10EffectTechnique               *m_etVorticity;
    ID3D10EffectTechnique               *m_etConfinement;
    ID3D10EffectTechnique               *m_etDiffuse;
    ID3D10EffectTechnique               *m_etDivergence;
    ID3D10EffectTechnique               *m_etScalarJacobi;
    ID3D10EffectTechnique               *m_etProject;

    ID3D10EffectTechnique               *m_etInitLevelSetToLiquidHeight;
    ID3D10EffectTechnique               *m_etInjectLiquid;
    ID3D10EffectTechnique               *m_etAirPressure;
    ID3D10EffectTechnique               *m_etRedistance;
    ID3D10EffectTechnique               *m_etExtrapolateVelocity;

    ID3D10EffectTechnique               *m_etLiquidStream_LevelSet;
    ID3D10EffectTechnique               *m_etLiquidStream_Velocity;
    ID3D10EffectTechnique               *m_etGravity;

    ID3D10EffectTechnique               *m_etGaussian;
    ID3D10EffectTechnique               *m_etCopyTextureDensity;
    ID3D10EffectTechnique               *m_etAddDensityDerivativeVelocity;

    ID3D10EffectTechnique               *m_etStaticObstacleTriangles;
    ID3D10EffectTechnique               *m_etStaticObstacleLines;
    ID3D10EffectTechnique               *m_etDrawBox;

    ID3D10EffectTechnique               *m_etDrawTexture;


    // Shader variables
    ID3D10EffectScalarVariable          *m_evTextureWidth;
    ID3D10EffectScalarVariable          *m_evTextureHeight;
    ID3D10EffectScalarVariable          *m_evTextureDepth;
    ID3D10EffectScalarVariable          *m_evDrawTexture;
    ID3D10EffectScalarVariable          *m_evDecay;
    ID3D10EffectScalarVariable          *m_evViscosity;
    ID3D10EffectScalarVariable          *m_evRadius;
    ID3D10EffectVectorVariable          *m_evCenter;
    ID3D10EffectVectorVariable          *m_evColor;
    ID3D10EffectVectorVariable          *m_evGravity;
    ID3D10EffectScalarVariable          *m_evVortConfinementScale;
    ID3D10EffectScalarVariable          *m_evTimeStep;
    ID3D10EffectScalarVariable          *m_evAdvectAsTemperature;
    ID3D10EffectScalarVariable          *m_evTreatAsLiquidVelocity;
    ID3D10EffectScalarVariable          *m_evFluidType;
    ID3D10EffectVectorVariable          *m_evObstBoxLBDcorner;
    ID3D10EffectVectorVariable          *m_evObstBoxRTUcorner;
    ID3D10EffectVectorVariable          *m_evObstBoxVelocity;
    
    ID3D10EffectShaderResourceVariable  *m_evTexture_pressure;
    ID3D10EffectShaderResourceVariable  *m_evTexture_velocity;
    ID3D10EffectShaderResourceVariable  *m_evTexture_vorticity;
    ID3D10EffectShaderResourceVariable  *m_evTexture_divergence;
    ID3D10EffectShaderResourceVariable  *m_evTexture_phi;
    ID3D10EffectShaderResourceVariable  *m_evTexture_phi_hat;
    ID3D10EffectShaderResourceVariable  *m_evTexture_phi_next;
    ID3D10EffectShaderResourceVariable  *m_evTexture_levelset;
    ID3D10EffectShaderResourceVariable  *m_evTexture_obstacles;
    ID3D10EffectShaderResourceVariable  *m_evTexture_obstvelocity; 

    // Use this to ping-pong between two render-targets used to store the density or level set
    RENDER_TARGET                       m_currentDstScalar;
    RENDER_TARGET                       m_currentSrcScalar;


    // Simulation Parameters
    //======================
    FLUID_TYPE  m_eFluidType;
    bool        m_bMouseDown;
    bool        m_bTreatAsLiquidVelocity;
    bool        m_bGravity;
    bool        m_bUseMACCORMACK;
    int         m_nJacobiIterations;
    int         m_nDiffusionIterations;
    int         m_nVelExtrapolationIterations;
    int         m_nRedistancingIterations;
    float       m_fDensityDecay;        // smoke/fire decay rate (1.0 - dissipation_rate)
    float       m_fDensityViscosity;    // smoke/fire viscosity
    float       m_fFluidViscosity;      // viscosity of the fluid (for Diffusion applied to velocity field)
    float       m_fVorticityConfinementScale;    // vorticity confinement scale
    float       m_t;                    // local 'time' for procedural variation

    // Emitter parameters
    //=========================
    float       m_fSaturation;
    float       m_fImpulseSize;
    D3DXVECTOR4 m_vImpulsePos;
    D3DXVECTOR4 m_vImpulseDir;


    // Liquid stream
    //==============
    bool        m_bLiquidStream;
    float       m_fStreamSize;
    D3DXVECTOR4 m_streamCenter;


    // Obstacles
    //===================================
    // Simulation Domain Boundary
    bool        m_bClosedBoundaries;    // can be open or closed
    // ObstacleBox (for testing purposes)
    bool        m_bDrawObstacleBox;
    D3DXVECTOR4 m_vObstBoxPos;
    D3DXVECTOR4 m_vObstBoxPrevPos;
    D3DXVECTOR4 m_vObstBoxVelocity;
};

#endif

