/*
	Copyright (c) 2020 Tommi Roenty   http://www.tommironty.fi/
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

#pragma once

class SimpleRT 
{
public:
	TextureID* pTexture;
	TextureID* pRTV;
	TextureID* pSRV;

	SimpleRT( ID3D11Device* IRenderer::GetRendererInstance(), ID3D11DeviceContext* IRenderer::GetRendererInstance(), SURFACE_DESC* pTexDesc )
	{
		int hr;

		//These have to be set to have a render target
		pTexDesc->BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		pTexDesc->Usage = D3D11_USAGE_DEFAULT;
		pTexDesc->CPUAccessFlags = 0;

		( IRenderer::GetRendererInstance()->CreateTexture2D( pTexDesc, 0, &pTexture ) );
		( IRenderer::GetRendererInstance()->CreateShaderResourceView( pTexture, 0, &pSRV ) );
		( IRenderer::GetRendererInstance()->CreateRenderTargetView( pTexture, 0, &pRTV ) );
	}

	~SimpleRT()
	{
		//SAFE_RELEASE( pTexture );
		//SAFE_RELEASE( pRTV );
		//SAFE_RELEASE( pSRV );
	}

	operator TextureID*()
	{
		return pTexture;
	}

	operator TextureID*()
	{
		return pSRV;
	}

	operator TextureID*()
	{
		return pRTV;
	}
};

class SimpleArrayRT : public SimpleRT
{
public:
	TextureID** ppRTVs;
	unsigned int iArraySize;

	SimpleArrayRT( ID3D11Device* IRenderer::GetRendererInstance(), ID3D11DeviceContext* IRenderer::GetRendererInstance(), SURFACE_DESC* pTexDesc )
		: SimpleRT( IRenderer::GetRendererInstance(), IRenderer::GetRendererInstance(), pTexDesc )
	{
		int hr;

		SURFACE_DESC layerViewDesc;
		pRTV->GetDesc( &layerViewDesc );

		iArraySize = pTexDesc->ArraySize;
		ppRTVs = new TextureID*[iArraySize];
		switch (layerViewDesc.ViewDimension) {
			case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
				layerViewDesc.Texture2DArray.ArraySize = 1;
				for( unsigned int i = 0; i < iArraySize; ++i )
				{
					layerViewDesc.Texture2DArray.FirstArraySlice = i;
					( IRenderer::GetRendererInstance()->CreateRenderTargetView( pTexture, &layerViewDesc, &ppRTVs[i] ) );
				}
				break;
			case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
				layerViewDesc.Texture2DMSArray.ArraySize = 1;
				for( unsigned int i = 0; i < iArraySize; ++i )
				{
					layerViewDesc.Texture2DMSArray.FirstArraySlice = i;
					( IRenderer::GetRendererInstance()->CreateRenderTargetView( pTexture, &layerViewDesc, &ppRTVs[i] ) );
				}
				break;
		}
	}

	~SimpleArrayRT()
	{
		for (unsigned int i = 0; i < iArraySize; i++) {
			//SAFE_RELEASE( ppRTVs[i] );
		}
		SAFE_DELETE(ppRTVs);
	}
};

class DepthRT 
{
public:
	TextureID* pTexture;
	TextureID*	pDSV;
	TextureID* pSRV;

	DepthRT( ID3D11Device* IRenderer::GetRendererInstance(), SURFACE_DESC* pTexDesc )
	{
		int hr;
		( IRenderer::GetRendererInstance()->CreateTexture2D( pTexDesc, 0, &pTexture ) );

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.Flags = 0;
		( IRenderer::GetRendererInstance()->CreateDepthStencilView( pTexture, &dsvDesc, &pDSV ) );

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;
		( IRenderer::GetRendererInstance()->CreateShaderResourceView( pTexture, &srvDesc, &pSRV ) );
	}

	~DepthRT()
	{
		//SAFE_RELEASE( pTexture );
		//SAFE_RELEASE( pDSV );
		//SAFE_RELEASE( pSRV );
	}

	operator TextureID*()
	{
		return pTexture;
	}

	operator TextureID*()
	{
		return pDSV;
	}

	operator TextureID*()
	{
		return pSRV;
	}
};