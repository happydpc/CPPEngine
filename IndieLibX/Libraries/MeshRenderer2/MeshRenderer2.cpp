/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
#include "MeshRenderer2.h"

ShaderID MeshRenderer2::SimpleShader2=-1;
VertexFormatID MeshRenderer2::SimpleVertexDeclaration2=-1;

	ShaderID MeshRenderer2::ColorShader=-1;
	VertexFormatID MeshRenderer2::ColorVertexDeclaration=-1;

	ShaderID MeshRenderer2::_Shader=-1;
	VertexFormatID MeshRenderer2::_VertexDeclaration=-1;

	ShaderID MeshRenderer2::AmbientShader=-1;
	ShaderID MeshRenderer2::DiffuseShader=-1;
	ShaderID MeshRenderer2::PhongShader1=-1;
	ShaderID MeshRenderer2::PhongShader2=-1;
	VertexFormatID MeshRenderer2::AmbientVertexDeclaration=-1;
	VertexFormatID MeshRenderer2::DiffuseVertexDeclaration=-1;
	VertexFormatID MeshRenderer2::PhongVertexDeclaration1=-1;
	VertexFormatID MeshRenderer2::PhongVertexDeclaration2=-1;

	ShaderID MeshRenderer2::PhysicalBasedShader=-1;
        VertexFormatID MeshRenderer2::PhysicalBasedVertexDeclaration=-1;

	ShaderID MeshRenderer2::SimpleShader=-1;
	VertexFormatID MeshRenderer2::SimpleVertexDeclaration=-1;
	ShaderID MeshRenderer2::OutlineShader=-1;
	VertexFormatID MeshRenderer2::OutlineVertexDeclaration=-1;
	ShaderID MeshRenderer2::ShadingShader=-1;
	VertexFormatID MeshRenderer2::ShadingVertexDeclaration=-1;

	ShaderID MeshRenderer2::CelShader=-1;
	VertexFormatID MeshRenderer2::CelVertexDeclaration=-1;
	ShaderID MeshRenderer2::CelShader1=-1;
	VertexFormatID MeshRenderer2::CelVertexDeclaration1=-1;
	ShaderID MeshRenderer2::CelShader2=-1;
	VertexFormatID MeshRenderer2::CelVertexDeclaration2=-1;
	ShaderID MeshRenderer2::CelShader3=-1;
	VertexFormatID MeshRenderer2::CelVertexDeclaration3=-1;
	ShaderID MeshRenderer2::CelShader4=-1;
	VertexFormatID MeshRenderer2::CelVertexDeclaration4=-1;

	ShaderID MeshRenderer2::DirectionalShader=-1;
	VertexFormatID MeshRenderer2::DirectionalVertexDeclaration=-1;
	ShaderID MeshRenderer2::SpotShader=-1;
	VertexFormatID MeshRenderer2::SpotVertexDeclaration=-1;
	ShaderID MeshRenderer2::MultiShader=-1;
	VertexFormatID MeshRenderer2::MultiVertexDeclaration=-1;
	ShaderID MeshRenderer2::SnowShader=-1;
	VertexFormatID MeshRenderer2::SnowVertexDeclaration=-1;
	TextureID MeshRenderer2::ExposureDepthMapTexture=-1;
	TextureID MeshRenderer2::ShadingTexture=-1;
	TextureID MeshRenderer2::CelShade=-1;

/*
MeshRenderer2
{
	struct Animator
	{
		Animator():mRootNode(0), mScene(0){}
		void SetAnimIndex(unsigned int pAnimIndex){}
		unsigned int CurrentAnimIndex(){return 0;}
		int GetNumAnimations(){return 0;}
		void* mRootNode;
		void* mScene;
	};
	struct MeshHelper
	{
		MeshHelper(){}
		~MeshHelper(){}
		Animator* mAnimator;
		MeshHelper** apcMeshes;
	};
*/

int MeshRenderer2::InitShader(tShader aShader_)
{
	//if(Shader==-1)
	{

		tShader aShader=aShader_;
	std::string fn=stx_convertpath("/Framework3/Framework3.xml");
    TiXmlDocument doc(fn.c_str());
    if (!doc.LoadFile())
	{//LOG_PRINT("!doc.LoadFile(%s)\n", fn.c_str());
      stx_exit(0);}
    TiXmlHandle docHandle(&doc);
		TiXmlElement* Shader3DStrE = docHandle.FirstChild("Body").FirstChild("Shader3D").Element();
		if(Shader3DStrE){
    std::string Shader3DStr=Shader3DStrE->Attribute("Value");
	LOG_PRINT("Shader3DStr=%s\n", Shader3DStr.c_str());

			if(Shader3DStr=="SimpleShader"){aShader=eSimpleShader;}
			if(Shader3DStr=="AmbientLighting"){aShader=eAmbientLighting;}
			if(Shader3DStr=="DiffuseLighting"){aShader=eDiffuseLighting;}
			if(Shader3DStr=="PhongLighting1"){aShader=ePhongLighting1;}
			if(Shader3DStr=="PhongLighting2"){aShader=ePhongLighting2;}
			if(Shader3DStr=="DirectionaLighting"){aShader=eDirectionalLighting;}
			if(Shader3DStr=="SpotLighting"){aShader=eSpotLighting;}
			if(Shader3DStr=="MultiLighting"){aShader=eMultiLighting;}
			if(Shader3DStr=="SnowShader"){aShader=eSnowShader;}
			if(Shader3DStr=="CelShader"){aShader=eCelShader;}
			if(Shader3DStr=="CelShader1"){aShader=eCelShader1;}
			if(Shader3DStr=="CelShader2"){aShader=eCelShader2;}
			if(Shader3DStr=="CelShader3"){aShader=eCelShader3;}
			if(Shader3DStr=="CelShader4"){aShader=eCelShader4;}
}
		mShader=aShader;
		FormatDesc format[] = 	{
						0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
						0, TYPE_NORMAL,   FORMAT_FLOAT, 3,
						0, TYPE_BINORMAL, FORMAT_FLOAT, 3,
						0, TYPE_TANGENT,  FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
					};
		FormatDesc formatC[] = 	{
						0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 3,
						0, TYPE_BINORMAL, FORMAT_FLOAT, 3,
						0, TYPE_TANGENT,  FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
					};

		const char* mainVS="main";
		const char* mainFS="main";
		if(aShader==eSimpleShader2)
			mainFS="main2";

		m_v4AmbientColor=D3DXFROMWINEVECTOR4(0.1f, 0.0f, 0.0f, 1.0f);
		m_v4DiffuseColor=D3DXFROMWINEVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		m_v4LightDir=D3DXFROMWINEVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
		m_v4EyeDir=D3DXFROMWINEVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

		FormatDesc lineformat[] = 	{
						0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
						0, TYPE_NORMAL,   FORMAT_FLOAT, 3,
						0, TYPE_TEXCOORD, FORMAT_FLOAT, 3
					};

		if((aShader==eCelShader)||(aShader==eCelShader3))
		{
			if(CelShade==-1)
				CelShade=IRenderer::GetRendererInstance()->addImageLibTexture("/CelShading/celshade.png", false, IRenderer::GetRendererInstance()->Getlinear());
		}

		switch(aShader)
		{
			case eColorShader:
				if(ColorShader==-1)
				{
					ColorShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/ColorShader.HLSL.shd", mainVS, mainFS);
					ColorVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(formatC, elementsOf(formatC), ColorShader);
				}
				break;
			case eSimpleShader:
				if(SimpleShader==-1)
				{
					SimpleShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/rest.HLSL.shd", mainVS, mainFS);
					SimpleVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), SimpleShader);
				}
				break;
			case eAmbientLighting:
				if(AmbientShader==-1)
				{
					AmbientShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/AmbientLighting.HLSL.shd", mainVS, mainFS);
					AmbientVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), AmbientShader);
				}
				break;
			case eDiffuseLighting:
				if(DiffuseShader==-1)
				{
					DiffuseShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/DiffuseLighting.HLSL.shd", mainVS, mainFS);
					DiffuseVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), DiffuseShader);
				}
				break;
            case e_Lighting:
					Shader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/_Lighting.HLSL.shd", mainVS, mainFS);
					VertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), Shader);
				break;
			case ePhongLighting1:
				if(PhongShader1==-1)
				{
					////{//LOG_FNLN;}
					#if 0//def LINUX
					PhongShader1 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/PhongLighting1.GLSL.shd", mainVS, mainFS);
					#else
					PhongShader1 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/PhongLighting1.HLSL.shd", mainVS, mainFS);
					#endif
					PhongVertexDeclaration1 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), PhongShader1);
				}
				break;
			case ePhongLighting2:
				if(PhongShader2==-1)
				{
					////{//LOG_FNLN;}
					#if 0//def LINUX
					PhongShader2 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/PhongLighting2.GLSL.shd", mainVS, mainFS);
					#else
					PhongShader2 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/PhongLighting2.HLSL.shd", mainVS, mainFS);
					#endif
					PhongVertexDeclaration2 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), PhongShader2);
				}
				break;
			case ePhysicalBasedLighting:
				if(PhysicalBasedShader==-1)
				{
					////{//LOG_FNLN;}
					#if 0//def LINUX
					PhysicalBasedShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/pbr.GLSL.shd", mainVS, mainFS);
					#else
					PhysicalBasedShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/pbr.shd", mainVS, mainFS);
					#endif
					PhysicalBasedVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), PhysicalBasedShader);
				}
				break;
			case eDirectionalLighting:
				if(DirectionalShader==-1)
				{
					DirectionalShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/DirectionalLighting.HLSL.shd", mainVS, mainFS);
					DirectionalVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), DirectionalShader);
				}
				break;
			case eSpotLighting:
				if(SpotShader==-1)
				{
					SpotShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/SpotLighting.HLSL.shd", mainVS, mainFS);
					SpotVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), SpotShader);
				}
				break;
			case eMultiLighting:
				if(MultiShader==-1)
				{
					MultiShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/MultiLighting.HLSL.shd", mainVS, mainFS);
					MultiVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), MultiShader);
				}
				break;
			case eSimpleLighting:
				if(SimpleShader2==-1)
				{
					SimpleShader2 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/glsl_simple_lighting/simple_lighting.shd", mainVS, mainFS);
					SimpleVertexDeclaration2 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), SimpleShader2);
				}
				break;
			#if 1
			case eSnowShader:
				if(SnowShader==-1)
				{
					SnowShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/SnowAccumulation/Snow.shd", "RenderSceneVS_SM30", "RenderScenePS");
					//SnowShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/SnowAccumulation/Snow.shd", "RenderSceneVS_NoSnow", "RenderScenePS_NoSnow");
					SnowVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), SnowShader);
					ExposureDepthMapTexture=-1;//IRenderer::GetRendererInstance()->addImageLibTexture("/", false, IRenderer::GetRendererInstance()->Getlinear());
				}
				break;
			case eCelShader:
				if(OutlineShader==-1)
				{
					OutlineShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/NVIDIA/outline.shd", "main", "main");
					OutlineVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(lineformat, elementsOf(lineformat), OutlineShader);
					ShadingShader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/NVIDIA/shading.shd", "main", "main");
					ShadingVertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(lineformat, elementsOf(lineformat), ShadingShader);
					ShadingTexture=IRenderer::GetRendererInstance()->addImageLibTexture("/CelShading/NVIDIA/celshade.png", false, IRenderer::GetRendererInstance()->Getlinear());
				}
				break;
			case eCelShader1:
				if(CelShader1==-1)
				{
					CelShader1 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/CelShader.shd", "main", "main");
					CelVertexDeclaration1 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), CelShader1);
				}
				break;
			case eCelShader2:
				if(CelShader2==-1)
				{
					CelShader2 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/CelShader2.shd", "main", "main");
					CelVertexDeclaration2 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), CelShader2);
				}
				break;
			case eCelShader3:
				if(CelShader3==-1)
				{
					//CelShader3 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/Toon.fx", "OutlineVertexShader", "OutlinePixelShader");
					CelShader3 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/Toon.fx", "CelVertexShader", "CelPixelShader");
					CelVertexDeclaration3 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), CelShader3);
					ShadingTexture=IRenderer::GetRendererInstance()->addImageLibTexture("/CelShading/NVIDIA/celshade.png", false, IRenderer::GetRendererInstance()->Getlinear());
				}
				break;
			case eCelShader4:
				if(CelShader4==-1)
				{
					//CelShader4 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/ToonColored.fx", "OutlineVertexShader", "OutlinePixelShader");
					CelShader4 = IRenderer::GetRendererInstance()->addShaderFromFile(	"/CelShading/x/ToonColored.fx", "CelVertexShader", "CelPixelShader");
					CelVertexDeclaration4 = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), CelShader4);
				}
				break;
			#endif
			default:
				if(Shader==-1)
				{
					////{//LOG_FNLN;}
					Shader = IRenderer::GetRendererInstance()->addShaderFromFile(	"/MeshRenderer2/rest.HLSL.shd", mainVS, mainFS);
					VertexDeclaration = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), Shader);
				}
				break;
		}
	}
	return 0;
}
LuaScript MeshRenderer2_LuaScript_;
MeshRenderer2::MeshRenderer2()
{
		_max=D3DXFROMWINEVECTOR3( -1e10f, -1e10f, -1e10f);
		_min=D3DXFROMWINEVECTOR3( 1e10f, 1e10f, 1e10f);
		texID = -1;
		Shader = -1;
            VertexDeclaration = -1;
	m_bMeshRenderer2Simpler_ProceduralBuilding=false;
	j_tmp=-1;
	z_tmp=-1;
	tex_tmp=-1;



	AmbientShader=-1;
	DiffuseShader=-1;
	PhongShader1=-1;
	PhongShader2=-1;
	Shader=-1;
	AmbientVertexDeclaration=-1;
	DiffuseVertexDeclaration=-1;
	PhongVertexDeclaration1=-1;
	PhongVertexDeclaration2=-1;
	PhysicalBasedShader=-1;
	PhysicalBasedVertexDeclaration=-1;

	_VertexDeclaration=-1;
	_Shader=-1;

	VertexDeclaration=-1;

	SimpleShader=-1;
	SimpleVertexDeclaration=-1;
	OutlineShader=-1;
	OutlineVertexDeclaration=-1;
	ShadingShader=-1;
	ShadingVertexDeclaration=-1;
	CelShader=-1;
	CelVertexDeclaration=-1;
	CelShader1=-1;
	CelVertexDeclaration1=-1;
	CelShader2=-1;
	CelVertexDeclaration2=-1;
	CelShader3=-1;
	CelVertexDeclaration3=-1;
	CelShader4=-1;
	CelVertexDeclaration4=-1;

	DirectionalShader=-1;
	DirectionalVertexDeclaration=-1;
	SpotShader=-1;
	SpotVertexDeclaration=-1;
	MultiShader=-1;
	MultiVertexDeclaration=-1;

	SnowShader=-1;
	SnowVertexDeclaration=-1;
	ExposureDepthMapTexture=-1;
	ShadingTexture=-1;
	CelShade=-1;
}
MeshRenderer2::~MeshRenderer2(){}



void MeshRenderer2::LoadThreadProc2(std::string& aFileNameMesh)
{
// only ever give us triangles.
                aiPropertyStore* props = aiCreatePropertyStore();
                aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );

                unsigned int aiPostProccesFlags;

                switch (2)
                {
                    case 0:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_Fast;
                        break;
                    case 1:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_Quality;
                        break;
                    case 2:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_MaxQuality;
                        break;
                    default:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_MaxQuality;
                        break;
                }

		#if 0
		aiPostProccesFlags |= aiProcess_GenNormals;
		#elif 0
		aiPostProccesFlags |= aiProcess_GenSmoothNormals;
		#endif
		aiPostProccesFlags |= aiProcess_ConvertToLeftHanded;
                //LOG_FNLN;
		//LOG_PRINT("aFileNameMesh:%s\n", aFileNameMesh.c_str());
		if(0)
		{
			std::vector<stx_Mesh> meshes;
			//LOG_FNLN;
			stx_readXFile(aFileNameMesh.c_str(), meshes);
		}

  	if(aFileNameMesh.substr(aFileNameMesh.find_last_of(".") + 1) == "dae")
		return;
  	if(aFileNameMesh.substr(aFileNameMesh.find_last_of(".") + 1) == "DAE")
		return;

                // aiProcess_FlipUVs is needed for VAO / VBOs,  not sure why.
	LOG_PRINT("MeshRenderer2::LoadThreadProc2:aFileNameMesh=%s\n", aFileNameMesh.c_str());
                pcScene = (aiScene*) aiImportFileExWithProperties(aFileNameMesh.c_str(), aiPostProccesFlags | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | 0, 0, props);
	if(!pcScene)
	{
        	LOG_PRINT("::toupper:\n");
		std::string path = aFileNameMesh;
		std::size_t botDirPos = path.find_last_of("/");
		std::string dir = path.substr(0, botDirPos);
		std::string file = path.substr(botDirPos, path.length());
		std::transform(file.begin(), file.end(),file.begin(), ::toupper);
		//std::transform(file.begin(), file.end(),file.begin(), ::tolower);
		std::string aFileNameMesh_=dir+file;
	LOG_PRINT("MeshRenderer2::LoadThreadProc2:aFileNameMesh=%s\n", aFileNameMesh_.c_str());
		pcScene = (aiScene*) aiImportFileExWithProperties(aFileNameMesh_.c_str(), aiPostProccesFlags | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | 0, 0, props);
		if(!pcScene)
		{
			//LOG_FNLN;
			LOG_PRINT("Failed: pcScene==0\n");
			return;
			//stx_exit(0);
		}
	}
                aiReleasePropertyStore(props);
	{
		if(pcScene->HasAnimations())
		{
			if (0 != pcScene->mNumAnimations)
    			{
       				for (unsigned int i = 0; i < pcScene->mNumAnimations;++i)
				{
					//LOG_PRINT("Animation[%d]=%s\n", i, pcScene->mAnimations[i]->mName.data);
        			}
			}

		}

	if(pcScene->HasMeshes())
	{
	//LOG_PRINT("mNumMeshes=%d\n", pcScene->mNumMeshes);
        for (unsigned int i = 0; i < pcScene->mNumMeshes;++i)
	{
            for (unsigned int p = 0; p < pcScene->mMeshes[i]->mNumBones;++p)
            {
                //LOG_PRINT("Bone[%d]=%s\n", i, pcScene->mMeshes[i]->mBones[p]->mName.data);
            }

		//LOG_PRINT("mesh=%d\n", i);
        	const aiMesh* mesh = pcScene->mMeshes[i];
/*
    aiMesh()
        : mPrimitiveTypes( 0 )
        , mNumVertices( 0 )
        , mNumFaces( 0 )
        , mVertices( NULL )
        , mNormals( NULL )
        , mTangents( NULL )
        , mBitangents( NULL )
        , mFaces( NULL )
        , mNumBones( 0 )
        , mBones( NULL )
        , mMaterialIndex( 0 )
        , mNumAnimMeshes( 0 )
        , mAnimMeshes( NULL )
*/
	//LOG_PRINT("mNumVertices=%d\n", mesh->mNumVertices);
        //LOG_PRINT("mNumFaces=%d\n", mesh->mNumFaces);
        //LOG_PRINT("mNumBones=%d\n", mesh->mNumBones);
        //LOG_PRINT("mNumAnimMeshes=%d\n", mesh->mNumAnimMeshes);

        			if (mesh->mNumFaces)
					LOG_PRINT("mesh->mFaces=%x\n", &mesh->mFaces[0].mIndices[0]);

        			if (mesh->mVertices)
					LOG_PRINT("mesh->mVertices=%x\n", &mesh->mVertices[0]);
            			if (mesh->mNormals)
					LOG_PRINT("mesh->mNormals=%x\n", &mesh->mNormals[0]);
            			if (mesh->mTangents)
					LOG_PRINT("mesh->mTangents=%x\n", &mesh->mTangents[0]);
            			if (mesh->mBitangents)
					LOG_PRINT("mesh->mBitangents=%x\n", &mesh->mBitangents[0]);
            			if (mesh->HasVertexColors( 0))
					LOG_PRINT("mesh->mColors=%x\n", &mesh->mColors[0][0]);
            			if (mesh->HasTextureCoords( 0))
					LOG_PRINT("mesh->mTextureCoords[0]=%x\n", &mesh->mTextureCoords[0][0].x);
            			if (mesh->HasTextureCoords( 1))
					LOG_PRINT("mesh->mTextureCoords[1]=%x\n", &mesh->mTextureCoords[1][0].x);
			}
		}
	}

