// texturewood.cpp
//
// This program uses the libnoise library to generate texture maps consisting
// of stained oak-like wood.
//
// Copyright (C) 2004 Jason Bevins
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// (COPYING.txt) for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include <noise/noise.h>

#include "noiseutils.h"

using namespace noise;

// Height of the texture.
const int TEXTURE_HEIGHT = 256;

// Creates the color gradients for the texture.
void CreateTextureColor (utils::RendererImage& renderer);

// Given a noise module, this function renders a flat square texture map and
// writes it to a Windows bitmap (*.bmp) file.  Because the texture map is
// square, its width is equal to its height.  The texture map can be seamless
// (tileable) or non-seamless.
void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename);

// Given a noise module, this function renders a spherical texture map and
// writes it to a Windows bitmap (*.bmp) file.  The texture map's width is
// double its height.
void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename);

// Given a noise map, this function renders a texture map and writes it to a
// Windows bitmap (*.bmp) file.
void RenderTexture (const utils::NoiseMap& noiseMap, const char* filename);



/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

TextureID texture=-1;

void CreateILTexture(ubyte *data, unsigned int width, unsigned int height, unsigned int bpp, TextureID& id)
{
	Image3 img;
	ubyte* data_=img.create(FORMAT_RGBA8, width, height, 1, 1);
	stx_memcpy(data_, data, width*height*bpp*sizeof(ubyte));
	img.swap(0, 2);
	id=IRenderer::GetRendererInstance()->addTexture(&img, false, IRenderer::GetRendererInstance()->Getlinear());
}

int CreateTexture()
{
  // Base wood texture.  The base texture uses concentric cylinders aligned
  // on the z axis, like a log.
  module::Cylinders baseWood;
  baseWood.SetFrequency (16.0);

  // Perlin noise to use for the wood grain.
  module::Perlin woodGrainNoise;
  woodGrainNoise.SetSeed (0);
  woodGrainNoise.SetFrequency (48.0);
  woodGrainNoise.SetPersistence (0.5);
  woodGrainNoise.SetLacunarity (2.20703125);
  woodGrainNoise.SetOctaveCount (3);
  woodGrainNoise.SetNoiseQuality (QUALITY_STD);

  // Stretch the Perlin noise in the same direction as the center of the
  // log.  This produces a nice wood-grain texture.
  module::ScalePoint scaledBaseWoodGrain;
  scaledBaseWoodGrain.SetSourceModule (0, woodGrainNoise);
  scaledBaseWoodGrain.SetYScale (0.25);

  // Scale the wood-grain values so that they may be added to the base wood
  // texture.
  module::ScaleBias woodGrain;
  woodGrain.SetSourceModule (0, scaledBaseWoodGrain);
  woodGrain.SetScale (0.25);
  woodGrain.SetBias (0.125);

  // Add the wood grain texture to the base wood texture.
  module::Add combinedWood;
  combinedWood.SetSourceModule (0, baseWood);
  combinedWood.SetSourceModule (1, woodGrain);

  // Slightly perturb the wood texture for more realism.
  module::Turbulence perturbedWood;
  perturbedWood.SetSourceModule (0, combinedWood);
  perturbedWood.SetSeed (1);
  perturbedWood.SetFrequency (4.0);
  perturbedWood.SetPower (1.0 / 256.0);
  perturbedWood.SetRoughness (4);

  // Cut the wood texture a small distance from the center of the "log".
  module::TranslatePoint translatedWood;
  translatedWood.SetSourceModule (0, perturbedWood);
  translatedWood.SetZTranslation (1.48);

  // Cut the wood texture on an angle to produce a more interesting wood
  // texture.
  module::RotatePoint rotatedWood;
  rotatedWood.SetSourceModule (0, translatedWood);
  rotatedWood.SetAngles (84.0, 0.0, 0.0);

  // Finally, perturb the wood texture to produce the final texture.
  module::Turbulence finalWood;
  finalWood.SetSourceModule (0, rotatedWood);
  finalWood.SetSeed (2);
  finalWood.SetFrequency (2.0);
  finalWood.SetPower (1.0 / 64.0);
  finalWood.SetRoughness (4);

  // Given the wood noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalWood, false, TEXTURE_HEIGHT,
    "textureplane.bmp");
  CreatePlanarTexture (finalWood, true, TEXTURE_HEIGHT,
    "textureseamless.bmp");
  CreateSphericalTexture (finalWood, TEXTURE_HEIGHT,
    "texturesphere.bmp");

  return 0;
}

void CreateTextureColor (utils::RendererImage& renderer)
{
  // Create a dark-stained wood palette (oak?)
  renderer.ClearGradient ();
  renderer.AddGradientPoint (-1.00, utils::Color (189, 94, 4, 255));
  renderer.AddGradientPoint ( 0.50, utils::Color (144, 48, 6, 255));
  renderer.AddGradientPoint ( 1.00, utils::Color ( 60, 10, 8, 255));
}

void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename)
{
  // Map the output values from the noise module onto a plane.  This will
  // create a two-dimensional noise map which can be rendered as a flat
  // texture map.
  utils::NoiseMapBuilderPlane plane;
  utils::NoiseMap noiseMap;
  plane.SetBounds (-1.0, 1.0, -1.0, 1.0);
  plane.SetDestSize (height, height);
  plane.SetSourceModule (noiseModule);
  plane.SetDestNoiseMap (noiseMap);
  plane.EnableSeamless (seamless);
  plane.Build ();

  RenderTexture (noiseMap, filename);
}

void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename)
{
  // Map the output values from the noise module onto a sphere.  This will
  // create a two-dimensional noise map which can be rendered as a spherical
  // texture map.
  utils::NoiseMapBuilderSphere sphere;
  utils::NoiseMap noiseMap;
  sphere.SetBounds (-90.0, 90.0, -180.0, 180.0); // degrees
  sphere.SetDestSize (height * 2, height);
  sphere.SetSourceModule (noiseModule);
  sphere.SetDestNoiseMap (noiseMap);
  sphere.Build ();

  RenderTexture (noiseMap, filename);
}

void RenderTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateTextureColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (false);

  // Render the texture.
  textureRenderer.Render ();

#if 1
  int width  = destTexture.GetWidth  ();
  int height = destTexture.GetHeight ();
  int bpp = 4;// ??? CalcWidthByteCount (width)/width;
	ubyte *data=(ubyte *)destTexture.GetSlabPtr (0);
	CreateILTexture(data, width, height, bpp, texture);
#else
  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
#endif
}

ShaderID shd=-1;
VertexFormatID vf=-1;

int init(const char* aTitle)
{
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/libnoise/rendersample.shd", "main", "main");
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	SamplerStateID ss=IRenderer::GetRendererInstance()->Getlinear();
	//texture=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, ss);
	CreateTexture();
	return 0;
}

void render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();
	//		   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setTexture("Base", texture);
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("texturewood");	
	IInput*    i=STX_Service::GetInputInstance();
	{
	float f=128.0f/256.0f;
	D3DXFROMWINEVECTOR4 col(f, f, f, 1.0f);
	IRenderer::GetRendererInstance()->Clear(true, true, col);
	IRenderer::GetRendererInstance()->BeginScene();

		const char* txt = "Initializing. Please wait a moment.";
		IRenderer::GetRendererInstance()->drawText(txt, 250, 250, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
	}
	init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		render();
	}
	return 0;
}
