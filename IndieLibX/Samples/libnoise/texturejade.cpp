// texturejade.cpp
//
// This program uses the libnoise library to generate texture maps consisting
// of African jade.
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
  // Primary jade texture.  The ridges from the ridged-multifractal module
  // produces the veins.
  module::RidgedMulti primaryJade;
  primaryJade.SetSeed (0);
  primaryJade.SetFrequency (2.0);
  primaryJade.SetLacunarity (2.20703125);
  primaryJade.SetOctaveCount (6);
  primaryJade.SetNoiseQuality (QUALITY_STD);

  // Base of the secondary jade texture.  The base texture uses concentric
  // cylinders aligned on the z axis, which will eventually be perturbed.
  module::Cylinders baseSecondaryJade;
  baseSecondaryJade.SetFrequency (2.0);

  // Rotate the base secondary jade texture so that the cylinders are not
  // aligned with any axis.  This produces more variation in the secondary
  // jade texture since the texture is parallel to the y-axis.
  module::RotatePoint rotatedBaseSecondaryJade;
  rotatedBaseSecondaryJade.SetSourceModule (0, baseSecondaryJade);
  rotatedBaseSecondaryJade.SetAngles (90.0, 25.0, 5.0);

  // Slightly perturb the secondary jade texture for more realism.
  module::Turbulence perturbedBaseSecondaryJade;
  perturbedBaseSecondaryJade.SetSourceModule (0, rotatedBaseSecondaryJade);
  perturbedBaseSecondaryJade.SetSeed (1);
  perturbedBaseSecondaryJade.SetFrequency (4.0);
  perturbedBaseSecondaryJade.SetPower (1.0 / 4.0);
  perturbedBaseSecondaryJade.SetRoughness (4);

  // Scale the secondary jade texture so it contributes a small part to the
  // final jade texture.
  module::ScaleBias secondaryJade;
  secondaryJade.SetSourceModule (0, perturbedBaseSecondaryJade);
  secondaryJade.SetScale (0.25);
  secondaryJade.SetBias (0.0);

  // Add the two jade textures together.  These two textures were produced
  // using different combinations of coherent noise, so the final texture will
  // have a lot of variation.
  module::Add combinedJade;
  combinedJade.SetSourceModule (0, primaryJade);
  combinedJade.SetSourceModule (1, secondaryJade);

  // Finally, perturb the combined jade textures to produce the final jade
  // texture.  A low roughness produces nice veins.
  module::Turbulence finalJade;
  finalJade.SetSourceModule (0, combinedJade);
  finalJade.SetSeed (2);
  finalJade.SetFrequency (4.0);
  finalJade.SetPower (1.0 / 16.0);
  finalJade.SetRoughness (2);

  // Given the jade noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalJade, false, TEXTURE_HEIGHT,
    "textureplane.bmp");
  CreatePlanarTexture (finalJade, true, TEXTURE_HEIGHT,
    "textureseamless.bmp");
  CreateSphericalTexture (finalJade, TEXTURE_HEIGHT,
    "texturesphere.bmp");

  return 0;
}

void CreateTextureColor (utils::RendererImage& renderer)
{
  // Create a nice jade palette.
  renderer.ClearGradient ();
  renderer.AddGradientPoint (-1.000, utils::Color ( 24, 146, 102, 255));
  renderer.AddGradientPoint ( 0.000, utils::Color ( 78, 154, 115, 255));
  renderer.AddGradientPoint ( 0.250, utils::Color (128, 204, 165, 255));
  renderer.AddGradientPoint ( 0.375, utils::Color ( 78, 154, 115, 255));
  renderer.AddGradientPoint ( 1.000, utils::Color ( 29, 135, 102, 255));
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
	IRenderer* r=IRenderer::GetRendererInstance("texturejade");	
	IInput*    i=STX_Service::GetInputInstance();
	{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
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