#if 1
	m_Vertex=ePositionTextureVertex;
#else
	m_Vertex=ePositionNormalVertex;
	for (unsigned int i_ = 0; i_ < pcScene->mNumMeshes;++i_)
	{
AssetHelper::MeshHelper* pcMesh=apcMeshes[i_];
	if (pcMesh->piDiffuseTexture!=-1)
{
		m_Vertex=ePositionTextureVertex;
	break;

}
}
#endif
        for (unsigned int i = 0; i < pcScene->mNumMeshes;++i)
	{
        	const aiMesh* mesh = pcScene->mMeshes[i];
		if(m_Vertex==ePositionTextureVertex)
		{
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
				stx_VertexPositionNormalTexture v;
				v.Position.x=mesh->mVertices[x].x;
				v.Position.y=mesh->mVertices[x].y;
				v.Position.z=mesh->mVertices[x].z;
				#if 1
				if(mesh->mColors[0])
				{
					v.Color.x = mesh->mColors[x]->r;
					v.Color.y = mesh->mColors[x]->g;
					v.Color.z = mesh->mColors[x]->b;
				}
				else
					v.Color = D3DXFROMWINEVECTOR3(0.0f,1.0f,0.0f);
				#endif
            			if (mesh->HasTextureCoords( 0))	{
                			v.Tex = D3DXFROMWINEVECTOR2(
                    				mesh->mTextureCoords[0][x].x,
                    				mesh->mTextureCoords[0][x].y);
            			}
            			else v.Tex = D3DXFROMWINEVECTOR2(0.5f,0.5f);
				vertices.push_back(v);
			}
		}
		else if(m_Vertex==ePositionNormalVertex)
		{
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
				stx_VertexPositionNormalTexture v;
				v.Position.x=mesh->mVertices[x].x;
				v.Position.y=mesh->mVertices[x].y;
				v.Position.z=mesh->mVertices[x].z;
				#if 1
				if(mesh->mColors[0])
				{
					v.Color.x = mesh->mColors[x]->r;
					v.Color.y = mesh->mColors[x]->g;
					v.Color.z = mesh->mColors[x]->b;
				}
				else
					v.Color = D3DXFROMWINEVECTOR3(0.0f,1.0f,0.0f);
				#endif
				if (0 == mesh->mNormals)
				{
#if 1
					v.Normal.x=0.0f;
					v.Normal.y=0.0f;
					v.Normal.z=0.0f;
#else
					D3DXFROMWINEVECTOR3 p(v.Position.x,v.Position.y,v.Position.z);
					D3DXFROMWINEVECTOR3 n;
					D3DXFROMWINEVec3Normalize(&n, &p);
					v.Normal.x=n.x;
					v.Normal.y=n.y;
					v.Normal.z=n.z;
#endif
				}
				else {
					v.Normal.x=mesh->mNormals[x].x;
					v.Normal.y=mesh->mNormals[x].y;
					v.Normal.z=mesh->mNormals[x].z;}
				vertices.push_back(v);
			}
		}


	//for (unsigned int i_ = 0; i_ < pcScene->mNumMeshes;++i_)
	//{


		unsigned int nidx;
		switch (mesh->mPrimitiveTypes) {
			case aiPrimitiveType_POINT:
				nidx = 1;break;
			case aiPrimitiveType_LINE:
				nidx = 2;break;
			case aiPrimitiveType_TRIANGLE:
				nidx = 3;break;
			default: /* assert(false) ??? */ continue;
		};

		// check whether we can use 16 bit indices
		if (mesh->mNumFaces * nidx >= 65536)	{
			// create 32 bit index buffer
			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					indices.push_back( mesh->mFaces[x].mIndices[a] );
				}
			}
		}
		// std::vector<__DWORD__> piDWIB;
		else	{
			// create 16 bit index buffer
			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					indices.push_back( (::uint16_t)mesh->mFaces[x].mIndices[a]);
				}
			}
		}
	stx_Mesh m_;
	meshes.push_back(m_);
#if 0
		struct stx_Mesh
		std::vector<TextureID> textures;
		std::vector<std::vector<__WORD__>> rindices;
#endif
	const aiMesh* pcSource=pcScene->mMeshes[i];
	aiString szPath;
	//aiTextureMapMode mapU(aiTextureMapMode_Wrap),mapV(aiTextureMapMode_Wrap);
	if (pcSource->mTextureCoords[0])
	{
	const aiMaterial* pcMat = pcScene->mMaterials[pcSource->mMaterialIndex];
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_DIFFUSE(0),&szPath))
		{
			std::string f1=szPath.data;
			std::string txfn=szPath.data;
			if(m_szPathTex.length())
				txfn=m_szPathTex+"/"+f1;
			//LOG_FNLN;
			//LOG_PRINT("Assimp Mesh Texture:%s\n", txfn.c_str());
			if(f1=="")
				meshes[meshes.size()-1].textures.push_back(-1);
			else
			{
				meshes[meshes.size()-1].textures.push_back(
			IRenderer::GetRendererInstance()->addImageLibTexture(
			txfn.c_str(),
			false,
			IRenderer::GetRendererInstance()->Getlinear()
			,STATIC
			));
			}
		}
	}

	meshes[meshes.size()-1].vertices=vertices;vertices.clear();
	meshes[meshes.size()-1].rindices.push_back(indices);indices.clear();;
}
	texID=-1;
}
int MeshRenderer2::Load(const char* aFilename, tShader aShader, bool scaleasset, bool a_bMeshRenderer2Simpler_ProceduralBuilding)
{
	return LoadFile(aFilename, aShader, scaleasset, a_bMeshRenderer2Simpler_ProceduralBuilding);
}
int MeshRenderer2::LoadFile(const char* aFilename_, tShader aShader_, bool scaleasset, bool a_bMeshRenderer2Simpler_ProceduralBuilding)
{
		Clear();
	m_bMeshRenderer2Simpler_ProceduralBuilding=a_bMeshRenderer2Simpler_ProceduralBuilding;
	//LOG_PRINT("sizeof(__WORD__)=%d\n", sizeof(__WORD__));
	//LOG_PRINT("sizeof(__DWORD__)=%d\n", sizeof(__DWORD__));
	//LOG_PRINT("sizeof(unsigned int)=%d\n", sizeof(unsigned int));

	if(std::string(aFilename_)=="/Lightning/scene.X")
		return 0;
	//if(std::string(aFilename_)=="/Smoke/Gargoyle/gargoyle.x")
	//	return 0;
	if(std::string(aFilename_)=="/DXJune2010/Dwarf/DwarfWithEffectInstance.x")
		return 0;
	#if 0
	if(std::string(aFilename_)=="/Orcs/Bonfire_wOrcs.x")
		return 0;
	if(std::string(aFilename_)=="/NatureScene/Water/watermesh.3ds")
		return 0;
	if(std::string(aFilename_)=="/blender_3ds/cube.3ds")
		return 0;
	if(std::string(aFilename_)=="/blender_3ds/cup/cup.obj")
		return 0;
	if(std::string(aFilename_)=="/PM/P-TankHistory/allied/t34_76/t34ds/T34-76.3DS")
		return 0;
	#endif

	meshes.clear();

	std::string fn=stx_convertpath(aFilename_);
	const char* aFilename=fn.c_str();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	m_sFileName=aFilename;

	unsigned int found = m_sFileName.find_last_of("/\\");
	if (found != string::npos)
  		m_szPathTex=m_sFileName.substr(0,found);

	LoadThreadProc2(fn);

	if(!meshes.size())
		return 0;

	if(scaleasset)
		ScaleAsset();
	for(unsigned int i=0;i<meshes.size();i++)
	{
		stx_NormalizeMesh(meshes[i]);
		if(aShader_==eCelShader)
			meshes[i].initNormals();
	}

	//initrindices();

	for(unsigned int i=0;i<meshes.size();i++)
		meshes[i].indices.clear();

	#if 1
	//LOG_FNLN;
	//LOG_PRINT("fn=%s\n", aFilename);
	//LOG_PRINT("meshes.size()=%d\n", meshes.size());
	for(unsigned int j=0;j<meshes.size();j++)
	{
		//LOG_PRINT("meshes[%d].name=%s\n", j, meshes[j].m_sName.c_str());
		//LOG_PRINT("meshes[%d].vertices.size()=%d\n", j, meshes[j].vertices.size());
		//LOG_PRINT("meshes[%d].indices.size()=%d\n", j, meshes[j].indices.size());
		//LOG_PRINT("meshes[%d].faces.size()=%d\n", j, meshes[j].faces.size());
		//LOG_PRINT("meshes[%d].textures.size()=%d\n", j, meshes[j].textures.size());
		//LOG_PRINT("meshes[%d].textures_.size()=%d\n", j, meshes[j].textures_.size());
		//LOG_PRINT("meshes[%d].textureNames.size()=%d\n", j, meshes[j].textureNames.size());
		//LOG_PRINT("meshes[%d].rindices.size()=%d\n", j, meshes[j].rindices.size());
		for(unsigned int i=0;i<meshes[j].rindices.size();i++)
		{
			//LOG_PRINT("meshes[%d].textureName_=%s\n", j, meshes[j].textureNames_[i].c_str());
			//LOG_PRINT("meshes[%d].textures_[%d]=%d\n", j, i, meshes[j].textures_[i]);
			//LOG_PRINT("meshes[%d].rindices[%d].size()=%d\n", j, i, meshes[j].rindices[i].size());
		}
	}
	#endif
	LOG_FNLN;
	LOG_PRINT("meshes.size()=%d\n", meshes.size());
	if(meshes.size())
	{
		LOG_PRINT("meshes[0].vertices.size()=%d\n", meshes[0].vertices.size());
	}
	#if 1
	    	D3DXFROMWINEVECTOR3 min_( 1e10f, 1e10f, 1e10f);
	    	D3DXFROMWINEVECTOR3 max_( -1e10f, -1e10f, -1e10f);
	    	for( unsigned int i = 0; i < meshes[0].vertices.size();i++)
	    	{
	    		min_.x = std::min( min_.x, meshes[0].vertices[i].Position.x);
	    		min_.y = std::min( min_.y, meshes[0].vertices[i].Position.y);
	    		min_.z = std::min( min_.z, meshes[0].vertices[i].Position.z);
	    		max_.x = std::max( max_.x, meshes[0].vertices[i].Position.x);
	    		max_.y = std::max( max_.y, meshes[0].vertices[i].Position.y);
	    		max_.z = std::max( max_.z, meshes[0].vertices[i].Position.z);
	    	}
				LOG_FNLN;
	      LOG_PRINT("vertices.size()=%d\n", vertices.size());
	      LOG_PRINT("indices.size()=%d\n", indices.size());
	  		LOG_PRINT("meshes[0].vertices.size()=%d\n", meshes[0].vertices.size());
	  		LOG_PRINT("meshes[0].indices.size()=%d\n", meshes[0].indices.size());
	      LOG_PRINT("min.x=%f\n", min_.x);
	      LOG_PRINT("min.y=%f\n", min_.y);
	      LOG_PRINT("min.z=%f\n", min_.z);
	      LOG_PRINT("max.x=%f\n", max_.x);
	      LOG_PRINT("max.y=%f\n", max_.y);
	      LOG_PRINT("max.z=%f\n", max_.z);
				#endif
	return 0;
}

void MeshRenderer2::initrindices()
{
	for(unsigned int m=0;m<meshes.size();m++)
	{
if(m_bMeshRenderer2Simpler_ProceduralBuilding){
		for(unsigned int t=0;t<meshes[m].textureNames.size();t++)
		{
			if(meshes[m].textureNames[t].length())
			{
				std::string txfn=m_szPathTex+"/"+meshes[m].textureNames[t];
				meshes[m].textures[t]=IRenderer::GetRendererInstance()->addImageLibTexture(txfn.c_str(), false, IRenderer::GetRendererInstance()->Getlinear());
			}
		}
		if(meshes[m].rindices.size())
			continue;
		meshes[m].rindices.resize(meshes[m].textureNames.size());
		if(meshes[m].faces.size())
		{
			for(unsigned int f=0;f<meshes[m].faces.size();f++)
			{
				for(unsigned int t=0;t<meshes[m].textureNames.size();t++)
				{
					if(meshes[m].faces[f]==t)
					{
						meshes[m].rindices[t].push_back(meshes[m].indices[3*f+0]);
						meshes[m].rindices[t].push_back(meshes[m].indices[3*f+1]);
						meshes[m].rindices[t].push_back(meshes[m].indices[3*f+2]);
					}
				}
			}
		}
		else
			meshes[m].rindices.push_back(meshes[m].indices);
}else{
		if(meshes[m].rindices.size())
		{
			for(unsigned int f=0;f<meshes[m].faces.size();f++)
			{
				meshes[m].textures_.push_back(meshes[m].faces[0]);
				meshes[m].textureNames_.push_back(meshes[m].textureNames[meshes[m].faces[0]]);
			}
			continue;
		}
		std::vector<__WORD__> faces_;
		{
			for(unsigned int f=0;f<meshes[m].faces.size();f++)
			{
				if ( std::find(faces_.begin(), faces_.end(), meshes[m].faces[f]) == faces_.end() )
					faces_.push_back(meshes[m].faces[f]);
			}
			meshes[m].rindices.resize(0);
			meshes[m].textures_.resize(0);
			for(unsigned int f_=0;f_<faces_.size();f_++)
			{
				meshes[m].rindices.resize(meshes[m].rindices.size()+1);
				for(unsigned int f=0;f<meshes[m].faces.size();f++)
				{
					{
						if(meshes[m].faces[f]==faces_[f_])
						{
							meshes[m].rindices[f_].push_back(meshes[m].indices[3*f+0]);
							meshes[m].rindices[f_].push_back(meshes[m].indices[3*f+1]);
							meshes[m].rindices[f_].push_back(meshes[m].indices[3*f+2]);
						}
					}
				}
				meshes[m].textures_.push_back(faces_[f_]);
				if(meshes[m].faces.size()>f_)
					if(meshes[m].textureNames.size()>meshes[m].faces[f_])
						meshes[m].textureNames_.push_back(meshes[m].textureNames[meshes[m].faces[f_]]); // ??? Muistialueen ylitys (luotiin core-tiedosto)
			}
		}

		if(!meshes[m].textureNames_.size())
		{
			if(meshes[m].textureNames.size())
				meshes[m].textureNames_.push_back(meshes[m].textureNames[0]);
			else
				meshes[m].textureNames_.push_back("");
		}
}
	}
}
int MeshRenderer2::LoadAsset(const char* filename, bool scaleasset)
{
	Clear();
	Load(filename, eShaderNone, scaleasset);
	return 0;
}
const char* MeshRenderer2::GetFileName(){return m_sFileName.c_str();}

void MeshRenderer2::SetAmbientColor(D3DXFROMWINEVECTOR4 a)
{
	m_v4AmbientColor=a;

				//LOG_FNLN;
				//stx_Dump4f(a, "a");
				//stx_Dump4f(m_v4AmbientColor, "m_v4AmbientColor");
}
void MeshRenderer2::SetDiffuseColor(D3DXFROMWINEVECTOR4 a)
{
	m_v4DiffuseColor=a;
				//LOG_FNLN;
				//stx_Dump4f(a, "a");
				//stx_Dump4f(m_v4DiffuseColor, "m_v4DiffuseColor");
}
void MeshRenderer2::SetLightDir(D3DXFROMWINEVECTOR4 a){m_v4LightDir=a;}
void MeshRenderer2::SetEyeDir(D3DXFROMWINEVECTOR4 a){m_v4EyeDir=a;}
void MeshRenderer2::SetTX(float aX){}
void MeshRenderer2::SetTY(float aY){}
void MeshRenderer2::SetTZ(float aZ){}
void MeshRenderer2::SetSX(float aScaleX){}
void MeshRenderer2::SetSY(float aScaleY){}
void MeshRenderer2::SetSZ(float aScaleZ){}
void MeshRenderer2::SetSpinX(float aAngleX){}
void MeshRenderer2::SetSpinY(float aAngleY){}
void MeshRenderer2::SetSpinZ(float aAngleZ){}

//void MeshRenderer2::GetBounds(D3DXFROMWINEVECTOR3& ____min, D3DXFROMWINEVECTOR3& ____max){}
//void MeshRenderer2::GetBoundingSphereRadius(float& r){}

