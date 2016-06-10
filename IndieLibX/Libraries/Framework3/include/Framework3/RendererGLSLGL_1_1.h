#if !defined(USE_GLES_SHADERS) && !defined(HAS_CGGL) && !defined(_MSC_VER)
#ifndef __FrameworkGLSLGL_1_1_H__
#define __FrameworkGLSLGL_1_1_H__ 1
/* * * * * * * * * * * * * Author's note * * * * * * * * * * * *\
*
*  |_|	   |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|	*
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _	*
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_	*
*  |_|	   |_| |_|_ _ _|_| |_|	   |_| |_|_ _ _|_|  _ _ _ _|_|	*
*  |_|	   |_|	 |_|_|_|   |_|	   |_|	 |_|_|_|   |_|_|_|_|	*
*								*
*		      http://www.humus.name			*
*								 *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.		   *
*								 *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/RendererGL_1_1.h>
//typedef void* ShHandle;
inline int constantCompShaderGLSLGL3(const void *s0, const void *s1){
        return strcmp(((ConstantShaderGLSLGL3 *) s0)->name.c_str(), ((ConstantShaderGLSLGL3 *) s1)->name.c_str());
}

inline int samplerCompShaderGLSLGL3(const void *s0, const void *s1){
        return strcmp(((SamplerShaderGLSLGL3 *) s0)->name.c_str(), ((SamplerShaderGLSLGL3 *) s1)->name.c_str());
}

struct RendererGLSLGL_1_1 : public RendererGL_1_1
{
	protected:
		bool mOk;
	public:		
		virtual unsigned int GetShaderCount(){return shaders.size();}
		virtual const char* getString(){return "GLSLGL_1_1";}
		virtual bool IsOK(){return mOk;}
	int getSamplerUnit(const ShaderID shader, const char *samplerName) const;
	void applyTextures();
void applyConstants();
void setTexture(const char *textureName, const TextureID texture);
void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState
		, const int vtstream, const VertexBufferID vtbuffer);
void DrawPrimitive(Primitives PrimitiveType,UINT StartVertex,UINT PrimitiveCount, const uint flags);
void DrawIndexedPrimitive(Primitives PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount, const uint flags);
void changeShader(const ShaderID shader);
void setShaderConstantRaw(const char *name, const void *data, const int size);
ShaderID addGLSLShader(const char *vsText0, const char *gsText, const char *fsText0,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags);
virtual ShaderID addHLSLShaderVrtl(const char *vsText0, const char *gsText, const char *fsText0,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags);



bool TranslateVertexShader(const std::string& hlsl, const std::string& main, std::string& glsl);
bool TranslateFragmentShader(const std::string& hlsl, const std::string& main, std::string& glsl);
VArray <ShaderGLSLGL3> shaders;

	void changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBuffer, const intptr offset);
        RendererGLSLGL_1_1(int awidth=800, int aheight=600,
		bool InitLogFile=false,
			bool fullScreen=false,
		int argc=0, char *argv[]=0);

	virtual ~RendererGLSLGL_1_1();
#if 0
	void	CreateTextureFromFile_Serial(  CHAR* szFileName,
						TextureID* ppTexture,
						//, void* pContext
						CPackedFile*		    g_PackFile,
						UINT g_SkipMips
						);
void	CreateVertexBuffer_Serial(  VertexBufferID& ppBuffer, UINT iSizeBytes,
//					    DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
//					    void* pContext
						);
void	CreateIndexBuffer_Serial(  IndexBufferID& ppBuffer, UINT iSizeBytes,
//					    DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
//					    void* pContext
						);

	BYTE *fillVertexBuffer(const VertexBufferID id, BYTE*  m_pData, int l){return 0;}
	BYTE *fillIndexBuffer(const IndexBufferID id, BYTE*  m_pData, int l){return 0;}

void setGLdefaults();
		////FontID defaultFont;
	////FontID GetDefaultFont(){return defaultFont;}
	////virtual VArray <TexFont*>& GetFonts(){return fonts;}
	////virtual TexFont* GetFont(unsigned int i){return fonts.GetAt(i);}

	float4 scaleBias2D;
	float4& GetscaleBias2D(){return scaleBias2D;}

	virtual int GetTextureWidth(TextureID id);
	virtual int GetTextureHeight(TextureID id);

	virtual void viewport(int x, int y, int viewportWidth, int viewportHeight);

int texturePitchVrtl(const TextureID id,int level);
BYTE * lockTexture(const TextureID id,int level, int& pitch);
BYTE * lockVertexBuffer(const VertexBufferID id);
BYTE * lockIndexBuffer(const IndexBufferID id);
BYTE * unlockTextureVrtl(const TextureID id,int level);
BYTE * unlockVertexBuffer(const VertexBufferID id);
BYTE * unlockIndexBuffer(const IndexBufferID id);

		int deleteVertexBuffer(const VertexBufferID vertexBuffer);
		int deleteIndexBuffer(const IndexBufferID indexBuffer);

    ProcessorID addTextureProcessor( TextureID& ppTexture, int g_SkipMips );
    HRESULT ProcessTexture( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes );
    HRESULT LockTextureDeviceObject(ProcessorID pProcessor);
    HRESULT CopyToResourceTexture(ProcessorID pProcessor);
    HRESULT UnLockTextureDeviceObject(ProcessorID pProcessor);

    ProcessorID addVertexBufferProcessor( VertexBufferID& ppBuffer, int iSizeBytes,   void* pData);
    HRESULT ProcessVertexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes );
    HRESULT LockVertexBufferDeviceObject(ProcessorID pProcessor);
    HRESULT CopyToResourceVertexBuffer(ProcessorID pProcessor);
    HRESULT UnLockVertexBufferDeviceObject(ProcessorID pProcessor);

    ProcessorID addIndexBufferProcessor( IndexBufferID& ppBuffer, int iSizeBytes, void* pData);
    HRESULT ProcessIndexBuffer(ProcessorID pProcessor, void*pLocalData, SIZE_T Bytes );
    HRESULT LockIndexBufferDeviceObject(ProcessorID pProcessor);
    HRESULT CopyToResourceIndexBuffer(ProcessorID pProcessor);
    HRESULT UnLockIndexBufferDeviceObject(ProcessorID pProcessor);



	////VArray <TexFont*> fonts;

	VArray <TextureGL2> textures;
	VArray <VertexBufferGL2> vertexBuffers;
	VArray <IndexBufferGL2> indexBuffers;
protected:
	VArray <VertexFormatGL2> vertexFormats;
	VArray <SamplerStateGL2> samplerStates;
	VArray <BlendStateGL2> blendStates;
	VArray <AlphaStateGL2> alphaStates;
	VArray <DepthStateGL2> depthStates;
	VArray <RasterizerStateGL2> rasterizerStates;

	VArray <CTextureProcessorGL2> textureProcessors;
	VArray <CVertexBufferProcessorGL2> vertexBufferProcessors;
	VArray <CIndexBufferProcessorGL2> indexBufferProcessors;


public:
virtual void buildPerspectiveMatrix(float fieldOfView,
                                   float aspectRatio,
                                   float zNear, float zFar)//,mat4& m)
{
buildPerspectiveMatrixGL(fieldOfView,
                                   aspectRatio,
                                   zNear, zFar,p);
	m_fieldOfView=fieldOfView;
    m_aspectRatio=aspectRatio;
    m_zNear=zNear;
    m_zFar=zFar;
//	p=m;

}
        virtual void buildLookAtMatrix(float eyex, float eyey, float eyez,
                              float centerx, float centery, float centerz,
                              float upx, float upy, float upz)//,mat4& m)
        {
#if 1
buildLookAtMatrixGL(eyex, eyey, eyez,
                              centerx, centery, centerz,
                              upx, upy, upz,mv);
#else
	float3 c1=float3(centerx-eyex,centery-eyey,centerz-eyez);
	mv=identity4();
if(_isinf(c1.x)) return;
if(_isinf(c1.y)) return;
if(_isinf(c1.z)) return;
	float radius=0.0f; float phi=0.0f; float theta=0.0f;
	float f=c1.x*c1.x+c1.y*c1.y+c1.z*c1.z;
	if(f<=0.0000001f) return;
	radius=sqrt(f);
	phi = asinf(c1.z / (radius));
	if(0.0<=c1.y)
		theta = acosf(c1.x / ((radius)* cosf((phi))));
	else if(c1.y<0.0f)
		theta = -acosf(c1.x /((radius)*cosf((phi))));
	mv=
	rotateXY(phi,theta)
	*translate(-float3(eyex, eyey, eyez));
#endif
	m_eye=float3(eyex, eyey, eyez);
	m_center=float3(centerx, centery, centerz);
    m_up=float3(upx, upy, upz);
//mv=m;
        }

	        virtual bool supportsR2VB();
        virtual bool supportsHalf();
        virtual bool supportsPS3();
        virtual bool supportsATI1N();

        void setTexture(const char *textureName, const TextureID texture);
        void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState		, const int vtstream
		, const VertexBufferID vtbuffer
);
        void setTextureSlice(const char *textureName, const TextureID texture, const int slice);

        TextureID addTextureVrtl(Image33& img, const bool useMips, const SamplerStateID samplerState, uint flags, float3 alpha);

        TextureID addRenderTarget(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags);
        TextureID addRenderDepth(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags);
        void setRenderTargetSize(const TextureID renderTarget, const int width, const int height);
        bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize);
        bool generateMipMaps(const TextureID renderTarget);

	int removeTexture(const TextureID texture);
	UINT32 GetAvailableTextureMem();
#if 0
        char vendor[256];
        float textureLod[MAX_TEXTUREUNIT];
        VertexFormatID activeVertexFormat[MAX_VERTEXSTREAM];
        GLuint fbo;
        GLenum drawBuffers[MAX_MRTS];
        GLenum currentFrontFace;
        GLenum currentSrcFactor;
        GLenum currentDstFactor;
        GLenum currentBlendMode;
        int currentMask;
        bool currentBlendEnable;
        int currentDepthFunc;
        bool currentDepthTestEnable;
        bool currentDepthWriteEnable;
        bool currentMultiSampleEnable;
        bool currentScissorEnable;
        int currentCullMode;
        int currentFillMode;
        GLuint currentVBO;
#endif
		virtual void resetVrtl(const uint flags);
		virtual void resetToDefaults();
        void SaveImg(Image3& img,const char* fn);

        virtual void changeRenderTargets(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int depthSlice, const int *slices);
        virtual void changeToMainFramebuffer();

        void applyTextures();

        void setSamplerState(const char *samplerName, const SamplerStateID samplerState);
        void applySamplerStates();
        int getSamplerUnit(const ShaderID shader, const char *samplerName) const;

        ShaderID addHLSLShader(const char *vsText, const char *gsText, const char *fsText,
			const char *vsName, const char *psName, const int vsLine, const int gsLine, const int fsLine,
                const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags);
        VertexFormatID addVertexFormatVrtl(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader);
        VertexBufferID addVertexBufferVrtl(const long size, const BufferAccess bufferAccess, const void *data, VertexBufferID old, const IDataProcessor* p);
        IndexBufferID addIndexBufferVrtl(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID old, const IDataProcessor* p);


        SamplerStateID addSamplerStateVrtl(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod, const uint maxAniso, const int compareFunc, const float *border_color);
        BlendStateID addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask, const bool alphaToCoverage);
        DepthStateID addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc, const bool stencilTest,
                const uint8 stencilMask, const int stencilFunc, const int stencilFail, const int depthFail, const int stencilPass);
        RasterizerStateID addRasterizerStateVrtl(const int cullMode, const int fillMode, const bool multiSample, const bool scissor);

        void changeShader(const ShaderID shader);
        void changeVertexFormat(const VertexFormatID vertexFormat);
        void changeVertexBuffer(const int stream, const VertexBufferID vertexBuffer, const intptr offset);
        void changeIndexBuffer(const IndexBufferID indexBuffer);

//      void changeSamplerState(const uint samplerUnit, const SamplerStateID samplerState);
        void changeBlendState(const BlendStateID blendState, const uint sampleMask);
        void changeDepthState(const DepthStateID depthState, const uint stencilRef);
        void changeRasterizerState(const RasterizerStateID rasterizerState);

        void setShaderConstantRaw(const char *name, const void *data, const int size);
        void applyConstants();

    AlphaStateID addAlphaState(
		const int func,
		const float ref
		);
	void changeAlphaState(const AlphaStateID alphaState);

    virtual void SaveScreenShotVrtl(const char* fn);
	virtual void Init0(void);

void grabScreenshot(Image3& img,int w,int h);
	virtual void setup2DMode(const float left, const float right, const float top, const float bottom);

        virtual void DrawPrimitive(Primitives PrimitiveType,
                UINT StartVertex,
                UINT PrimitiveCount, const uint flags);
    virtual void DrawIndexedPrimitive(Primitives PrimitiveType,
                INT BaseVertexIndex,
                UINT MinVertexIndex,
                UINT NumVertices,
                UINT startIndex,
                UINT primCount, const uint flags);
    virtual void BeginSceneVrtl(bool aB){};
    virtual void EndSceneVrtl(bool aB){};
	virtual void Clear(bool color, bool depth, float4 colors);
    virtual void PresentVrtl(bool aB);


protected:
        void changeFrontFace(const GLenum frontFace);
//      void setupFilter(const Texture &tex, const Filter filter, const uint flags);
        void setupSampler(GLenum glTarget, const SamplerStateGL2 &ss);
public:



        TextureID currentTextures[MAX_TEXTUREUNIT], selectedTextures[MAX_TEXTUREUNIT];
        SamplerStateID currentSamplerStates[MAX_SAMPLERSTATE], selectedSamplerStates[MAX_SAMPLERSTATE];
        float textureLod[MAX_TEXTUREUNIT];

        VertexFormatID activeVertexFormat[MAX_VERTEXSTREAM];

        GLuint fbo;
        GLenum drawBuffers[MAX_MRTS];
        GLenum currentFrontFace;

        GLenum currentSrcFactorRGB;
        GLenum currentDstFactorRGB;
        GLenum currentSrcFactorAlpha;
        GLenum currentDstFactorAlpha;
        GLenum currentBlendModeRGB;
        GLenum currentBlendModeAlpha;
        int currentMask;
        bool currentBlendEnable;
        bool currentAlphaEnable;
        bool currentAlphaToCoverageEnable;

        int currentDepthFunc;
        bool currentDepthTestEnable;
        bool currentDepthWriteEnable;

        bool currentMultiSampleEnable;
        bool currentScissorEnable;
        int currentCullMode;
        int currentFillMode;

        GLuint currentVBO;
#endif





        void *uniformFuncs[CONSTANT_TYPE_COUNT];
#if 0
    ShHandle parserv;// = Hlsl2Glsl_ConstructParser(EShLangVertex, 0);
    ShHandle parserf;// = Hlsl2Glsl_ConstructParser(EShLangFragment, 0);

    // Construct a Translator to use for final code translation
    ShHandle translator;// = Hlsl2Glsl_ConstructTranslator(debugOptions);
#endif
};
#endif
#endif