void MeshRenderer2::SetRenderingMode(unsigned int i){}
void MeshRenderer2::ChangeShader(const D3DXFROMWINEMATRIX* mat, TextureID id, ShaderID shdid, VertexFormatID vfid,
			D3DXFROMWINEVECTOR4 aAmbientColor,
			D3DXFROMWINEVECTOR4 aDiffuseColor,
			D3DXFROMWINEVECTOR4 aLightDir_,
			D3DXFROMWINEVECTOR4 aEyeDir_)
{
//{//LOG_FNLN;}
	if(shdid>-1)
		IRenderer::GetRendererInstance()->setShader(shdid);
//{//LOG_FNLN;}
	if((shdid>-1)&&(vfid>-1))
		IRenderer::GetRendererInstance()->setVertexFormat(vfid);
	//else if (VertexDeclaration > -1)
	//	IRenderer::GetRendererInstance()->setVertexFormat(VertexDeclaration);
//{//LOG_FNLN;}



	if((shdid>-1)&&mat)
		IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", *mat);
//{//LOG_FNLN;}
	if((shdid>-1)&&(id>-1))
		IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", id);
//{//LOG_FNLN;}
	if(shdid>-1)
		return;
//{//LOG_FNLN;}
	if(id > -1)
		texID=id;
	//if(mShader!=eCelShader)
		tex_tmp=texID;
	D3DXFROMWINEMATRIX w, I;
	D3DXFROMWINEMatrixIdentity(&I);

	if(mat)
    {
        D3DXFROMWINEMatrixIdentity(&w);
		w=*mat;
    }
	else
	{
		float mAngleX=0.0f;
		float mAngleY=0.0f;
		stx_GetAngles(mAngleX, mAngleY);
		D3DXFROMWINEMatrixRotationYawPitchRoll(&w, mAngleX, mAngleY, 0.0f);
		//D3DXFROMWINEMatrixIdentity(&w);
	}

	D3DXFROMWINEMATRIX LR, RR;
#if 0
	static float mLAngleX=3.0f*90.0f;
	static float mLAngleY=2.0f*90.0f;
	static float mRAngleX=3.0f*90.0f;
	static float mRAngleY=2.0f*90.0f;
#elif 0
{
	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F9))
	{
		mLAngleX+=45.0f;
		if(mLAngleX>360.0f)
			mLAngleX=0.0f;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F10))
	{
		mLAngleY+=45.0f;
		if(mLAngleY>360.0f)
			mLAngleY=0.0f;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F11))
	{
		mRAngleX+=45.0f;
		if(mRAngleX>360.0f)
			mRAngleX=0.0f;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F12))
	{
		mRAngleY+=45.0f;
		if(mRAngleY>360.0f)
			mRAngleY=0.0f;
	}
}
#elif 0
	static bool m_bLuaScript=false;
	if(!m_bLuaScript)
	{
		#if 0
		std::string fn=stx_convertpath("/Framework3/Framework3.lua");
		bool r=MeshRenderer2_LuaScript_.Load(fn.c_str());
		#else
		bool r=MeshRenderer2_LuaScript_.Load("/Framework3/Framework3.lua");
		#endif
		if(r)
		{
			mLAngleX = atof(MeshRenderer2_LuaScript_.get<std::string>("mLAngleX").c_str());
			mLAngleY = atof(MeshRenderer2_LuaScript_.get<std::string>("mLAngleY").c_str());
			mRAngleX = atof(MeshRenderer2_LuaScript_.get<std::string>("mRAngleX").c_str());
			mRAngleY = atof(MeshRenderer2_LuaScript_.get<std::string>("mRAngleY").c_str());
		}
		m_bLuaScript=true;
	}
#endif
float mLAngleX=3.0f*90.0f;
float mLAngleY=2.0f*90.0f;
float mRAngleX=3.0f*90.0f;
float mRAngleY=2.0f*90.0f;
	D3DXFROMWINEMatrixRotationYawPitchRoll(&LR, mLAngleX, mLAngleY, 0.0f);
	D3DXFROMWINEMatrixRotationYawPitchRoll(&RR, mRAngleX, mRAngleY, 0.0f);

			D3DXFROMWINEVECTOR4 AmbientColor=m_v4AmbientColor;
			//if(mShader==eAmbientLighting) AmbientColor*=10.0f;
			D3DXFROMWINEVECTOR4 DiffuseColor=m_v4DiffuseColor;
				//LOG_FNLN;
				//stx_Dump4f(m_v4AmbientColor, "m_v4AmbientColor");
				//stx_Dump4f(m_v4DiffuseColor, "m_v4DiffuseColor");
				//stx_Dump4f(AmbientColor, "AmbientColor");
				//stx_Dump4f(DiffuseColor, "DiffuseColor");
				D3DXFROMWINEVECTOR3 AmbientColor3(m_v4AmbientColor.x, m_v4AmbientColor.y, m_v4AmbientColor.z);
			D3DXFROMWINEVECTOR3 DiffuseColor3(m_v4DiffuseColor.x, m_v4DiffuseColor.y, m_v4DiffuseColor.z);

			D3DXFROMWINEVECTOR4 LightDir_=m_v4LightDir;
			D3DXFROMWINEVECTOR4 EyeDir_=m_v4EyeDir;

//D3DXFROMWINEVec4TransformCoord(&LightDir_, &LightDir_, &LR);
//D3DXFROMWINEVec4TransformCoord(&EyeDir_, &EyeDir_, &RR);

	D3DXFROMWINEVECTOR3 LightDir___;
	D3DXFROMWINEVECTOR3 EyeDir___;

	D3DXFROMWINEVECTOR3 LightDir__(LightDir_.x, LightDir_.y, LightDir_.z);
	D3DXFROMWINEVECTOR3 EyeDir__(EyeDir_.x, EyeDir_.y, EyeDir_.z);
	D3DXFROMWINEVec3Normalize(&LightDir__, &LightDir__);
	D3DXFROMWINEVec3Normalize(&EyeDir__, &EyeDir__);
	D3DXFROMWINEVECTOR4 LightDir(LightDir__.x, LightDir__.y, LightDir__.z, 1.0f);
	D3DXFROMWINEVECTOR4 EyeDir(EyeDir__.x, EyeDir__.y, EyeDir__.z, 1.0f);
				D3DXFROMWINEMATRIX wit;FLOAT determinant=0.0f;
				D3DXFROMWINEMatrixInverse(&wit, &determinant, &w);

				D3DXFROMWINEVECTOR3 LightDirection(-1.0f, 0.0f, 0.0f);
				D3DXFROMWINEVECTOR3 LightPosition[5];
				D3DXFROMWINEVECTOR3 LightIntensity[5];
				D3DXFROMWINEVECTOR4 lights[4];
D3DXFROMWINEVECTOR3 mLightPosInWorldSpace;
    D3DXFROMWINEMATRIX mLightModelMatrix, mMatrix;
    D3DXFROMWINEVECTOR3 mLightPosInModelSpace;
    // Calculate position of the light
    // Rotate and then push into the distance.
    // Do a compile rotation every 10 seconds;

	static float angleInRadians = 0.0f;
	angleInRadians += (float)D3DXFROMWINE_PI * 0.005f;

    //LOG_PRINT("angleInRadians=%f\n", angleInRadians);

    D3DXFROMWINEMatrixIdentity(&mLightModelMatrix);
    D3DXFROMWINEMatrixRotationY(&mMatrix, angleInRadians);
    mLightModelMatrix*=mMatrix;
    //D3DXFROMWINEMatrixTranslation(&mMatrix, 0, 0, 2);
    //mLightModelMatrix*=mMatrix;

    mLightPosInModelSpace=D3DXFROMWINEVECTOR3(1.0f, 0.0f, 0.0f);

    D3DXFROMWINEVec3TransformCoord(&mLightPosInWorldSpace, &mLightPosInModelSpace, &mLightModelMatrix);
D3DXFROMWINEVECTOR4 mLightPosInWorldSpace4=D3DXFROMWINEVECTOR4(mLightPosInWorldSpace.x, mLightPosInWorldSpace.y, mLightPosInWorldSpace.z, 1.0f);

    				for( int i = 0; i < 5; i++ )
				{
        				D3DXFROMWINEVECTOR3 pos=D3DXFROMWINEVECTOR3(2.0f * cosf((2.0f*D3DXFROMWINE_PI / 5) * i), 1.2f, 2.0f * sinf((2.0f*D3DXFROMWINE_PI / 5) * i) + 1.0f);
					D3DXFROMWINEVec3TransformCoord(&pos, &pos, &w);
					LightPosition[i]=mLightPosInWorldSpace*1000.0f;



					LightPosition[i]=pos;
    				}
    				LightIntensity[0]=D3DXFROMWINEVECTOR3(0.0f,0.8f,0.8f);
    				LightIntensity[1]=D3DXFROMWINEVECTOR3(0.0f,0.0f,0.8f);
    				LightIntensity[2]=D3DXFROMWINEVECTOR3(0.8f,0.0f,0.0f);
    				LightIntensity[3]=D3DXFROMWINEVECTOR3(0.0f,0.8f,0.0f);
    				LightIntensity[4]=D3DXFROMWINEVECTOR3(0.8f,0.8f,0.8f);
				D3DXFROMWINEVECTOR3 m_v3EyeDir(m_v4EyeDir.x, m_v4EyeDir.y, m_v4EyeDir.z);
    				lights[0]=D3DXFROMWINEVECTOR4(LightPosition[0].x,LightPosition[0].y,LightPosition[0].z,1.0f);
    				lights[1]=D3DXFROMWINEVECTOR4(LightPosition[1].x,LightPosition[1].y,LightPosition[1].z,1.0f);
    				lights[2]=D3DXFROMWINEVECTOR4(LightPosition[2].x,LightPosition[2].y,LightPosition[2].z,1.0f);
    				lights[3]=D3DXFROMWINEVECTOR4(LightPosition[3].x,LightPosition[3].y,LightPosition[3].z,1.0f);
{
    D3DXFROMWINEMATRIX mLightModelMatrix, mMatrix;
    D3DXFROMWINEVECTOR4 mLightPosInWorldSpace, mLightPosInModelSpace;
    // Calculate position of the light
    // Rotate and then push into the distance.
    // Do a compile rotation every 10 seconds;

	static float angleInRadians = 0.0f;
	angleInRadians += (float)D3DXFROMWINE_PI * 0.005f;

    //LOG_PRINT("angleInRadians=%f\n", angleInRadians);

    D3DXFROMWINEMatrixIdentity(&mLightModelMatrix);
    D3DXFROMWINEMatrixRotationY(&mMatrix, angleInRadians);
    mLightModelMatrix*=mMatrix;
    //D3DXFROMWINEMatrixTranslation(&mMatrix, 0, 0, 2);
    //mLightModelMatrix*=mMatrix;

    mLightPosInModelSpace=D3DXFROMWINEVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

    D3DXFROMWINEVec4TransformCoord(&mLightPosInWorldSpace, &mLightPosInModelSpace, &mLightModelMatrix);

				//LightDir___=mLightPosInWorldSpace*1000.0f;
				//EyeDir___=m_v4EyeDir;
				lights[0]=LightDir;
				lights[1]=LightDir;
				lights[2]=LightDir;
				lights[3]=LightDir;
}

			D3DXFROMWINEVECTOR3 Kd(0.4f, 0.4f, 0.4f);
			D3DXFROMWINEVECTOR3 Ks(0.9f, 0.9f, 0.9f);
			D3DXFROMWINEVECTOR3 Ka(0.1f, 0.1f, 0.1f);
			float Shininess=180.0f;
			LightPosition[0]=D3DXFROMWINEVECTOR3(1.0f, 0.0f, 0.0f);
			LightIntensity[0]=D3DXFROMWINEVECTOR3(0.8f,0.8f,0.8f);
			D3DXFROMWINEVECTOR3 SpotLightposition(10.0f, 10.0f, 10.0f);
			D3DXFROMWINEVECTOR3 SpotLightintensity(0.9f,0.9f,0.9f);
			D3DXFROMWINEVECTOR3 SpotLightdirection( -1.0f, 0.0f, 0.0f);
			float SpotLightexponent=30.0f;  // Angular attenuation exponent
			float SpotLightcutoff=15.0f;    // Cutoff angle (between 0 and 90}
			Shininess=100.0;

//{//LOG_FNLN;}
//{//LOG_FNLN;}
#if 0
		LOG_FNLN;
		LOG_PRINT("Renderer: %s\n", STX_Service::GetWindowInstance()->GetCaption());
		LOG_PRINT("mShader=%x\n", mShader);
		switch(mShader)
		{
			case eShaderNone:
				LOG_PRINT("eShaderNone\n");
				break;
			case eHLSL_Shader:
				LOG_PRINT("eHLSL_Shader\n");
				break;
			case eGLSL_Shader:
				LOG_PRINT("eGLSL_Shader\n");
				break;
			case eGLES_Shader_android:
				LOG_PRINT("eGLES_Shader_android\n");
				break;
			case eGLES_Shader_Ubuntu:
				LOG_PRINT("eGLES_Shader_Ubuntu\n");
				break;
			case eSimpleShader:
				LOG_PRINT("eSimpleShader\n");
				break;
			case eSimpleShader2:
				LOG_PRINT("eSimpleShader2\n");
				break;
			case eAmbientLighting:
				LOG_PRINT("eAmbientLighting\n");
				break;
			case eDiffuseLighting:
				LOG_PRINT("eDiffuseLighting\n");
				break;
			case ePhongLighting1:
				LOG_PRINT("ePhongLighting1\n");
				break;
			case ePhongLighting2:
				LOG_PRINT("ePhongLighting2\n");
				break;
			case ePhysicalBasedLighting:
				LOG_PRINT("ePhysicalBasedLighting\n");
				break;
			case ePositionTexture:
				LOG_PRINT("ePositionTexture\n");
				break;
			case ePositionNormal:
				LOG_PRINT("ePositionNormal\n");
				break;
			case ePositionNormalTexture:
				LOG_PRINT("ePositionNormalTexture\n");
				break;
			case ePositionNormalNormal:
				LOG_PRINT("ePositionNormalNormal\n");
				break;
			case eCelShader:
				LOG_PRINT("eCelShader\n");
				break;
			case eDirectionalLighting:
				LOG_PRINT("eDirectionalLighting\n");
				break;
			case eSpotLighting:
				LOG_PRINT("eSpotLighting\n");
				break;
			case eMultiLighting:
				LOG_PRINT("eMultiLighting\n");
				break;
			case eSnowShader:
				LOG_PRINT("eSnowShader\n");
				break;
			case eCelShader1:
				LOG_PRINT("eCelShader1\n");
				break;
			case eCelShader2:
				LOG_PRINT("eCelShader2\n");
				break;
			case eCelShader3:
				LOG_PRINT("eCelShader3\n");
				break;
			case eCelShader4:
				LOG_PRINT("eCelShader4\n");
				break;
			default:
				LOG_PRINT("???\n");
				break;
		}
#endif
		switch(mShader)
		{
			case eColorShader:
					//{//LOG_FNLN;}//LOG_PRINT("eColorShader:\n");}
				IRenderer::GetRendererInstance()->setShader(ColorShader);
				IRenderer::GetRendererInstance()->setVertexFormat(ColorVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case eSimpleShader:
					//{//LOG_FNLN;}//LOG_PRINT("eSimpleShader:\n");}
				IRenderer::GetRendererInstance()->setShader(SimpleShader);
				IRenderer::GetRendererInstance()->setVertexFormat(SimpleVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case eAmbientLighting:
				//{//LOG_FNLN;}
				IRenderer::GetRendererInstance()->setShader(AmbientShader);
				IRenderer::GetRendererInstance()->setVertexFormat(AmbientVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				//LOG_FNLN;
				//stx_Dump4f(AmbientColor, "AmbientColor");
				IRenderer::GetRendererInstance()->setShaderConstant4f("AmbientColor", AmbientColor);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case eDiffuseLighting:
				IRenderer::GetRendererInstance()->setShader(DiffuseShader);
				IRenderer::GetRendererInstance()->setVertexFormat(DiffuseVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				IRenderer::GetRendererInstance()->setShaderConstant4f("AmbientColor", AmbientColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("DiffuseColor", DiffuseColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecLightDir", LightDir);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case ePhongLighting1:
				//{//LOG_FNLN;}
				IRenderer::GetRendererInstance()->setShader(PhongShader1);
				IRenderer::GetRendererInstance()->setVertexFormat(PhongVertexDeclaration1);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				//LOG_FNLN;
				//stx_Dump4f(AmbientColor, "AmbientColor");
				//stx_Dump4f(DiffuseColor, "DiffuseColor");
				IRenderer::GetRendererInstance()->setShaderConstant4f("AmbientColor", AmbientColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("DiffuseColor", DiffuseColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecLightDir", LightDir);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecEye", EyeDir);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case ePhongLighting2:
				//{//LOG_FNLN;}
				IRenderer::GetRendererInstance()->setShader(PhongShader2);
				IRenderer::GetRendererInstance()->setVertexFormat(PhongVertexDeclaration2);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				//LOG_FNLN;
				//stx_Dump4f(AmbientColor, "AmbientColor");
				//stx_Dump4f(DiffuseColor, "DiffuseColor");
				IRenderer::GetRendererInstance()->setShaderConstant4f("AmbientColor", AmbientColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("DiffuseColor", DiffuseColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecLightDir", LightDir);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecEye", EyeDir);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
			case e_Lighting:
				//{//LOG_FNLN;}
				IRenderer::GetRendererInstance()->setShader(_Shader);
				IRenderer::GetRendererInstance()->setVertexFormat(_VertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant3f("PointLight_position", LightDir__);
				IRenderer::GetRendererInstance()->setShaderConstant4f("PointLight_ambientColour", AmbientColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("PointLight_diffuseColour", DiffuseColor);
				IRenderer::GetRendererInstance()->setShaderConstant4f("PointLight_specularColour", DiffuseColor);
				IRenderer::GetRendererInstance()->setShaderConstant1f("PointLight_ambientIntensity", 1.0f);
				IRenderer::GetRendererInstance()->setShaderConstant1f("PointLight_diffuseIntensity", 1.0f);
				IRenderer::GetRendererInstance()->setShaderConstant1f("PointLight_specularIntensity", 1.0f);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("View", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("Projection", I);
				IRenderer::GetRendererInstance()->setShaderConstant4f("EyePosition", EyeDir);
				break;
			case ePhysicalBasedLighting:
				IRenderer::GetRendererInstance()->setShader(PhysicalBasedShader);
				IRenderer::GetRendererInstance()->setVertexFormat(PhysicalBasedVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecLightDir", mLightPosInWorldSpace4);
				IRenderer::GetRendererInstance()->setShaderConstant4f("vecEye", EyeDir);
				IRenderer::GetRendererInstance()->setShaderConstantArray4f("lights", lights, 4);
				IRenderer::GetRendererInstance()->setShaderConstant3f("camPos", m_v3EyeDir);
				break;
			case eDirectionalLighting:
				IRenderer::GetRendererInstance()->setShader(DirectionalShader);
				IRenderer::GetRendererInstance()->setVertexFormat(DirectionalVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant3f("LightPosition", LightPosition[0]);
				IRenderer::GetRendererInstance()->setShaderConstant3f("LightIntensity", LightIntensity[0]);
				IRenderer::GetRendererInstance()->setShaderConstant1f("Shininess", 180.0);
				Kd = D3DXFROMWINEVECTOR3(0.8f, 0.8f, 0.8f);
				Ks = D3DXFROMWINEVECTOR3(0.9f, 0.9f, 0.9f);
				Ka = D3DXFROMWINEVECTOR3(0.1f, 0.1f, 0.1f);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Kd", Kd);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ks", Ks);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ka", Ka);
				break;
			case eSpotLighting:
				IRenderer::GetRendererInstance()->setShader(SpotShader);
				IRenderer::GetRendererInstance()->setVertexFormat(SpotVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
#if 1
				IRenderer::GetRendererInstance()->setShaderConstant3f("SpotLightposition", SpotLightposition);
				IRenderer::GetRendererInstance()->setShaderConstant3f("SpotLightintensity", SpotLightintensity);
				IRenderer::GetRendererInstance()->setShaderConstant3f("SpotLightdirection", SpotLightdirection);
				IRenderer::GetRendererInstance()->setShaderConstant1f("SpotLightexponent", SpotLightexponent);
				IRenderer::GetRendererInstance()->setShaderConstant1f("SpotLightcutoff", SpotLightcutoff);
				IRenderer::GetRendererInstance()->setShaderConstant1f("Shininess", 100.0);
				Kd = D3DXFROMWINEVECTOR3(0.9f, 0.5f, 0.3f);
				Ks = D3DXFROMWINEVECTOR3(0.95f, 0.95f, 0.95f);
				Ka = D3DXFROMWINEVECTOR3(0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Kd", Kd);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ks", Ks);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ka", Ka);
#endif
				break;
			case eMultiLighting:
				IRenderer::GetRendererInstance()->setShader(MultiShader);
				IRenderer::GetRendererInstance()->setVertexFormat(MultiVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstantArray3f("LightPosition", LightPosition, 5);
				IRenderer::GetRendererInstance()->setShaderConstantArray3f("LightIntensity", LightIntensity, 5);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Kd", Kd);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ks", Ks);
				IRenderer::GetRendererInstance()->setShaderConstant3f("Ka", Ka);
				IRenderer::GetRendererInstance()->setShaderConstant1f("Shininess", 180.0);
				break;




			case eSimpleLighting:
				IRenderer::GetRendererInstance()->setShader(SimpleShader2);
				IRenderer::GetRendererInstance()->setVertexFormat(SimpleVertexDeclaration2);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				break;




			#if 1
			case eSnowShader:
//{//LOG_FNLN;}
				IRenderer::GetRendererInstance()->setShader(SnowShader);
				IRenderer::GetRendererInstance()->setVertexFormat(SnowVertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mWorld", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mWorldIT", wit);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mWorldView", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mViewProj", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mProjection", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mWorldViewProjection", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mViewInverse", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("g_mExposureWorldViewOrthoProj", w); // mWorld*g_pExposureView*g_pExposureOrthoProj
				IRenderer::GetRendererInstance()->setShaderConstant1f("SceneWidth",  float(IRenderer::GetRendererInstance()->GetViewportWidth()));
				IRenderer::GetRendererInstance()->setShaderConstant1f("SceneHeight", float(IRenderer::GetRendererInstance()->GetViewportHeight()));
				//IRenderer::GetRendererInstance()->setTexture("MeshTextureSampler", MeshTexture);
				IRenderer::GetRendererInstance()->setTexture("ExposureDepthMapSampler", ExposureDepthMapTexture);
				break;
			case eCelShader:
//{//LOG_FNLN;}
				if(OutlineShader!=-1)
				{
					IRenderer::GetRendererInstance()->setShader(OutlineShader);
					IRenderer::GetRendererInstance()->setVertexFormat(OutlineVertexDeclaration);
					IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
					w_tmp=w;
					IRenderer::GetRendererInstance()->setTexture("CelShade", ShadingTexture);
				}
				break;
			case eCelShader1:
				IRenderer::GetRendererInstance()->setShader(CelShader1);
				IRenderer::GetRendererInstance()->setVertexFormat(CelVertexDeclaration1);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				break;
			case eCelShader2:
				IRenderer::GetRendererInstance()->setShader(CelShader2);
				IRenderer::GetRendererInstance()->setVertexFormat(CelVertexDeclaration2);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				break;
			case eCelShader3:
				IRenderer::GetRendererInstance()->setShader(CelShader3);
				IRenderer::GetRendererInstance()->setVertexFormat(CelVertexDeclaration3);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("View", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("Projection", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldInverseTranspose", wit);
				IRenderer::GetRendererInstance()->setTexture("textureSampler", ShadingTexture);
				break;
			case eCelShader4:
				IRenderer::GetRendererInstance()->setShader(CelShader4);
				IRenderer::GetRendererInstance()->setVertexFormat(CelVertexDeclaration4);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("World", w);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("View", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("Projection", I);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldInverseTranspose", wit);
				break;
			#endif
			default:
					//{//LOG_FNLN;}//LOG_PRINT("default:\n");}
				IRenderer::GetRendererInstance()->setShader(Shader);
				IRenderer::GetRendererInstance()->setVertexFormat(VertexDeclaration);
				IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w);
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", texID);
				break;
		}
//{//LOG_FNLN;}
}



void MeshRenderer2::BeginDraw(	const D3DXFROMWINEMATRIX* amat, TextureID id, ShaderID shdid, VertexFormatID vfid,
			D3DXFROMWINEVECTOR4 AmbientColor,
			D3DXFROMWINEVECTOR4 DiffuseColor,
			D3DXFROMWINEVECTOR4 LightDir,
			D3DXFROMWINEVECTOR4 EyeDir)
{
#if 0
	D3DXFROMWINEMATRIX mat;
	if(amat)
		mat=*amat;
	else
		D3DXFROMWINEMatrixIdentity(&mat);
#endif
	ChangeShader(amat, id, shdid, vfid, AmbientColor, DiffuseColor, LightDir, EyeDir);
}

	unsigned int MeshRenderer2::EndDraw(const unsigned int flags)
        {
		unsigned int ret=0;
#if 0
	if(indices.size()){
		ret=IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLES, 0, vertices.size(), indices.size()/3, &indices[0], &indices[0], sizeof(__WORD__), &vertices[0], &vertices[0], sizeof(stx_VertexPositionNormalTexture), flags);
		if(ret){//LOG_PRINT("%s:%s:%d:ret=%x\n",__FILE__,__FUNCTION__,__LINE__,ret);}}
	else{
		ret=IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLES, vertices.size()/3, &vertices[0], &vertices[0], sizeof(stx_VertexPositionNormalTexture), flags);
		if(ret){//LOG_PRINT("%s:%s:%d:ret=%x\n",__FILE__,__FUNCTION__,__LINE__,ret);}}
#else
		int j=0;
		int z=0;
		if(j_tmp>-1)
			j=j_tmp;
		if(z_tmp>-1)
			z=z_tmp;

		if(!meshes.size())
		{
			//LOG_FNLN;
			//LOG_PRINT("meshes.size()=0\n");
			stx_exit(0);
		}

		if(j>meshes.size())
		{
			//LOG_FNLN;
			//LOG_PRINT("j>meshes.size()\n");
			//LOG_PRINT("%d>meshes.size()=%d\n", j, meshes.size());
			stx_exit(0);
		}

		if(z>meshes[j].rindices.size())
		{
			//LOG_FNLN;
			//LOG_PRINT("j>meshes[j].rindices.size()\n");
			//LOG_PRINT("%d>meshes[%d].rindices.size()=%d\n", j, j, meshes[j].rindices.size());
			stx_exit(0);
		}

		if(mShader!=eCelShader)
		{
			if(tex_tmp>-1)
			{
				IRenderer::GetRendererInstance()->setTexture("DIFFUSE_SAMPLER", tex_tmp);
				tex_tmp=-1;
			}
			if(meshes[j].rindices[z].size()/3)
			{
				//LOG_FNLN;
				//LOG_PRINT("DrawIndexedPrimitiveUP:\n");
				//LOG_PRINT("meshes[%d].vertices.size()=%d\n", j, meshes[j].vertices.size());
				//LOG_PRINT("meshes[%d].rindices[%d].size()=%d\n", j, z, meshes[j].rindices[z].size());
				//LOG_PRINT("&meshes[%d].vertices[0]=%x\n", j, &meshes[j].vertices[0]);
				//LOG_PRINT("&meshes[%d].rindices[%d].at(0)=%x\n", j, z, &meshes[j].rindices[z].at(0));
				IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLES, 0, meshes[j].vertices.size(), meshes[j].rindices[z].size()/3, &meshes[j].rindices[z].at(0), &meshes[j].rindices[z].at(0), CONSTANT_INDEX2, &meshes[j].vertices[0], &meshes[j].vertices[0], sizeof(stx_VertexPositionNormalTexture));
			}else{
				//LOG_FNLN;
				//LOG_PRINT("DrawPrimitiveUP:\n");
				//LOG_PRINT("meshes[%d].vertices.size()=%d\n", j, meshes[j].vertices.size());
				//LOG_PRINT("&meshes[%d].vertices[0]=%x\n", j, &meshes[j].vertices[0]);
				IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLES, meshes[j].vertices.size()/3, &meshes[j].vertices[0], &meshes[j].vertices[0], sizeof(stx_VertexPositionNormalTexture));
			}
		}
		else
		{
			IRenderer::GetRendererInstance()->setShader(OutlineShader);
			IRenderer::GetRendererInstance()->setVertexFormat(OutlineVertexDeclaration);
			IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w_tmp);
			if(meshes[j].rindices[z].size()/3)
				IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLES, 0, meshes[j].linevertices.size(), meshes[j].rindices[z].size()/3, &meshes[j].rindices[z].at(0), &meshes[j].rindices[z].at(0), CONSTANT_INDEX2, &meshes[j].linevertices[0], &meshes[j].linevertices[0], sizeof(stx_VertexPositionNormalNormal));
			else
				IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLES, meshes[j].linevertices.size()/3, &meshes[j].linevertices[0], &meshes[j].linevertices[0], sizeof(stx_VertexPositionNormalNormal));
			IRenderer::GetRendererInstance()->setShader(ShadingShader);
			IRenderer::GetRendererInstance()->setVertexFormat(ShadingVertexDeclaration);
			IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", w_tmp);
			IRenderer::GetRendererInstance()->setTexture("CelShade", CelShade);
			if(meshes[j].rindices[z].size()/3)
				IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLES, 0, meshes[j].linevertices.size(), meshes[j].rindices[z].size()/3, &meshes[j].rindices[z].at(0), &meshes[j].rindices[z].at(0), CONSTANT_INDEX2, &meshes[j].linevertices[0], &meshes[j].linevertices[0], sizeof(stx_VertexPositionNormalNormal));
			else
				IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLES, meshes[j].linevertices.size()/3, &meshes[j].linevertices[0], &meshes[j].linevertices[0], sizeof(stx_VertexPositionNormalNormal));

		if(j_tmp>-1)
			j_tmp=-1;
		if(z_tmp>-1)
			z_tmp=-1;
		}
#if 0
		float mAngleX=0.0f;
		float mAngleY=0.0f;
		stx_GetAngles(mAngleX, mAngleY);
		D3DXFROMWINEMATRIX R;
		D3DXFROMWINEMatrixRotationYawPitchRoll(&R, mAngleX, mAngleY, 0.0f);
		D3DXFROMWINEVECTOR3 v3(1,0,0);
		D3DXFROMWINEVec3TransformCoord(&v3, &v3, &R);
		m_v4LightDir.x=v3.x;
		m_v4LightDir.y=v3.y;
		m_v4LightDir.z=v3.z;
#endif
#endif
		return ret;
        }
#if 1
#if 1
    void MeshRenderer2::Render( stx_Effect::stx_Technique* pTechnique, const D3DXFROMWINEMATRIX* aw)
#else
    void MeshRenderer2::Render( stx_Effect::stx_Technique* pTechnique, const char* awName, const D3DXFROMWINEMATRIX* aw)
#endif
{
	if(!pTechnique)
		return;
	stx_Effect* pEffect=pTechnique->m_pEffect;
	if(!pEffect)
		return;
	pEffect->SetTechnique(pTechnique->m_sName.c_str());
	unsigned int cPasses=0;
	unsigned int Value=0;
	pEffect->Begin(&cPasses, Value);
	for(unsigned int iiii=0;iiii<cPasses;iiii++)
	{
		pEffect->BeginPass(iiii);
    		render(aw);
		pEffect->EndPass();
	}
	pEffect->End();

}
#endif
void MeshRenderer2::render(const D3DXFROMWINEMATRIX* aw)
{
	for(unsigned int j=0;j<meshes.size();j++)
	{
		if(m_bMeshRenderer2Simpler_ProceduralBuilding)
			if(!meshes[j].vertices.size())
				continue;
	for(unsigned int z=0;z<meshes[j].rindices.size();z++)
	{
		if(m_bMeshRenderer2Simpler_ProceduralBuilding)
			if(!meshes[j].rindices[z].size())
				continue;
		TextureID texID_=-1;
		if(z<meshes[j].textures.size())
		{
			if(m_bMeshRenderer2Simpler_ProceduralBuilding)
				texID_=meshes[j].textures /* _ */ [z];
			else
				texID_=meshes[j].textures[z];
		}
	j_tmp=j;
	z_tmp=z;
	BeginDraw(aw,  texID_);
	EndDraw();
	}}
}
void MeshRenderer2::render_(const D3DXFROMWINEMATRIX* aw)
{
#if 0
	D3DXFROMWINEMATRIX w;
	if(aw)
		w=*aw;
	else
		D3DXFROMWINEMatrixIdentity(&w);
#endif
	render(aw);
}
void MeshRenderer2::render(
		float a_fTX,float a_fTY,float a_fTZ,
		float a_fSX,float a_fSy,float a_fSZ,
		float a_fSpinX,
		float a_fSpinY,
		float a_fSpinZ)
{
	 m_fTX=a_fTX;
	 m_fTY=a_fTY;
	 m_fTZ=a_fTZ;
			m_fSX=a_fSX;
			m_fSy=a_fSy;
			m_fSZ=a_fSZ;
			m_fSpinX=a_fSpinX;
			m_fSpinY=a_fSpinY;
			m_fSpinZ=a_fSpinZ;
		D3DXFROMWINEMATRIX mMatR;
D3DXFROMWINEMATRIX mMatS;
D3DXFROMWINEMATRIX mMatT;
		D3DXFROMWINEMatrixRotationYawPitchRoll(&mMatR,
			m_fSpinX,
			m_fSpinY,
			m_fSpinZ);
D3DXFROMWINEMatrixScaling(&mMatS,m_fSX,m_fSy,m_fSZ);
D3DXFROMWINEMatrixTranslation(&mMatT,m_fTX,m_fTY,m_fTZ);
D3DXFROMWINEMATRIX mMat = mMatR* mMatS *mMatT;
render( &mMat);
}

void MeshRenderer2::SetPlay(bool a){}
void MeshRenderer2::SetAnimIndex( unsigned int pAnimIndex){}
size_t MeshRenderer2::GetCurrentAnimIndex(){return 0;}
size_t MeshRenderer2::GetNumAnimations(){return 0;}

//-------------------------------------------------------------------------------
// Calculate the boundaries of a given node and all of its children
// The boundaries are in Worldspace (AABB)
// piNode Input node
// p_avOut Receives the min/max boundaries. Must point to 2 D3DXFROMWINEVECTOR3s
// piMatrix Transformation matrix of the graph at this position
//-------------------------------------------------------------------------------
int MeshRenderer2::CalculateBounds(D3DXFROMWINEVECTOR3* p_avOut,
	const D3DXFROMWINEMATRIX& piMatrix)
{
	/*
	D3DXFROMWINEMATRIX mTemp; // ??? = piNode->mTransformation;
	//mTemp.Transpose();
	D3DXFROMWINEMatrixIdentity(&mTemp);
	D3DXFROMWINEMATRIX aiMe = mTemp * piMatrix;
	*/
	for (unsigned int i = 0; i < meshes.size();++i)
	{
		for( unsigned int a = 0; a < meshes[i].vertices.size();++a)
		{
			D3DXFROMWINEVECTOR3 pc =meshes[i].vertices[a].Position;
			D3DXFROMWINEVECTOR3 pc1=pc;

			//D3DXFROMWINEVec3TransformCoord((D3DXFROMWINEVECTOR3*)&pc1,(D3DXFROMWINEVECTOR3*)&pc, (D3DXFROMWINEMATRIX*)&aiMe);

			p_avOut[0].x = std::min( p_avOut[0].x, pc1.x);
			p_avOut[0].y = std::min( p_avOut[0].y, pc1.y);
			p_avOut[0].z = std::min( p_avOut[0].z, pc1.z);
			p_avOut[1].x = std::max( p_avOut[1].x, pc1.x);
			p_avOut[1].y = std::max( p_avOut[1].y, pc1.y);
			p_avOut[1].z = std::max( p_avOut[1].z, pc1.z);
		}
	} /*
	for (unsigned int i = 0; i < piNode->mNumChildren;++i)
	{
		CalculateBounds( piNode->mChildren[i], p_avOut, aiMe );
	} */
	return 1;
}
//-------------------------------------------------------------------------------
// Scale the asset that it fits perfectly into the viewer window
// The function calculates the boundaries of the mesh and modifies the
// global world transformation matrix according to the aset AABB
//-------------------------------------------------------------------------------
void MeshRenderer2::GetBoundingSphereRadius(float& r)
{
	D3DXFROMWINEVECTOR3 ____min;
	D3DXFROMWINEVECTOR3 ____max;
	GetBounds(____min, ____max);
	D3DXFROMWINEVECTOR3 vDelta = ____max-____min;
	r = 0.5f*D3DXFROMWINEVec3Length(&vDelta);
}
void MeshRenderer2::GetBounds(D3DXFROMWINEVECTOR3& ____min, D3DXFROMWINEVECTOR3& ____max)
{
	D3DXFROMWINEVECTOR3 aiVecs[2] = {D3DXFROMWINEVECTOR3( 1e10f, 1e10f, 1e10f),
		D3DXFROMWINEVECTOR3( -1e10f, -1e10f, -1e10f) };

	{
		D3DXFROMWINEMATRIX m;
		CalculateBounds(aiVecs,m);
	}

	____min = *(D3DXFROMWINEVECTOR3*)&aiVecs[0];
	____max = *(D3DXFROMWINEVECTOR3*)&aiVecs[1];
}
    	D3DXFROMWINEVECTOR3 MeshRenderer2::GetMeshBBoxExtents( unsigned int )
	{
		D3DXFROMWINEVECTOR3 __min;
		D3DXFROMWINEVECTOR3 __max;
		GetBounds(__min, __max);
		return __max;
	}
    	D3DXFROMWINEVECTOR3 MeshRenderer2::GetMeshBBoxCenter( unsigned int )
	{
		D3DXFROMWINEVECTOR3 __min;
		D3DXFROMWINEVECTOR3 __max;
		GetBounds(__min, __max);
		return __max/2.0f;// is this correct ???
	}
D3DXFROMWINEMATRIX MeshRenderer2::ScaleAsset(void)
{
	D3DXFROMWINEVECTOR3 aiVecs[2] = {D3DXFROMWINEVECTOR3( 1e10f, 1e10f, 1e10f),
		D3DXFROMWINEVECTOR3( -1e10f, -1e10f, -1e10f) };

	{
		D3DXFROMWINEMATRIX m;
		CalculateBounds(aiVecs,m);
	}

	D3DXFROMWINEVECTOR3 vDelta = aiVecs[1]-aiVecs[0];
	D3DXFROMWINEVECTOR3 vHalf =  aiVecs[0] + (vDelta / 2.0f);
	float fScale = 2.0f / D3DXFROMWINEVec3Length(&vDelta);

	m_mWorld =  D3DXFROMWINEMATRIX(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-vHalf.x,-vHalf.y,-vHalf.z,1.0f) *
		D3DXFROMWINEMATRIX(
		fScale,0.0f,0.0f,0.0f,
		0.0f,fScale,0.0f,0.0f,
		0.0f,0.0f,fScale,0.0f,
		0.0f,0.0f,0.0f,1.0f);

	for (unsigned int i = 0; i < meshes.size();++i)
	{
		for( unsigned int j = 0; j < meshes[i].vertices.size();++j)
		{
			D3DXFROMWINEVec3TransformCoord(&meshes[i].vertices[j].Position, &meshes[i].vertices[j].Position, &m_mWorld);
		}
	}
	return m_mWorld;
}

#if 0
	void MeshRenderer2::RemoveHiddenTriangles2()
	{
		std::vector<__WORD__> indicestmp=indices;
		// ??? for(int i=indices.size()-1;i>=0;3*i--)
            	{
			D3DXFROMWINEVECTOR3 p[3],v[3],d[3];
			for(unsigned int j=0;j<indices.size()/3;j++)
            		{
				if(i==j)
					continue;
				p[0]=vertices[indices[3*j+0]].Position;
				p[1]=vertices[indices[3*j+1]].Position;
				p[2]=vertices[indices[3*j+2]].Position;
				v[0]=vertices[indices[3*i+0]].Position;
				v[1]=vertices[indices[3*i+1]].Position;
				v[2]=vertices[indices[3*i+2]].Position;

				for (unsigned int n=0;n<3;n++)
					for (unsigned int m=0;m<3;m++)
					{
						d[m]=v[n]-p[m];
						d[m]/=D3DXFROMWINEVec3Length(&d[m]);
					}
				for (unsigned int m=0;m<3;m++)
				{
					boor r=stx_RayIntersectsTriangle(p[m], d[m], v[0], v[1], v[2]);
					if(r) indicestmp.erase(indicestmp.begin() + 3*i, indicestmp.begin() + 3*i + 3);
				}̈́
			}
		}
		indices=indicestmp;
	}
#endif
	void MeshRenderer2::RemoveHiddenTriangles(const std::vector<D3DXFROMWINEVECTOR3>& minmax)
	{
		for (unsigned int k=0;k<minmax.size()/2;k++)
            	{
		for (unsigned int i=0;i<indices.size()/3;i++)
            	{
			int iRemoveTriangle=0;
			{D3DXFROMWINEVECTOR3 ap=vertices[indices[3*i+0]].Position;
			 D3DXFROMWINEVECTOR3 amin=minmax[2*k+0];
			 D3DXFROMWINEVECTOR3 amax=minmax[2*k+1];
		if((ap.x>amin.x)&&(ap.y>amin.y)&&(ap.z>amin.z)&&(ap.x<amax.x)&&(ap.y<amax.y)&&(ap.z<amax.z))
			{
				vertices.erase(vertices.begin() + indices[3*i+0]);
				iRemoveTriangle++;
			}}
			{D3DXFROMWINEVECTOR3 ap=vertices[indices[3*i+1]].Position;
			 D3DXFROMWINEVECTOR3 amin=minmax[2*k+0];
			 D3DXFROMWINEVECTOR3 amax=minmax[2*k+1];
		if((ap.x>amin.x)&&(ap.y>amin.y)&&(ap.z>amin.z)&&(ap.x<amax.x)&&(ap.y<amax.y)&&(ap.z<amax.z))
			{
				vertices.erase(vertices.begin() + indices[3*i+1]);
				iRemoveTriangle++;
			}}
			{D3DXFROMWINEVECTOR3 ap=vertices[indices[3*i+2]].Position;
			 D3DXFROMWINEVECTOR3 amin=minmax[2*k+0];
			 D3DXFROMWINEVECTOR3 amax=minmax[2*k+1];
		if((ap.x>amin.x)&&(ap.y>amin.y)&&(ap.z>amin.z)&&(ap.x<amax.x)&&(ap.y<amax.y)&&(ap.z<amax.z))
			{
				vertices.erase(vertices.begin() + indices[3*i+2]);
				iRemoveTriangle++;
			}}
			if(iRemoveTriangle==3)
				indices.erase(indices.begin() + 3*i, indices.begin() + 3*i + 3);

		}
	}}
	void MeshRenderer2::CreateCombinedShape(const std::vector<MeshRenderer2>& av, const std::vector<D3DXFROMWINEMATRIX>& aW)
	{
		Clear();
		std::vector<D3DXFROMWINEVECTOR3> minmax;
            	unsigned int i=0;
            	unsigned int ind0=0;
            	while(1)
		{
		if(i>=av.size())
			break;
		if(i>=aW.size())
			break;
		//LOG_PRINT("i=%d\n", i);
		for (unsigned int j=0;j<av[i].indices.size();j++)
            	{
		//LOG_PRINT("j=%d\n", j);
			D3DXFROMWINEVECTOR3 v1=av[i].vertices[av[i].indices[j]].Position;
			stx_VertexPositionNormalTexture v2;
			D3DXFROMWINEVec3TransformCoord(&v2.Position, &v1, &aW[i]);
			vertices.push_back(v2);
			indices.push_back(av[i].indices[j]+ind0);
		}
		minmax.push_back(_min);
		minmax.push_back(_max);
		ind0+=av[i].indices.size();
		i++;
		}
		RemoveHiddenTriangles(minmax);
		for(unsigned int i=0;i<vertices.size();i++)
			stx_CalculatePositionalSphericalMapping(vertices[i]);
	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());

	}
	unsigned int MeshRenderer2::getVertexCount(){return vertices.size();}

	unsigned int MeshRenderer2::getTriangleCount(){return indices.size()/3;}

	VertexFormatID MeshRenderer2::getVertexDeclaration(){return VertexDeclaration;}
	void MeshRenderer2::setVertexDeclaration(VertexFormatID aVertexDeclaration){VertexDeclaration=aVertexDeclaration;}

        unsigned int MeshRenderer2::Draw(D3DXFROMWINEMATRIX* amat, TextureID id, ShaderID shdid, VertexFormatID vfid, D3DXFROMWINEVECTOR4 AmbientColor, D3DXFROMWINEVECTOR4 DiffuseColor, D3DXFROMWINEVECTOR4 LightDir, D3DXFROMWINEVECTOR4 EyeDir)
        {
	j_tmp=0;
	z_tmp=0;
		BeginDraw(amat, id, shdid, vfid, AmbientColor, DiffuseColor, LightDir, EyeDir);
		EndDraw(RESET_ALL);
		return 0;
        }

        /// <summary>
        /// Fills vertices std::vector  with <see cref="stx_VertexPositionNormalTexture"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void MeshRenderer2::CreateTexturedBox(float width, float height, float depth, tShader aShader_)
        {
		Clear();
		m_Shape=eTexturedBox;
            // Because the box is centered at the origin, need to divide by two to find the + and - offsets
            width = width / 2.0f;
            height = height / 2.0f;
            depth = depth / 2.0f;

            D3DXFROMWINEVECTOR3 topLeftFront(-width, height, depth);
            D3DXFROMWINEVECTOR3 bottomLeftFront(-width, -height, depth);
            D3DXFROMWINEVECTOR3 topRightFront(width, height, depth);
            D3DXFROMWINEVECTOR3 bottomRightFront(width, -height, depth);
            D3DXFROMWINEVECTOR3 topLeftBack(-width, height, -depth);
            D3DXFROMWINEVECTOR3 topRightBack(width, height, -depth);
            D3DXFROMWINEVECTOR3 bottomLeftBack(-width, -height, -depth);
            D3DXFROMWINEVECTOR3 bottomRightBack(width, -height, -depth);

            D3DXFROMWINEVECTOR2 textureTopLeft(0.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureBottomLeft(0.0f, 1.0f);
            D3DXFROMWINEVECTOR2 textureTopRight(1.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureBottomRight(1.0f, 1.0f);

            D3DXFROMWINEVECTOR3 frontNormal(0.0f, 0.0f, 1.0f);
            D3DXFROMWINEVECTOR3 backNormal(0.0f, 0.0f, -1.0f);
            D3DXFROMWINEVECTOR3 topNormal(0.0f, 1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 bottomNormal(0.0f, -1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 leftNormal(-1.0f, 0.0f, 0.0f);
            D3DXFROMWINEVECTOR3 rightNormal(1.0f, 0.0f, 0.0f);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, backNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, rightNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, rightNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal, textureBottomLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomRight));
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal, textureTopLeft));
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal, textureTopRight));

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

            // Create the actual vertex buffer

	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();
	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;

        }
        void MeshRenderer2::CreateTexturedQuad(float width, float height, tShader aShader_)
        {
		Clear();
		m_Shape=eTexturedQuad;
            // Because the box is centered at the origin, need to divide by two to find the + and - offsets
            width = width / 2.0f;
            height = height / 2.0f;

            D3DXFROMWINEVECTOR3 topLeftFront(-width, height, 0.0f);
            D3DXFROMWINEVECTOR3 bottomLeftFront(-width, -height, 0.0f);
            D3DXFROMWINEVECTOR3 topRightFront(width, height, 0.0f);
            D3DXFROMWINEVECTOR3 bottomRightFront(width, -height, 0.0f);

            D3DXFROMWINEVECTOR2 textureTopLeft(0.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureTopRight(1.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureBottomLeft(0.0f, 1.0f);
            D3DXFROMWINEVECTOR2 textureBottomRight(1.0f, 1.0f);

            D3DXFROMWINEVECTOR3 frontNormal(0.0f, 0.0f, 1.0f);

            // Front face.
            vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal, textureTopLeft, D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
            vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal, textureTopRight, D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
            vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomLeft, D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
            vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal, textureBottomRight, D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));

            unsigned int indices_[] = { 1, 0, 2, 1, 2, 3};

	    for(unsigned int i=0;i<elementsOf(indices_);i++)
		indices.push_back(indices_[i]);



		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

            // Create the actual vertex buffer

	    // ??? computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();


	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;

        }
        /// <summary>
        /// Fills vertices std::vector  with <see cref="stx_VertexPositionNormalTexture"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void MeshRenderer2::CreateBox(float width, float height, float depth, tShader aShader_)
        {
		Clear();
		m_Shape=eBox;
            // Because the box is centered at the origin, need to divide by two to find the + and - offsets
            width = width / 2.0f;
            height = height / 2.0f;
            depth = depth / 2.0f;

            D3DXFROMWINEVECTOR3 topLeftFront(-width, height, depth);
            D3DXFROMWINEVECTOR3 bottomLeftFront(-width, -height, depth);
            D3DXFROMWINEVECTOR3 topRightFront(width, height, depth);
            D3DXFROMWINEVECTOR3 bottomRightFront(width, -height, depth);
            D3DXFROMWINEVECTOR3 topLeftBack(-width, height, -depth);
            D3DXFROMWINEVECTOR3 topRightBack(width, height, -depth);
            D3DXFROMWINEVECTOR3 bottomLeftBack(-width, -height, -depth);
            D3DXFROMWINEVECTOR3 bottomRightBack(width, -height, -depth);

            D3DXFROMWINEVECTOR3 frontNormal(0.0f, 0.0f, 1.0f);
            D3DXFROMWINEVECTOR3 backNormal(0.0f, 0.0f, -1.0f);
            D3DXFROMWINEVECTOR3 topNormal(0.0f, 1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 bottomNormal(0.0f, -1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 leftNormal(-1.0f, 0.0f, 0.0f);
            D3DXFROMWINEVECTOR3 rightNormal(1.0f, 0.0f, 0.0f);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, rightNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightBack, rightNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topRightFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(bottomLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftBack, backNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(topLeftFront, frontNormal));
		stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

	//	0 1 3 3 2 0
		indices.push_back(vertices.size()-4);
		indices.push_back(vertices.size()-3);
		indices.push_back(vertices.size()-1);

		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size()-2);
		indices.push_back(vertices.size()-4);

		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

            // Create the actual vertex buffer
	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
        }

#if 0
// http://www.songho.ca/opengl/gl_sphere.html
///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void MeshRenderer2::buildVerticesSmooth(float radius, int sectorCount, int stackCount, tShader aShader)
{
    const float PI = 3.1415926f;

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            D3DXFROMWINEVECTOR3 v(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            D3DXFROMWINEVECTOR3 n(nx, ny, nz);

            // vertex tex coord between [0, 1]
            D3DXFROMWINEVECTOR2 uv((float)j / sectorCount, (float)i / stackCount);
	        vertices.push_back(stx_VertexPositionNormalTexture(v, n, uv));
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                indices.push_back(k1);   // k1---k2---k1+1
                indices.push_back(k2);
                indices.push_back(k1+1);
            }

            if(i != (stackCount-1))
            {
                indices.push_back(k1+1); // k1+1---k2---k2+1
                indices.push_back(k2);
                indices.push_back(k2+1);
            }

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    //buildInterleavedVertices();
}



///////////////////////////////////////////////////////////////////////////////
// generate vertices with flat shading
// each triangle is independent (no shared vertices)
///////////////////////////////////////////////////////////////////////////////
void MeshRenderer2::buildVerticesFlat(float radius, int sectorCount, int stackCount, tShader aShader)
{
    const float PI = 3.1415926f;

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);       // r * cos(u)
        float z = radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            vertex.s = (float)j/sectorCount;        // s
            vertex.t = (float)i/stackCount;         // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
    std::vector<float> n;                           // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for(i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if(i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                D3DXFROMWINEVECTOR3 V1(v1.x, v1.y, v1.z);
                D3DXFROMWINEVECTOR3 V2(v2.x, v2.y, v2.z);
                D3DXFROMWINEVECTOR3 V3(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                D3DXFROMWINEVECTOR2 uv1(v1.s, v1.t);
                D3DXFROMWINEVECTOR2 uv2(v2.s, v2.t);
                D3DXFROMWINEVECTOR2 uv3(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);
                D3DXFROMWINEVECTOR3 N(n[0], n[1], n[2]);
	            vertices.push_back(stx_VertexPositionNormalTexture(V1, N, uv1));
	            vertices.push_back(stx_VertexPositionNormalTexture(V2, N, uv2));
	            vertices.push_back(stx_VertexPositionNormalTexture(V3, N, uv3));

                // put indices of 1 triangle
                indices.push_back(index);
                indices.push_back(index+1);
                indices.push_back(index+2);

                // indices for line (first stack requires only vertical line)
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);

                index += 3;     // for next
            }
            else if(i == (stackCount-1)) // a triangle for last stack =========
            {
                // put a triangle
                D3DXFROMWINEVECTOR3 V1(v1.x, v1.y, v1.z);
                D3DXFROMWINEVECTOR3 V2(v2.x, v2.y, v2.z);
                D3DXFROMWINEVECTOR3 V3(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                D3DXFROMWINEVECTOR2 uv1(v1.s, v1.t);
                D3DXFROMWINEVECTOR2 uv2(v2.s, v2.t);
                D3DXFROMWINEVECTOR2 uv3(v3.s, v3.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                D3DXFROMWINEVECTOR3 N(n[0], n[1], n[2]);
	            vertices.push_back(stx_VertexPositionNormalTexture(V1, N, uv1));
	            vertices.push_back(stx_VertexPositionNormalTexture(V2, N, uv2));
	            vertices.push_back(stx_VertexPositionNormalTexture(V3, N, uv3));

                // put indices of 1 triangle
                indices.push_back(index);
                indices.push_back(index+1);
                indices.push_back(index+2);

                // indices for lines (last stack requires both vert/hori lines)
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                D3DXFROMWINEVECTOR3 V1(v1.x, v1.y, v1.z);
                D3DXFROMWINEVECTOR3 V2(v2.x, v2.y, v2.z);
                D3DXFROMWINEVECTOR3 V3(v3.x, v3.y, v3.z);
                D3DXFROMWINEVECTOR3 V4(v4.x, v4.y, v4.z);

                // put tex coords of quad
                D3DXFROMWINEVECTOR2 uv1(v1.s, v1.t);
                D3DXFROMWINEVECTOR2 uv2(v2.s, v2.t);
                D3DXFROMWINEVECTOR2 uv3(v3.s, v3.t);
                D3DXFROMWINEVECTOR2 uv4(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                D3DXFROMWINEVECTOR3 N(n[0], n[1], n[2]);
	            vertices.push_back(stx_VertexPositionNormalTexture(V1, N, uv1));
	            vertices.push_back(stx_VertexPositionNormalTexture(V2, N, uv2));
	            vertices.push_back(stx_VertexPositionNormalTexture(V3, N, uv3));
	            vertices.push_back(stx_VertexPositionNormalTexture(V4, N, uv4));

                // put indices of quad (2 triangles)
                indices.push_back(index);
                indices.push_back(index+1);
                indices.push_back(index+2);
                indices.push_back(index+2);
                indices.push_back(index+1);
                indices.push_back(index+3);

                // indices for lines
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 4;     // for next
            }
        }
    }

    // generate interleaved vertex array as well
    //buildInterleavedVertices();
}

void MeshRenderer2::clearArrays()
{
    vertices.clear();
    indices.clear();
}///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
std::vector<float> MeshRenderer2::computeFaceNormal(float x1, float y1, float z1,  // v1
                                             float x2, float y2, float z2,  // v2
                                             float x3, float y3, float z3)  // v3
{
    const float EPSILON_ = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if(length > EPSILON_)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
#endif

        /// <summary>
        /// Fills vertices std::vector  with <see cref="stx_VertexPositionNormalTexture"/> vertices forming a sphere.
        /// </summary>
        /// <remarks>
        /// Draw with an INDEXED TriangleList.  The values to use are as follows:
        /// Vertex Count    = slices * (stacks - 1) + 2
        /// Primitive Count = slices * (stacks - 1) * 2
        /// </remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created sphere.</param>
        /// <param name="radius">Radius of the sphere. This value should be greater than or equal to 0.0f.</param>
        /// <param name="slices">Number of slices about the Y axis.</param>
        /// <param name="stacks">Number of stacks along the Y axis. Should be 2 or greater. (stack of 1 is just a cylinder)</param>
        void MeshRenderer2::CreateSphere(float radius, tShader aShader_)
        {
		int slices = (int) 10*(4 * stx_cbrt(radius) + 6);
        	int stacks = (int) 10*(4 * stx_cbrt(radius) + 6);
		CreateSphere_(radius, slices, stacks, aShader_);
	}
        void MeshRenderer2::CreateSphere_(float radius, int aslices, int stacks, tShader aShader_)
        {
		Clear();
		m_Shape=eSphere;
            unsigned int slices=aslices;//+1;
            #if 0
                buildVerticesSmooth(radius, slices, stacks, aShader_);
		D3DXFROMWINEVECTOR3 aiVecs[2];
		D3DXFROMWINEMATRIX m;
		//D3DXFROMWINEMatrixIdentity(&m);
	D3DXFROMWINEMatrixScaling(&m, radius, radius, radius);
		CalculateBounds(&aiVecs[0], &m);
            #elif 0
                buildVerticesFlat(radius, slices, stacks, aShader_);
		D3DXFROMWINEVECTOR3 aiVecs[2];
		D3DXFROMWINEMATRIX m;
		//D3DXFROMWINEMatrixIdentity(&m);
	D3DXFROMWINEMatrixScaling(&m, radius, radius, radius);
		CalculateBounds(&aiVecs[0], &m);
            #elif 1
	    float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float stackStep = D3DXFROMWINE_PI / stacks;
            unsigned int vertexCount = slices * (stacks - 1) + 2;
            unsigned int triangleCount = slices * (stacks - 1) * 2;
            unsigned int indexCount = triangleCount * 3;

            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, -radius, 0), D3DXFROMWINEVECTOR3(0, -1, 0)));
            float stackAngle = D3DXFROMWINE_PI - stackStep;
            for (unsigned int i = 0; i < stacks - 1; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
                    //NOTE: y and z were switched from normal spherical coordinates because the sphere is "oriented" along the Y axis as opposed to the Z axis
                    float z = (float)(radius * sinf(stackAngle) * sinf(sliceAngle));
                    float x = (float)(radius * sinf(stackAngle) * cosf(sliceAngle));
                    float y = (float)(radius * cosf(stackAngle));
                    D3DXFROMWINEVECTOR3 position(x, y, z);
			D3DXFROMWINEVECTOR3 normal(x, y, z);
			D3DXFROMWINEVec3Normalize(&normal, &normal);
    			float tu = asinf(normal.x)/D3DXFROMWINE_PI + 0.5;
    			float tv = -asinf(normal.y)/D3DXFROMWINE_PI - 0.5;

                   vertices.push_back(stx_VertexPositionNormalTexture(position, normal, D3DXFROMWINEVECTOR2(tu, tv), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
                    sliceAngle += sliceStep;
                }
                stackAngle -= stackStep;
            }
            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, radius, 0), D3DXFROMWINEVECTOR3(0, 1, 0)));
    
		D3DXFROMWINEVECTOR3 aiVecs[2];
		D3DXFROMWINEMATRIX m;
		//D3DXFROMWINEMatrixIdentity(&m);
	D3DXFROMWINEMatrixScaling(&m, radius, radius, radius); // comment this line out to avoid a bug ???
	D3DXFROMWINEMatrixIdentity(&m);
		CalculateBounds(&aiVecs[0], &m);

	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertexCount);
            CreateIndexBuffer(vertexCount, indexCount, slices);
#endif
	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);

        }

        void MeshRenderer2::CreateCone(float bottomRadius, float height, tShader aShader_)
        {
		Clear();
		m_Shape=eSphere;
        int slices = 20;//(int) 10*(4 * stx_cbrt(bottomRadius) + 6)
		CreateCone_(bottomRadius, height, slices, aShader_);
	}

        void MeshRenderer2::CreateCone_(float bottomRadius, float height, int slices, tShader aShader_)
        {
		Clear();
		m_Shape=eSphere;
        //int slices = 20;//(int) 10*(4 * stx_cbrt(bottomRadius) + 6)
        float angle = 2.0f*D3DXFROMWINE_PI / slices;
        vertices.resize((slices + 2));
        int i = 0;
        for (int s = 0; s < slices; s++) {
            vertices[i].Position.x = bottomRadius * cosf(s * angle);
            vertices[i].Position.y = -height / 2;
            vertices[i].Position.z = bottomRadius * sinf(s * angle);
		i++;
        }
        vertices[i].Position.x = vertices[i].Position.z = 0;
        vertices[i].Position.y = -height / 2;
		i++;
        vertices[i].Position.y = height / 2;
        vertices[i].Position.x = vertices[i].Position.z = 0;
		i++;
/*
        indices = new short[slices * 6];
        short p = 0;
        for (i = 0; i < indices.length - 6; i += 3) {
            indices[i++] = indices[i + 2] = p++;
            indices[i++] = indices[i + 2] = p;
            indices[i++] = indices[i + 2] = slices;
            indices[i + 2]++;
        }
        indices[i++] = indices[i + 2] = p;
        indices[i++] = indices[i + 2] = 0;
        indices[i++] = indices[i + 2] = slices;
        indices[i + 2]++;
*/
#if 1
        indices.resize(slices * 6);
        int p = 0;
        for (i = 0; i < indices.size() - 6; i += 3) {
            indices[i] = indices[i + 2] = p++;i++;
            indices[i] = indices[i + 2] = p;i++;
            indices[i] = indices[i + 2] = slices;i++;
            indices[i + 2]++;
        }
        indices[i] = indices[i + 2] = p;i++;
        indices[i] = indices[i + 2] = 0;i++;
        indices[i] = indices[i + 2] = slices;i++;
        indices[i + 2]++;
#endif

        for (i = 0; i < vertices.size();i++)
		stx_CalculatePositionalSphericalMapping(vertices[i]);
		D3DXFROMWINEVECTOR3 aiVecs[2];
		D3DXFROMWINEMATRIX m;
	D3DXFROMWINEMatrixScaling(&m, bottomRadius, bottomRadius, bottomRadius); // comment this line out to avoid a bug ???
	D3DXFROMWINEMatrixIdentity(&m);
		CalculateBounds(&aiVecs[0], &m);
	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());
            CreateIndexBuffer(vertices.size(), indices.size(), slices);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();


	if(aShader_!=eShaderNone)
		InitShader(aShader_);
        }

        void MeshRenderer2::CreateHalfSphere(float radius, tShader aShader_)
        {
		int slices = (int) 10*(4 * stx_cbrt(radius) + 6);
        	int stacks = (int) 10*(4 * stx_cbrt(radius) + 6);
		CreateHalfSphere_(radius, slices, stacks, aShader_);
	}

        void MeshRenderer2::CreateHalfSphere_(float radius, int slices, int stacks, tShader aShader_)
        {
		Clear();
		m_Shape=eHalfSphere;
		//int slices = (int) 10*(4 * stx_cbrt(radius) + 6);
        	//int stacks = (int) 10*(4 * stx_cbrt(radius) + 6);

	    float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float stackStep = 0.5f*D3DXFROMWINE_PI / stacks;
            unsigned int vertexCount = slices * (stacks - 1) + 2;
            unsigned int triangleCount = slices * (stacks - 1) * 2;
            unsigned int indexCount = triangleCount * 3;

            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, radius, 0), D3DXFROMWINEVECTOR3(0, 1, 0)));
            //float stackAngle = D3DXFROMWINE_PI - stackStep;
            float stackAngle = 0.0f;
            for (unsigned int i = 0; i < stacks - 1; i++)
            {
                float sliceAngle = 0.0f;
                for (unsigned int j = 0; j < slices; j++)
                {
                    //NOTE: y and z were switched from normal spherical coordinates because the sphere is "oriented" along the Y axis as opposed to the Z axis
                    float x = (float)(radius * sinf(stackAngle) * cosf(sliceAngle));
                    float y = (float)(radius * cosf(stackAngle));
                    float z = (float)(radius * sinf(stackAngle) * sinf(sliceAngle));
                    D3DXFROMWINEVECTOR3 position(x, y, z);
			D3DXFROMWINEVECTOR3 normal(x, y, z);
			D3DXFROMWINEVec3Normalize(&normal, &normal);
    			float tu = asinf(normal.x)/D3DXFROMWINE_PI + 0.5;
    			float tv = -asinf(normal.y)/D3DXFROMWINE_PI - 0.5;

                   vertices.push_back(stx_VertexPositionNormalTexture(position, normal, D3DXFROMWINEVECTOR2(tu, tv), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
                    sliceAngle += sliceStep;
                }
                stackAngle += stackStep;
            }
            //vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, radius, 0), D3DXFROMWINEVECTOR3(0, 1, 0)));

		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);
	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());
            CreateIndexBuffer(vertices.size(), indexCount, slices, false);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();
	if(aShader_!=eShaderNone)
		InitShader(aShader_);

        }
        /// <summary>
        /// Fills vertices std::vector  with <see cref="stx_VertexPositionNormalTexture"/> vertices forming a cylinder.
        /// </summary>
        /// <remarks>
        /// Draw with an INDEXED TriangleList.  The values to use are as follows:
        /// Vertex Count    = slices * (stacks + 1) + 2
        /// Primitive Count = slices * (stacks + 1) * 2
        /// </remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created cylinder.</param>
        /// <param name="bottomRadius">Radius at the negative Y end. Value should be greater than or equal to 0.0f.</param>
        /// <param name="topRadius">Radius at the positive Y end. Value should be greater than or equal to 0.0f.</param>
        /// <param name="length">Length of the cylinder along the Y-axis.</param>
        /// <param name="slices">Number of slices about the Y axis.</param>
        /// <param name="stacks">Number of stacks along the Y axis.</param>
        void MeshRenderer2::CreateCylinder(float bottomRadius, float topRadius, float length, tShader aShader_)
        {
		int slices = (int) 10*(4 * stx_cbrt(bottomRadius+topRadius) + 6);
        	int stacks = (int) 10*(4 * stx_cbrt(bottomRadius+topRadius) + 6);
		CreateCylinder_(bottomRadius, topRadius, length, slices, stacks, aShader_);
	}
        void MeshRenderer2::CreateCylinder_(float bottomRadius, float topRadius, float length, int slices, int stacks, tShader aShader_)
        {
		Clear();
		m_Shape=eCylinder;
		//int slices = (int) 10*(4 * stx_cbrt(bottomRadius+topRadius) + 6);
        	//int stacks = (int) 10*(4 * stx_cbrt(bottomRadius+topRadius) + 6);
		//LOG_PRINT("slices = %d\n", slices);
		//LOG_PRINT("stacks = %d\n", stacks);

            // if both the top and bottom have a radius of zero, just return 0, because invalid
            if (bottomRadius <= 0 && topRadius <= 0)
            {

            }

            float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float heightStep = length / stacks;
            float radiusStep = (topRadius - bottomRadius) / stacks;
            float currentHeight = -length / 2;
            unsigned int vertexCount = (stacks + 1) * slices + 2;   //cone = stacks * slices + 1
            unsigned int triangleCount = (stacks + 1) * slices * 2; //cone = stacks * slices * 2 + slices
            unsigned int indexCount = triangleCount * 3;
            float currentRadius = bottomRadius;

            // Start at the bottom of the cylinder
            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, currentHeight, 0), D3DXFROMWINEVECTOR3(0, -1, 0)));
	stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
            for (unsigned int i = 0; i <= stacks; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
                    float x = currentRadius * (float)cosf(sliceAngle);
                    float y = currentHeight;
                    float z = currentRadius * (float)sinf(sliceAngle);

                    D3DXFROMWINEVECTOR3 position(x, y, z);
			D3DXFROMWINEVECTOR3 normal(x, y, z);

			D3DXFROMWINEVec3Normalize(&normal, &normal);

                    vertices.push_back(stx_VertexPositionNormalTexture(position, normal));
	stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

                    sliceAngle += sliceStep;
                }
                currentHeight += heightStep;
                currentRadius += radiusStep;
            }
            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, length / 2, 0), D3DXFROMWINEVECTOR3(0, 1, 0)));
	stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);

		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

            // Create the actual vertex buffer object
            computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertexCount);
            CreateIndexBuffer(vertexCount, indexCount, slices);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);

        }

        /// <summary>
        /// Fills indices std::vector  for spherical shapes like Spheres, Cylinders, and Cones.
        /// </summary>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created cylinder.</param>
        /// <param name="vertexCount">The total number of vertices making up the </param>
        /// <param name="indexCount">The total number of indices making up the </param>
        /// <param name="slices">The number of slices about the Y axis.</param>
        /// <returns>The index buffer containing the index data for the </returns>
        void MeshRenderer2::CreateIndexBuffer(unsigned int vertexCount, unsigned int indexCount, unsigned int slices, bool abBottom)
        {
            indices.resize(indexCount);
            unsigned int currentIndex = 0;

            // Bottom circle/cone of shape
            for (unsigned int i = 1; i <= slices; i++)
            {
                indices[currentIndex++] = 0;
                indices[currentIndex++] = i;
                if (i - 1 == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
            }

            // Middle sides of shape
            for (unsigned int i = 1; i < vertexCount - (unsigned int)slices - 1; i++)
            {
                indices[currentIndex++] = i;
                indices[currentIndex++] = i + (unsigned int)slices;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i + (unsigned int)slices - 1;

                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
                indices[currentIndex++] = i;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
            }

            // Top circle/cone of shape
		if(abBottom)
            for (unsigned int i = (unsigned int)vertexCount - (unsigned int)slices - 1; i < vertexCount - 1; i++)
            {
                indices[currentIndex++] = (unsigned int)vertexCount - 1;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
                indices[currentIndex++] = i;
            }
        }
typedef int (*sortFunction)(const void*, const void*);
int comparePosition(float *a, float *b)
{
	return stx_memcmp(a, b, sizeof(D3DXFROMWINEVECTOR3));
}

void MeshRenderer2::computeNormals(void* aVertices, unsigned int aVertexSizeInBytes, unsigned int avertexCount, bool aSmoothGroup)
{
		return;
	if(aVertexSizeInBytes<sizeof(D3DXFROMWINEVECTOR3)*2)
		return;

  __WORD__ vertexIndex;

  std::vector<float> faceNormals;
  float *normalVecs  = 0,
        *texCoords   = 0,
        *vertices    = 0,
         magnitude   = 0,
         normal[3],
         v0[3],
         v1[3];

  int (*TriangleRefs)[32];

  int  i, j, k,
       count  = 0,
       vertex = 0,
       result = 0;

  unsigned int *pFaces  = 0;

  //D3DXFROMWINEVECTOR3* meshnormalVecs  = new D3DXFROMWINEVECTOR3[getVertexCount()];
  normalVecs        = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3);
  vertices          = (float*)&aVertices;

  if(!aSmoothGroup)
  {
    for(i=0;i<getTriangleCount();i++)
    {

      v0[0] = vertices[3*pFaces[3*i+1]+0] - vertices[3*pFaces[3*i+0]+0];
      v0[1] = vertices[3*pFaces[3*i+1]+1] - vertices[3*pFaces[3*i+0]+1];
      v0[2] = vertices[3*pFaces[3*i+1]+2] - vertices[3*pFaces[3*i+0]+2];

      v1[0] = vertices[3*pFaces[3*i+2]+0] - vertices[3*pFaces[3*i+0]+0];
      v1[1] = vertices[3*pFaces[3*i+2]+1] - vertices[3*pFaces[3*i+0]+1];
      v1[2] = vertices[3*pFaces[3*i+2]+2] - vertices[3*pFaces[3*i+0]+2];

      normal[0] = (v0[1]*v1[2]-v0[2]*v1[1]);
      normal[1] = (v0[2]*v1[0]-v0[0]*v1[2]);
      normal[2] = (v0[0]*v1[1]-v0[1]*v1[0]);

      magnitude = sqrtf(normal[0]*normal[0] +
                                 normal[1]*normal[1] +
                                 normal[2]*normal[2]);

      if(magnitude > .00001f){
        magnitude  = 1.0f/magnitude;
        normal[0] *= magnitude;
        normal[1] *= magnitude;
        normal[2] *= magnitude;
      }

      for(j=0;j<3;j++){
        normalVecs[3*pFaces[3*i+j]+0] += normal[0];
        normalVecs[3*pFaces[3*i+j]+1] += normal[1];
        normalVecs[3*pFaces[3*i+j]+2] += normal[2];
      }
    }
    return;
  }

#if 0
  //indexArray(&vertexIndex, (char *)vertices, sizeof(float)*3, getVertexCount(), (sortFunction)comparePosition);

  TriangleRefs = (int (*)[32])malloc(sizeof(int)*32*vertexIndex.count);

  if(TriangleRefs==0)
    return;

  stx_memset(TriangleRefs, 0, sizeof(int)*32*vertexIndex.count);

  for(i=0;i<getTriangleCount();i++)
  {
    for(j=0;j<3;j++){
     vertex = indexFind(&vertexIndex, &vertices[3*pFaces[3*i+j]], &result);

      if(TriangleRefs[vertex][0]<48){
        TriangleRefs[vertex][0]++;
        TriangleRefs[vertex][TriangleRefs[vertex][0]]=i;
      }
    }
  }
#endif

  faceNormals.resize(3*getTriangleCount());

  for(i=0;i<getTriangleCount();i++)
  {
    v0[0] = vertices[3*pFaces[3*i+1]+0] - vertices[3*pFaces[3*i+0]+0];
    v0[1] = vertices[3*pFaces[3*i+1]+1] - vertices[3*pFaces[3*i+0]+1];
    v0[2] = vertices[3*pFaces[3*i+1]+2] - vertices[3*pFaces[3*i+0]+2];

    v1[0] = vertices[3*pFaces[3*i+2]+0] - vertices[3*pFaces[3*i+0]+0];
    v1[1] = vertices[3*pFaces[3*i+2]+1] - vertices[3*pFaces[3*i+0]+1];
    v1[2] = vertices[3*pFaces[3*i+2]+2] - vertices[3*pFaces[3*i+0]+2];

    faceNormals[3*i+0] = (v0[1]*v1[2]-v0[2]*v1[1]);
    faceNormals[3*i+1] = (v0[2]*v1[0]-v0[0]*v1[2]);
    faceNormals[3*i+2] = (v0[0]*v1[1]-v0[1]*v1[0]);

    magnitude = sqrtf(faceNormals[3*i+0]*faceNormals[3*i+0] +
                               faceNormals[3*i+1]*faceNormals[3*i+1] +
                               faceNormals[3*i+2]*faceNormals[3*i+2]);

    if(magnitude)
    {
      magnitude           = 1.0f/magnitude;
      faceNormals[3*i+0] *= magnitude;
      faceNormals[3*i+1] *= magnitude;
      faceNormals[3*i+2] *= magnitude;
    }
  }
#if 0
  for(i=0;i<getTriangleCount();i++)
  {
    for(j=0;j<3;j++){

      vertex = indexFind(&vertexIndex, (void *)&vertices[3*pFaces[3*i+j]], &result);
      count  = 0;

      for(k=1; k <= TriangleRefs[vertex][0];k++)
      {
        if(aSmoothGroup[i] == aSmoothGroup[TriangleRefs[vertex][k]])
        {
          normalVecs[3*pFaces[3*i+j]+0] += faceNormals[3*TriangleRefs[vertex][k]+0];
          normalVecs[3*pFaces[3*i+j]+1] += faceNormals[3*TriangleRefs[vertex][k]+1];
          normalVecs[3*pFaces[3*i+j]+2] += faceNormals[3*TriangleRefs[vertex][k]+2];
          count++;
        }
      }

      normalVecs[3*pFaces[3*i+j]+0] /= (float)count;
      normalVecs[3*pFaces[3*i+j]+1] /= (float)count;
      normalVecs[3*pFaces[3*i+j]+2] /= (float)count;
    }
  }
  indexFree(&vertexIndex);

  stx_free(TriangleRefs);

#endif
}

void MeshRenderer2::computeTangentSpace(void* aVertices, unsigned int aVertexSizeInBytes, unsigned int avertexCount, bool aSmoothGroup)
{
	//LOG_FNLN;
	//LOG_PRINT("aVertices=%x\n", aVertices);
	//LOG_PRINT("vertices.size()=%d\n", vertices.size());
	//LOG_PRINT("indices.size()=%d\n", indices.size());
	//LOG_PRINT("Triangle Count=%d\n", getTriangleCount());
	//LOG_PRINT("avertexCount=%d\n", avertexCount);
	//LOG_PRINT("aVertexSizeInBytes=%d\n", aVertexSizeInBytes);
	if(aVertexSizeInBytes<sizeof(D3DXFROMWINEVECTOR3)*4)
		return;
#if 0
  if(!Mesh->texCoords){
    computeNormals(aVertices, aVertexSizeInBytes, avertexCount);
    return;
  }
#endif

	int i, j, k, Count, Vertex=0, Result=0;
	__WORD__  VertexIndex;
	int (*TriangleRefs)[32];
	std::vector<float> FaceTangent;
	std::vector<float> FaceBinormal;
	std::vector<float> FaceNormal;
	float v0[3], v1[3], cross[3];
  float *pTexCoords = 0,
        *pBiNormals = 0,
        *pVertices  = 0,
        *pTangents  = 0,
        *pNormals   = 0,
        Mag;

	D3DXFROMWINEVECTOR3 Tangent,
          Binormal,
          Normal;

  __WORD__   *pFaces = 0;

  //meshnormalVecs  = new D3DXFROMWINEVECTOR3[getVertexCount()];
  float* normalVecs        = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3);
  float* vertices          = (float*)&aVertices;

  pBiNormals         = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3)*2;
  pTexCoords         = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3)*4;
  pTangents          = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3)*3;
  pVertices          = (float*)&aVertices;
  pNormals           = (float*)&aVertices+sizeof(D3DXFROMWINEVECTOR3);
  pFaces             = (__WORD__*)&indices[0];

  if(aSmoothGroup)
	{
		for(i=0;i<getTriangleCount();i++)
		{
			v0[0]=pVertices[3*pFaces[3*i+1]+0]-pVertices[3*pFaces[3*i+0]+0];
			v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			v1[0]=pVertices[3*pFaces[3*i+2]+0]-pVertices[3*pFaces[3*i+0]+0];
			v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			if(fabs(cross[0])>0.000001f)
			{
				Tangent[0]  =-cross[1]/cross[0];
				Binormal[0] =cross[2]/cross[0];
			}

			v0[0]=pVertices[3*pFaces[3*i+1]+1]-pVertices[3*pFaces[3*i+0]+1];
			v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			v1[0]=pVertices[3*pFaces[3*i+2]+1]-pVertices[3*pFaces[3*i+0]+1];
			v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			if(fabs(cross[0])>0.000001f)
			{
				Tangent[1]=-cross[1]/cross[0];
				Binormal[1]=cross[2]/cross[0];
			}

			v0[0]=pVertices[3*pFaces[3*i+1]+2]-pVertices[3*pFaces[3*i+0]+2];
			v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			v1[0]=pVertices[3*pFaces[3*i+2]+2]-pVertices[3*pFaces[3*i+0]+2];
			v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
			v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

			cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			if(fabs(cross[0])>0.000001f)
			{
				Tangent[2]=-cross[1]/cross[0];
				Binormal[2]=cross[2]/cross[0];
			}

			Mag = sqrtf(Tangent[0]*Tangent[0]+Tangent[1]*Tangent[1]+Tangent[2]*Tangent[2]);

			if(Mag)
			{
				Mag=1.0f/Mag;
				Tangent[0]*=Mag;
				Tangent[1]*=Mag;
				Tangent[2]*=Mag;
			}

			Mag= sqrtf(Binormal[0]*Binormal[0]+Binormal[1]*Binormal[1]+Binormal[2]*Binormal[2]);

			if(Mag)
			{
				Mag=1.0f/Mag;
				Binormal[0]*=Mag;
				Binormal[1]*=Mag;
				Binormal[2]*=Mag;
			}

			v0[0]=pVertices[3*pFaces[3*i+1]+0]-pVertices[3*pFaces[3*i+0]+0];
			v0[1]=pVertices[3*pFaces[3*i+1]+1]-pVertices[3*pFaces[3*i+0]+1];
			v0[2]=pVertices[3*pFaces[3*i+1]+2]-pVertices[3*pFaces[3*i+0]+2];

			v1[0]=pVertices[3*pFaces[3*i+2]+0]-pVertices[3*pFaces[3*i+0]+0];
			v1[1]=pVertices[3*pFaces[3*i+2]+1]-pVertices[3*pFaces[3*i+0]+1];
			v1[2]=pVertices[3*pFaces[3*i+2]+2]-pVertices[3*pFaces[3*i+0]+2];

			Normal[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			Normal[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			Normal[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			Mag = sqrtf(Normal[0]*Normal[0]+Normal[1]*Normal[1]+Normal[2]*Normal[2]);

			if(Mag)
			{
				Mag=1.0f/Mag;
				Normal[0]*=Mag;
				Normal[1]*=Mag;
				Normal[2]*=Mag;
			}

			for(j=0;j<3;j++)
			{
				pTangents[3*pFaces[3*i+j]+0]+=Tangent[0];
				pTangents[3*pFaces[3*i+j]+1]+=Tangent[1];
				pTangents[3*pFaces[3*i+j]+2]+=Tangent[2];

				pBiNormals[3*pFaces[3*i+j]+0]+=Binormal[0];
				pBiNormals[3*pFaces[3*i+j]+1]+=Binormal[1];
				pBiNormals[3*pFaces[3*i+j]+2]+=Binormal[2];

				pNormals[3*pFaces[3*i+j]+0]+=Normal[0];
				pNormals[3*pFaces[3*i+j]+1]+=Normal[1];
				pNormals[3*pFaces[3*i+j]+2]+=Normal[2];
			}
		}

		return;
	}
#if 0
 	//indexArray(&VertexIndex, (char *)pVertices, sizeof(float)*3, Mesh->NumVertex, (sortFunction)comparePosition);

	TriangleRefs=(int (*)[32])malloc(sizeof(int)*32*VertexIndex.count);

	if(TriangleRefs==0)
		return;

	stx_memset(TriangleRefs, 0, sizeof(int)*32*VertexIndex.count);

	for(i=0;i<getTriangleCount();i++)
	{
		for(j=0;j<3;j++)
		{
			Vertex=indexFind(&VertexIndex, &pVertices[3*pFaces[3*i+j]], &Result);

			if(TriangleRefs[Vertex][0]<48)
			{
				TriangleRefs[Vertex][0]++;
				TriangleRefs[Vertex][TriangleRefs[Vertex][0]]=i;
			}
		}
	}
#endif
	FaceTangent.resize(3*getTriangleCount());

	FaceBinormal.resize(3*getTriangleCount());

	FaceNormal.resize(3*getTriangleCount());

	for(i=0;i<getTriangleCount();i++)
	{
	//LOG_PRINT("i=%d\n", i);
		v0[0]=pVertices[3*pFaces[3*i+1]+0]-pVertices[3*pFaces[3*i+0]+0];
		v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		v1[0]=pVertices[3*pFaces[3*i+2]+0]-pVertices[3*pFaces[3*i+0]+0];
		v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			FaceTangent[3*i+0]=-cross[1]/cross[0];
			FaceBinormal[3*i+0]=cross[2]/cross[0];
		}

		v0[0]=pVertices[3*pFaces[3*i+1]+1]-pVertices[3*pFaces[3*i+0]+1];
		v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		v1[0]=pVertices[3*pFaces[3*i+2]+1]-pVertices[3*pFaces[3*i+0]+1];
		v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			FaceTangent[3*i+1]=-cross[1]/cross[0];
			FaceBinormal[3*i+1]=cross[2]/cross[0];
		}

		v0[0]=pVertices[3*pFaces[3*i+1]+2]-pVertices[3*pFaces[3*i+0]+2];
		v0[1]=pTexCoords[2*pFaces[3*i+1]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v0[2]=pTexCoords[2*pFaces[3*i+1]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		v1[0]=pVertices[3*pFaces[3*i+2]+2]-pVertices[3*pFaces[3*i+0]+2];
		v1[1]=pTexCoords[2*pFaces[3*i+2]+0]-pTexCoords[2*pFaces[3*i+0]+0];
		v1[2]=pTexCoords[2*pFaces[3*i+2]+1]-pTexCoords[2*pFaces[3*i+0]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			FaceTangent[3*i+2]=-cross[1]/cross[0];
			FaceBinormal[3*i+2]=cross[2]/cross[0];
		}

		Mag=(float)sqrt(FaceTangent[3*i+0]*FaceTangent[3*i+0]+FaceTangent[3*i+1]*FaceTangent[3*i+1]+FaceTangent[3*i+2]*FaceTangent[3*i+2]);

		if(Mag)
		{
			Mag=1.0f/Mag;
			FaceTangent[3*i+0]*=Mag;
			FaceTangent[3*i+1]*=Mag;
			FaceTangent[3*i+2]*=Mag;
		}

		Mag=(float)sqrt(FaceBinormal[3*i+0]*FaceBinormal[3*i+0]+FaceBinormal[3*i+1]*FaceBinormal[3*i+1]+FaceBinormal[3*i+2]*FaceBinormal[3*i+2]);

		if(Mag)
		{
			Mag=1.0f/Mag;
			FaceBinormal[3*i+0]*=Mag;
			FaceBinormal[3*i+1]*=Mag;
			FaceBinormal[3*i+2]*=Mag;
		}

		v0[0]=pVertices[3*pFaces[3*i+1]+0]-pVertices[3*pFaces[3*i+0]+0];
		v0[1]=pVertices[3*pFaces[3*i+1]+1]-pVertices[3*pFaces[3*i+0]+1];
		v0[2]=pVertices[3*pFaces[3*i+1]+2]-pVertices[3*pFaces[3*i+0]+2];

		v1[0]=pVertices[3*pFaces[3*i+2]+0]-pVertices[3*pFaces[3*i+0]+0];
		v1[1]=pVertices[3*pFaces[3*i+2]+1]-pVertices[3*pFaces[3*i+0]+1];
		v1[2]=pVertices[3*pFaces[3*i+2]+2]-pVertices[3*pFaces[3*i+0]+2];

		FaceNormal[3*i+0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		FaceNormal[3*i+1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		FaceNormal[3*i+2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		Mag=(float)sqrt(FaceNormal[3*i+0]*FaceNormal[3*i+0]+FaceNormal[3*i+1]*FaceNormal[3*i+1]+FaceNormal[3*i+2]*FaceNormal[3*i+2]);

		if(Mag)
		{
			Mag=1.0f/Mag;
			FaceNormal[3*i+0]*=Mag;
			FaceNormal[3*i+1]*=Mag;
			FaceNormal[3*i+2]*=Mag;
		}
	}

#if 0

	for(i=0;i<getTriangleCount();i++)
	{
		for(j=0;j<3;j++)
		{
			Vertex=indexFind(&VertexIndex, (void *)&pVertices[3*pFaces[3*i+j]], &Result);

			Count=0;

			for(k=1;k<=TriangleRefs[Vertex][0];k++)
			{
				if(Mesh->smoothGroup[i]==Mesh->smoothGroup[TriangleRefs[Vertex][k]])
				{
					pTangents[3*pFaces[3*i+j]+0]+=FaceTangent[3*TriangleRefs[Vertex][k]+0];
					pTangents[3*pFaces[3*i+j]+1]+=FaceTangent[3*TriangleRefs[Vertex][k]+1];
					pTangents[3*pFaces[3*i+j]+2]+=FaceTangent[3*TriangleRefs[Vertex][k]+2];

					pBiNormals[3*pFaces[3*i+j]+0]+=FaceBinormal[3*TriangleRefs[Vertex][k]+0];
					pBiNormals[3*pFaces[3*i+j]+1]+=FaceBinormal[3*TriangleRefs[Vertex][k]+1];
					pBiNormals[3*pFaces[3*i+j]+2]+=FaceBinormal[3*TriangleRefs[Vertex][k]+2];

					pNormals[3*pFaces[3*i+j]+0]+=FaceNormal[3*TriangleRefs[Vertex][k]+0];
					pNormals[3*pFaces[3*i+j]+1]+=FaceNormal[3*TriangleRefs[Vertex][k]+1];
					pNormals[3*pFaces[3*i+j]+2]+=FaceNormal[3*TriangleRefs[Vertex][k]+2];

					Count++;
				}
			}

			pTangents[3*pFaces[3*i+j]+0]/=(float)Count;
			pTangents[3*pFaces[3*i+j]+1]/=(float)Count;
			pTangents[3*pFaces[3*i+j]+2]/=(float)Count;

			pBiNormals[3*pFaces[3*i+j]+0]/=(float)Count;
			pBiNormals[3*pFaces[3*i+j]+1]/=(float)Count;
			pBiNormals[3*pFaces[3*i+j]+2]/=(float)Count;

			pNormals[3*pFaces[3*i+j]+0]/=(float)Count;
			pNormals[3*pFaces[3*i+j]+1]/=(float)Count;
			pNormals[3*pFaces[3*i+j]+2]/=(float)Count;
		}
	}
	indexFree(&VertexIndex);

	stx_free(TriangleRefs);

#endif
}

D3DXFROMWINEVECTOR3 v4tov3(D3DXFROMWINEVECTOR4& a)
{
	D3DXFROMWINEVECTOR3 r;
	r.x=a.x;
	r.y=a.y;
	r.z=a.z;
	return r;
}
void MeshRenderer2::Grid(std::vector<stx_VertexPositionNormalTexture>& vertices, int gridX, int gridZ)
{
	vertices.resize(0);//(gridX * gridZ);
	indices.resize(0);
	//mesh->resize(gridX * gridZ, (gridX - 1) * (gridZ - 1) * 2);
	int vertCnt = 0;
	int faceCnt = 0;
	for (int x = 0; x < gridX; x++)
		for (int z = 0; z < gridZ; z++)
		{
			vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1 - (float) x / (gridX - 1), 0, 1 - (float) z / (gridZ - 1))));
	stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
			if (x < gridX - 1 && z < gridZ - 1)
			{
				indices.push_back(vertCnt + 0);
				indices.push_back(vertCnt + 1);
				indices.push_back(vertCnt + 1 + gridZ);
				indices.push_back(vertCnt + 1 + gridZ);
				indices.push_back(vertCnt + gridZ);
				indices.push_back(vertCnt + 0);
			}
			vertCnt++;
		}

}

void MeshRenderer2::setFace(unsigned int c, unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

void MeshRenderer2::makeGrid(int gridX, int gridZ)
{
	int vertCnt = 0;
	int faceCnt = 0;
	for (int x = 0; x < gridX; x++)
		for (int z = 0; z < gridZ; z++)
		{
			vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1 - (float) x / (gridX - 1), 0, 1 - (float) z / (gridZ - 1))));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
			if (x < gridX - 1 && z < gridZ - 1)
			{
				setFace(
					faceCnt++, vertCnt + 0, vertCnt + 1, vertCnt + 1 + gridZ
					);
				setFace(
					faceCnt++, vertCnt + 1 + gridZ, vertCnt + gridZ, vertCnt + 0
					);
			}
			vertCnt++;
		}
}
void MeshRenderer2::CreateHemis(tShader aShader_)
{
	int stepLng = (int) 10*(4 * stx_cbrt(1.0f) + 6);
        int stepLat = (int) 10*(4 * stx_cbrt(1.0f) + 6);
	CreateHemis_(stepLng, stepLat, aShader_);
}
void MeshRenderer2::CreateHemis_(int stepLng, int stepLat, tShader aShader_)
{
		Clear();
	m_Shape=eHemis;
		//int stepLng = (int) 10*(4 * stx_cbrt(1.0f) + 6);
        	//int stepLat = (int) 10*(4 * stx_cbrt(1.0f) + 6);
	makeGrid(stepLng + 1, (stepLat / 4) + 2);
	float unitLng = 2 * D3DXFROMWINE_PI / stepLng;
	float unitLat = D3DXFROMWINE_PI / (stepLat / 2);
	int vertCnt = 0;
	for (int i = 0; i < stepLng + 1; i++)
	{
		for (int j = 0; j < (stepLat / 4) + 1; j++)
		{
			D3DXFROMWINEVECTOR3 v(0, 1, 0);
		D3DXFROMWINEMATRIX RM;
			D3DXFROMWINEMatrixRotationYawPitchRoll(&RM, j * unitLat, i * unitLng, 0.0f);
		D3DXFROMWINEVec3TransformCoord(&v, &v, &RM);
			vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(v.x, v.y, v.z)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
			indices.push_back(vertices.size()-1);
		}
		vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, 0, 0)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
		indices.push_back(vertices.size()-1);
	}
        //CreateIndexBuffer(vertices.size(), vertices.size(), stepLng);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;
}

#if 1
void MeshRenderer2::CreateTorus(float radMajor, float radMinor, tShader aShader_)
{
		int num_rings = (int) 10*(4 * stx_cbrt(radMinor) + 6);
        	int num_segments = (int) 10*(4 * stx_cbrt(radMajor) + 6);
	    int slices = (int) 10*(4 * stx_cbrt(radMinor) + 6);
            int stacks = (int) 10*(4 * stx_cbrt(radMajor) + 6);
	CreateTorus_(radMajor, radMinor, num_rings, num_segments, slices, stacks, aShader_);
}
void MeshRenderer2::CreateTorus_(float radMajor, float radMinor, int num_rings, int num_segments, int slices, int stacks, tShader aShader_)
{
		Clear();
	m_Shape=eTorus;
/*
		int num_rings = (int) 10*(4 * stx_cbrt(radMinor) + 6);
        	int num_segments = (int) 10*(4 * stx_cbrt(radMajor) + 6);
	    int slices = (int) 10*(4 * stx_cbrt(radMinor) + 6);
            int stacks = (int) 10*(4 * stx_cbrt(radMajor) + 6);
*/
	    float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float stackStep = D3DXFROMWINE_PI / stacks;
    // Compute torus vertices
    const float pi = D3DXFROMWINE_PI;
    const float r1 = 1.0f;
    const float r2 = 0.3f;
#if 1
    for(std::size_t i=0, index=0; i<=num_rings; ++i) {
      for(std::size_t j=0; j<=num_segments; ++j, ++index) {
#else
            float stackAngle = D3DXFROMWINE_PI - stackStep;
            for (unsigned int i = 0; i < stacks - 1; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
#endif
        // Compute texture coordinates (surface parameters)
        // Note that u is added to j. This creates a spiral pattern, which
        // allows us to draw the entire torus in one triangle strip without
        // using degenerate triangles.
        // (Yet we still need degenerate triangles to avoid texture
        // wrap-around)
        float u = float(i) / num_rings;
        float v = (float(j) + u) / num_segments;

        // Compute angles
        float u_angle = u * 2 * pi;
        float v_angle = v * 2 * pi;

        // Position
        float x = cos(u_angle) * (r1 + cos(v_angle) * r2);
        float y = sin(u_angle) * (r1 + cos(v_angle) * r2);
        float z = sin(v_angle) * r2;

        // Normal vector
        float nx = cos(u_angle) * cos(v_angle);
        float ny = sin(u_angle) * cos(v_angle);
        float nz = sin(v_angle);

	stx_VertexPositionNormalTexture v3;
        v3.Position = D3DXFROMWINEVECTOR3(x, y, z);
        v3.Normal = D3DXFROMWINEVECTOR3(nx, ny, nz);
        v3.Tex = D3DXFROMWINEVECTOR2(u, v);
	stx_CalculatePositionalSphericalMapping(v3);
	vertices.push_back(v3);
#if 1
      }
    }
#else
                   sliceAngle += sliceStep;
                }
                stackAngle -= stackStep;
            }
#endif
#if 1
	{
    int idx = 0;

	int nbrRingSegments=num_rings;
        int nbrTubeSegments=num_segments;

	int nbrRingSteps=num_rings;
        int nbrTubes=num_segments;

    indices.resize(nbrRingSegments * nbrTubeSegments * 6);
    for (int t = 0; t < nbrTubeSegments; t++) {
        for (int r = 0; r < nbrRingSegments; r++) {
            int idxTL = r + t * nbrRingSteps;
            int idxBL = r + (t + 1) * nbrRingSteps;
            int idxBR = r + 1 + (t + 1) * nbrRingSteps;
            int idxTR = r + 1 + t * nbrRingSteps;
            // Top left triangle defined in anti-clockwise order
            indices[idx++] = idxTL;   // Point
            indices[idx++] = idxBL;   // Point
            indices[idx++] = idxTR;   // Point
            // Bottom right triangle defined in anti-clockwise order
            indices[idx++] = idxBL;   // Point
            indices[idx++] = idxBR;   // Point
            indices[idx++] = idxTR;   // Point
        }
    }}
#elif 1
    for(std::size_t i=0, index=0; i<=vertices.size(); ++i) {
      indices.push_back((int)(i % vertices.size()));
      indices.push_back((int)((i + num_segments+1) % vertices.size()));
    }
#elif 0
	D3DXFROMWINEVECTOR3 aiVecs[2];
	D3DXFROMWINEMATRIX m;
	D3DXFROMWINEMatrixIdentity(&m);
	CalculateBounds(&aiVecs[0], &m);
	computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertices.size());
	//unsigned int slices=num_rings;
	unsigned int slices=num_segments;
            unsigned int triangleCount = num_rings * (num_segments - 1) * 2;
            unsigned int indexCount = triangleCount * 3;
        CreateIndexBuffer(vertices.size(), indexCount, slices);
#endif

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;
}
#endif

void MeshRenderer2::CreateOcta(tShader aShader_)
{
		Clear();
	m_Shape=eOcta;
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, 0, 0)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1, 0, 0)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1, 0, 1)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, 0, 1)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0.5f, -1, 0.5f)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0.5f, 1, 0.5f)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	setFace(0, 0, 1, 4);
	setFace(1, 1, 2, 4);
	setFace(2, 2, 3, 4);
	setFace(3, 3, 0, 4);
	setFace(4, 0, 5, 1);
	setFace(5, 1, 5, 2);
	setFace(6, 2, 5, 3);
	setFace(7, 3, 5, 0);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;
}
void MeshRenderer2::CreateTetra(tShader aShader_)
{
		Clear();
	m_Shape=eTetra;
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, 0, 0)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1, 1, 0)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, 1, 1)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(1, 0, 1)));
			stx_CalculatePositionalSphericalMapping(vertices[vertices.size()-1]);
	setFace(0, 0, 1, 3);
	setFace(1, 0, 2, 1);
	setFace(2, 0, 3, 2);
	setFace(3, 1, 2, 3);
	D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;
}

void MeshRenderer2::CreateEllipsoid(float a, float b, float c, tShader aShader_)
        {
		int slices = (int) 10*(4 * stx_cbrt(a) + 6);
        	int stacks = (int) 10*(4 * stx_cbrt(b) + 6);
	CreateEllipsoid_(a, b, c, slices, stacks, aShader_);
}

void MeshRenderer2::CreateEllipsoid_(float a, float b, float c, int slices, int stacks, tShader aShader_)
        {
		Clear();
		m_Shape=eEllipsoid;
		//int slices = (int) 10*(4 * stx_cbrt(a) + 6);
        	//int stacks = (int) 10*(4 * stx_cbrt(b) + 6);

	    float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float stackStep = D3DXFROMWINE_PI / stacks;
            unsigned int vertexCount = slices * (stacks - 1) + 2;
            unsigned int triangleCount = slices * (stacks - 1) * 2;
            unsigned int indexCount = triangleCount * 3;
            std::vector<stx_VertexPositionNormalTexture> sphereVertices(vertexCount);
            unsigned int currentVertex = 0;
            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, -c, 0), D3DXFROMWINEVECTOR3(0, -1, 0)));
            float stackAngle = D3DXFROMWINE_PI - stackStep;
            for (unsigned int i = 0; i < stacks - 1; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
                    //NOTE: y and z were switched from normal spherical coordinates because the sphere is "oriented" along the Y axis as opposed to the Z axis
                    float x = (float)a*cosf(sliceAngle)*sinf(stackAngle);
                    float y = (float)c*cosf(stackAngle);
                    float z = (float)b*sinf(sliceAngle)*sinf(stackAngle);
                    D3DXFROMWINEVECTOR3 position(x, y, z);
			D3DXFROMWINEVECTOR3 normal(x, y, z);
			D3DXFROMWINEVec3Normalize(&normal, &normal);
    			float tu = asinf(normal.x)/D3DXFROMWINE_PI + 0.5;
    			float tv = -asinf(normal.y)/D3DXFROMWINE_PI - 0.5;
                   vertices.push_back(stx_VertexPositionNormalTexture(position, normal, D3DXFROMWINEVECTOR2(tu, -(tv-1.0f)), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f), D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f)));
                    sliceAngle += sliceStep;
                }
                stackAngle -= stackStep;
            }
            vertices.push_back(stx_VertexPositionNormalTexture(D3DXFROMWINEVECTOR3(0, c, 0), D3DXFROMWINEVECTOR3(0, 1, 0)));

		D3DXFROMWINEVECTOR3 aiVecs[2];D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);
	    computeTangentSpace(&vertices[0], sizeof(vertices[0]), vertexCount);
            CreateIndexBuffer(vertexCount, indexCount, slices);

	stx_Mesh mesh;
	meshes.push_back(mesh);
	meshes[0].vertices=vertices;//vertices.clear();
	meshes[0].indices=indices;
	meshes[0].rindices.push_back(indices);meshes[0].indices.clear();//indices.clear();

	if(aShader_!=eShaderNone)
		InitShader(aShader_);
	texID=-1;
}

//-------------------------------------------------------------------------------
// Calculate the boundaries of a given node and all of its children
// The boundaries are in Worldspace (AABB)
// piNode Input node
// p_avOut Receives the min/max boundaries. Must point to 2 D3DXFROMWINEVECTOR3s
// piMatrix Transformation matrix of the graph at this position
//-------------------------------------------------------------------------------
D3DXFROMWINEMATRIX MeshRenderer2::CalculateBounds(D3DXFROMWINEVECTOR3* p_avOut, D3DXFROMWINEMATRIX* piMatrix)
{
	p_avOut[0] = D3DXFROMWINEVECTOR3( 1e10f, 1e10f, 1e10f);
	p_avOut[1] = D3DXFROMWINEVECTOR3( -1e10f, -1e10f, -1e10f);
	for( unsigned int a = 0; a < vertices.size();++a)
	{
		D3DXFROMWINEVECTOR3 pc1 = vertices[a].Position;
		p_avOut[0].x = std::min( p_avOut[0].x, pc1.x);
		p_avOut[0].y = std::min( p_avOut[0].y, pc1.y);
		p_avOut[0].z = std::min( p_avOut[0].z, pc1.z);
		p_avOut[1].x = std::max( p_avOut[1].x, pc1.x);
		p_avOut[1].y = std::max( p_avOut[1].y, pc1.y);
		p_avOut[1].z = std::max( p_avOut[1].z, pc1.z);
	}

	D3DXFROMWINEVECTOR3 vDelta = p_avOut[1]-p_avOut[0];
	D3DXFROMWINEVECTOR3 vHalf =  p_avOut[0] + (vDelta / 2.0f);
	float fScale = 2.0f / D3DXFROMWINEVec3Length(&vDelta);

	D3DXFROMWINEMATRIX m_mWorld =  D3DXFROMWINEMATRIX(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-vHalf.x,-vHalf.y,-vHalf.z,1.0f) *
		D3DXFROMWINEMATRIX(
		fScale,0.0f,0.0f,0.0f,
		0.0f,fScale,0.0f,0.0f,
		0.0f,0.0f,fScale,0.0f,
		0.0f,0.0f,0.0f,1.0f);

	if(0)
	for( unsigned int j = 0; j < vertices.size();++j)
	{
		D3DXFROMWINEVec3TransformCoord(&vertices[j].Position, &vertices[j].Position, &m_mWorld);
		if(piMatrix)
			D3DXFROMWINEVec3TransformCoord(&vertices[j].Position, &vertices[j].Position, piMatrix);
	}
	if(piMatrix)
		m_mWorld*=(*piMatrix);
	return m_mWorld;
}
#if 0
void MeshRenderer2::GetBounds(D3DXFROMWINEVECTOR3& ____min, D3DXFROMWINEVECTOR3& ____max)
{
	D3DXFROMWINEVECTOR3 aiVecs[2] = {D3DXFROMWINEVECTOR3( 1e10f, 1e10f, 1e10f),
		D3DXFROMWINEVECTOR3( -1e10f, -1e10f, -1e10f) };

	D3DXFROMWINEMATRIX m;D3DXFROMWINEMatrixIdentity(&m);CalculateBounds(&aiVecs[0], &m);

	____min = *(D3DXFROMWINEVECTOR3*)&aiVecs[0];
	____max = *(D3DXFROMWINEVECTOR3*)&aiVecs[1];
}
#endif
    	D3DXFROMWINEVECTOR3 MeshRenderer2::GetMeshBBoxExtents()
	{
		D3DXFROMWINEVECTOR3 __min;
		D3DXFROMWINEVECTOR3 __max;
		GetBounds(__min, __max);
		return __max;
	}
    	D3DXFROMWINEVECTOR3 MeshRenderer2::GetMeshBBoxCenter()
	{
		D3DXFROMWINEVECTOR3 __min;
		D3DXFROMWINEVECTOR3 __max;
		GetBounds(__min, __max);
		return __max/2.0f;// is this correct ???
	}
#if 0
void MeshRenderer2::CreateMesh(const char* afn, tShader aShader, bool scaleasset, bool a_bMeshRenderer2Simpler_ProceduralBuilding)
{
		Clear();
	m_Shape=eMesh;
	vertices.clear();
	indices.clear();
	MeshRenderer2 mesh_;
	mesh_.Load(afn, aShader, false);
	//std::vector<stx_Mesh> meshes;
	if(mesh_.meshes.size())
	{
		vertices=mesh_.meshes[0].vertices;
		indices=mesh_.meshes[0].indices;
	}
}
#endif

/*
# The L-system for the 3D-modelling of an house.

# scene
# highway
# road
# path
# yard
# house
# wall
# door
# window
# ceiling/roof
# chimney
# rain gutter

# VertexVector
# IndexVector
# TRIANGLESTRIP
# TRIANGLE
*/
/*
http://paulbourke.net/geometry/platonic/:
The measured properties of the 3 dimensional regular polyhedra
Tetrahedron

	Vertices: 4
Edges: 6
Faces: 4
Edges per face: 3
Edges per vertex: 3
Sin of angle at edge: 2 * sqrt(2) / 3
Surface area: sqrt(3) * edgelength^2
Volume: sqrt(2) / 12 * edgelength^3
Circumscribed radius: sqrt(6) / 4 * edgelength
Inscribed radius: sqrt(6) / 12 * edgelength

Coordinates

 1  1  1   -1  1 -1    1 -1 -1
-1  1 -1   -1 -1  1    1 -1 -1
 1  1  1    1 -1 -1   -1 -1  1
 1  1  1   -1 -1  1   -1  1 -1

Divide each coordinate by 2
Octahedron

	Vertices: 6
Edges: 12
Faces: 8
Edges per face:3
Edges per vertex: 4
Sin of angle at edge: 2 * sqrt(2) / 3
Surface area: 2 * sqrt(3) * edgelength^2
Volume: sqrt(2) / 3 * edgelength^3
Circumscribed radius: sqrt(2) / 2 * edgelength
Inscribed radius: sqrt(6) / 6 * edgelength

Coordinates

-a  0  a   -a  0 -a    0  b  0
-a  0 -a    a  0 -a    0  b  0
 a  0 -a    a  0  a    0  b  0
 a  0  a   -a  0  a    0  b  0
 a  0 -a   -a  0 -a    0 -b  0
-a  0 -a   -a  0  a    0 -b  0
 a  0  a    a  0 -a    0 -b  0
-a  0  a    a  0  a    0 -b  0

Where a = 1 / (2 * sqrt(2)) and b = 1 / 2
Hexahedron (cube)

	Vertices: 8
Edges: 12
Faces: 6
Edges per face: 4
Edges per vertex: 3
Sin of angle at edge: 1
Surface area: 6 * edgelength^2
Volume: edgelength^3
Circumscribed radius: sqrt(3) / 2 * edgelength
Inscribed radius: 1 / 2 * edgelength

Coordinates

-1 -1 -1    1 -1 -1    1 -1  1   -1 -1  1
-1 -1 -1   -1 -1  1   -1  1  1   -1  1 -1
-1 -1  1    1 -1  1    1  1  1   -1  1  1
-1  1 -1   -1  1  1    1  1  1    1  1 -1
 1 -1 -1    1  1 -1    1  1  1    1 -1  1
-1 -1 -1   -1  1 -1    1  1 -1    1 -1 -1

Divide each vertex by 2
Icosahedron

	Vertices: 12
Edges: 30
Faces: 20
Edges per face: 3
Edges per vertex: 5
Sin of angle at edge: 2 / 3
Surface area: 5 * sqrt(3) * edgelength^2
Volume: 5 * (3 + sqrt(5)) / 12 * edgelength^3
Circumscribed radius: sqrt(10 + 2 * sqrt(5)) / 4 * edgelength
Inscribed radius: sqrt(42 + 18 * sqrt(5)) / 12 * edgelength

Coordinates

 0  b -a    b  a  0   -b  a  0
 0  b  a   -b  a  0    b  a  0
 0  b  a    0 -b  a   -a  0  b
 0  b  a    a  0  b    0 -b  a
 0  b -a    0 -b -a    a  0 -b
 0  b -a   -a  0 -b    0 -b -a
 0 -b  a    b -a  0   -b -a  0
 0 -b -a   -b -a  0    b -a  0
-b  a  0   -a  0  b   -a  0 -b
-b -a  0   -a  0 -b   -a  0  b
 b  a  0    a  0 -b    a  0  b
 b -a  0    a  0  b    a  0 -b
 0  b  a   -a  0  b   -b  a  0
 0  b  a    b  a  0    a  0  b
 0  b -a   -b  a  0   -a  0 -b
 0  b -a    a  0 -b    b  a  0
 0 -b -a   -a  0 -b   -b -a  0
 0 -b -a    b -a  0    a  0 -b
 0 -b  a   -b -a  0   -a  0  b
 0 -b  a    a  0  b    b -a  0

Where a = 1 / 2 and b = 1 / (2 * phi)
phi is the golden ratio = (1 + sqrt(5)) / 2

Contribution by Craig Reynolds: vertices and faces for the icosahedron. Along with C++ code to create a sphere based upon the icosahedron: sphere.cpp, see also surface refinement for related ideas.
Dodecahedron

	Vertices: 20
Edges: 30
Faces: 12
Edges per face: 5
Edges per vertex: 3
Sin of angle at edge: 2 / sqrt(5)
Surface area: 3 * sqrt(25 + 10 * sqrt(5)) * edgelength^2
Volume: (15 + 7 * sqrt(5)) / 4 * edgelength^3
Circumscribed radius: (sqrt(15) + sqrt(3)) / 4 * edgelength
Inscribed radius: sqrt(250 + 110 * sqrt(5)) / 20 * edgelength

Coordinates

 c  0  1   -c  0  1   -b  b  b    0  1  c    b  b  b
-c  0  1    c  0  1    b -b  b    0 -1  c   -b -b  b
 c  0 -1   -c  0 -1   -b -b -b    0 -1 -c    b -b -b
-c  0 -1    c  0 -1    b  b -b    0  1 -c   -b  b -b
 0  1 -c    0  1  c    b  b  b    1  c  0    b  b -b
 0  1  c    0  1 -c   -b  b -b   -1  c  0   -b  b  b
 0 -1 -c    0 -1  c   -b -b  b   -1 -c  0   -b -b -b
 0 -1  c    0 -1 -c    b -b -b    1 -c  0    b -b  b
 1  c  0    1 -c  0    b -b  b    c  0  1    b  b  b
 1 -c  0    1  c  0    b  b -b    c  0 -1    b -b -b
-1  c  0   -1 -c  0   -b -b -b   -c  0 -1   -b  b -b
-1 -c  0   -1  c  0   -b  b  b   -c  0  1   -b -b  b

Where b = 1 / phi and c = 2 - phi
Divide each coordinate by 2.
*/
#if 1
void stx_SkyBox::OnCreateDevice(const unsigned int aN, const TextureID& aEnv, const char* aEffectFileName )
{
#if 1
	env = aEnv;
	m_pEffect=IRenderer::GetRendererInstance()->addEffectFromFile(aEffectFileName);
	float size=1000.0f;
	m_pMesh=new MeshRenderer2();
	m_pMesh->CreateTexturedBox(size, size, size, eShaderNone);
#endif
}


void stx_SkyBox::render(const D3DXFROMWINEMATRIX* aMvp)
{
#if 1
if(m_pEffect)
{
    unsigned int cPasses=0;
    unsigned int Value=0;
	unsigned int iPass=0;
	m_pEffect->Begin(&cPasses, Value);
	m_pEffect->BeginPass(iPass);

	if(m_pMesh)
		if(aMvp)
			m_pMesh->render(aMvp);
		else
			m_pMesh->render(&mvp);
	else
		render_(); // ???

	m_pEffect->EndPass();
	m_pEffect->End();
}
else
{
	IRenderer::GetRendererInstance()->setShader(skyBox);
	IRenderer::GetRendererInstance()->setVertexFormat(skyboxVF);

#if defined(CUBEMAPS)
	IRenderer::GetRendererInstance()->setTexture("Env", env, IRenderer::GetRendererInstance()->Gettrilinear());
#else
	IRenderer::GetRendererInstance()->setTexture("Env", env, IRenderer::GetRendererInstance()->GetbilinearClamp());
#endif
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthWrite());
	#if 0
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("mvp", I);
	#else
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("mvp", mvp);
	#endif
	IRenderer::GetRendererInstance()->setShaderConstant4f("fogColor", fogColor);
	IRenderer::GetRendererInstance()->setShaderConstant2f("scaleBias", D3DXFROMWINEVECTOR2(2 + 0.3f * camPos.y, camPos.y * 1.3f - 1.2f));
	render_();
}
#endif
}
#endif

