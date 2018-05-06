/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#include <vector>
#include <Scene/3D/ICameraSceneNode.hpp>
#include <Scene/3D/CSpotLightSceneNode.hpp>
#include <Scene/3D/IMeshSceneNode.hpp>
#include <Core/Core.hpp>
#include <Core/CMediaManager.hpp>
#include <Core/Logger/CLoggerFile.hpp>
#include <Core/Utility/CMatrix.hpp>
#include <Medias/CMaterial.hpp>
#include <Medias/CStaticMesh.hpp>
#include <Medias/CFont.hpp>
#include "IDeferredApplication.hpp"
#include "Loaders/CCinematicEventLoader.hpp"
#include "Event/ICinematicEvent.hpp"

namespace daidalosgameengine {
	/**
	* Test structure
	*/
	struct STest {
		std::string aaa;
		int bbb;
	};

	/**
	* Screen quad used for deferred shading
	*/
	struct SScreenQuadVertexLight {
		daidalosengine::CVector3F vertex;
		daidalosengine::CVector2F texcoord;
		daidalosengine::CVector3F viewDistance;
	};

	struct SLightVertexGeometry {
		daidalosengine::CVector3F vertex;
	};

	IDeferredApplication::IDeferredApplication(const HINSTANCE hInstance, const int nCmdShow) : IWin32Application(hInstance, nCmdShow, "Daidalos Engine") {

		// Cinematic loader
		daidalosengine::CMediaManager::GetInstance()->AddLoader(new CCinematicEventLoader(), "dgc");
		daidalosengine::CMediaManager::GetInstance()->AddSearchPath("Resources/Cinematics/");

		this->m_loading = false;
		this->m_loadingScene = new daidalosengine::C2DScene();

		this->m_cursor.SetTexture("cursor.png");
		this->m_cursor.SetVisible(false);

		daidalosengine::STexture2DFilterPolicy textureFilter2;
		textureFilter2.minFilter = daidalosengine::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.magFilter = daidalosengine::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.mipFilter = daidalosengine::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.addressU = daidalosengine::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.addressV = daidalosengine::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.maxAnisotropy = 0;

		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		// --------------------------------------------------------------------------------------------------
		// 2D Rendering 
		// --------------------------------------------------------------------------------------------------
		// Vertex declaration used by every 2d elements
		daidalosengine::SDeclarationElement decl2D[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declaration2D = renderer->CreateVertexDeclaration(decl2D);

		// Shader program used for 2d rendering
		this->m_shader2D.LoadFromFile("2d");


		// --------------------------------------------------------------------------------------------------
		// Deferred Shading
		// --------------------------------------------------------------------------------------------------

		// Main Frame buffer
		daidalosengine::SFrameBufferDeclaration fbd[] =
		{
			{ "diffuseRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "normalRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "specularRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "postProcessRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "depthRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_R32F, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "lightRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE },
			{ "lightDepthRT", daidalosengine::CVector2I(GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_R32F, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		daidalosengine::SFrameBufferDepthDeclaration fbdd[] =
		{
			{ "depth", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_DEPTH24S8 },
			{ "lightDepth", daidalosengine::CVector2I(GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_DSFB.CreateFromDeclaration(fbd, fbdd);
		this->m_DSFB.AttachDepth("depth");

		// Light geometry
		daidalosengine::SDeclarationElement lightGeometryDecl[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_lightGeometryVD = renderer->CreateVertexDeclaration(lightGeometryDecl);

		// Light screen quad
		daidalosengine::SDeclarationElement screenQuadDecl1[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD1, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_screenQuadLightVD = renderer->CreateVertexDeclaration(screenQuadDecl1);
		daidalosengine::SDeclarationElement screenQuadDecl2[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_screenQuadLightVD2 = renderer->CreateVertexDeclaration(screenQuadDecl2);
		this->m_screenQuadLightVB = renderer->CreateVertexBuffer(sizeof(SScreenQuadVertexLight), 4, daidalosengine::BUFFER_DYNAMIC | daidalosengine::BUFFER_WRITEONLY, nullptr);

		// Final screen quad
		daidalosengine::SDeclarationElement screenQuadDecl3[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_screenQuadFinalVD = renderer->CreateVertexDeclaration(screenQuadDecl3);
		this->m_screenQuadFinalVB = renderer->CreateVertexBuffer(sizeof(daidalosengine::SScreenQuadElement), 4, daidalosengine::BUFFER_DYNAMIC | daidalosengine::BUFFER_WRITEONLY, renderer->GetScreenQuad());

		// Diffuse
		daidalosengine::SDeclarationElement declarationDiffuse[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declarationDiffuse = renderer->CreateVertexDeclaration(declarationDiffuse);

		// Shaders
		this->m_DSFill.LoadFromFile("DS_fill_normal_mapping");
		this->m_DSShadowMap.LoadFromFile("DS_shadowMap");
		this->m_DSSpotLight.LoadFromFile("DS_spotLight");
		this->m_DSPointLight.LoadFromFile("DS_pointLight");
		this->m_DSDirectionalLight.LoadFromFile("DS_directionalLight");
		this->m_DSFinal.LoadFromFile("DS_final");
		this->m_DSPostProcessFinal.LoadFromFile("DS_postProcessFinal");
		this->m_shaderTransparent.LoadFromFile("ForwardTransparent");
		this->m_shaderDiffuse.LoadFromFile("ForwardDiffuse");

		// --------------------------------------------------------------------------------------------------
		// Deferred Shading
		// --------------------------------------------------------------------------------------------------
		daidalosengine::SDeclarationElement decl[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL1, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL2, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declaration = renderer->CreateVertexDeclaration(decl);

		// Post process
		this->m_texture2DPostProcess.CreateEmpty("postProcessTexture2D", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, &textureFilter2, daidalosengine::TEXTURE_FLAG_FRAMEBUFFER_TYPE);
		this->m_shaderScreenBlur.LoadFromFile("ScreenBlur");
		this->m_shaderScreenGlow.LoadFromFile("ScreenGlow");

		// Misc
		this->m_texture2DBlack.CreateFromFile("black.png", daidalosengine::PIXEL_FORMAT_A8R8G8B8, &textureFilter2);
		this->m_texture2DWhite.CreateFromFile("white.png", daidalosengine::PIXEL_FORMAT_A8R8G8B8, &textureFilter2);
		this->m_shaderScreen.LoadFromFile("Screen");

		// Create the fps label
		this->m_fpsLabel.SetBounds(daidalosengine::CRectangleI(10, 10, 200, 30));
		this->m_fpsLabel.SetSize(25);
		this->m_fpsLabel.SetColor(daidalosengine::CColor(255, 255, 102, 255));
		this->m_fpsLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_fpsLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_fpsLabel.SetText("FPS");

		// ------------------------------------------------------------------------------------------------
		// The loading scene
		// --------------------------------------------------------------------------------------------------
		this->m_loadingView = new CLoadingView(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));
		this->m_loadingScene->AddToMainView(this->m_loadingView);
	}

	IDeferredApplication::~IDeferredApplication() {
		// Remove the cinematic loader
		daidalosengine::CMediaManager::GetInstance()->RemoveLoader<ICinematicEvent>();

		delete this->m_loadingScene;
	}

	void IDeferredApplication::Update(const float time) {
		this->m_fpsCounter.Update(time);

		if (GetOptions().GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_SHOW_FPS, false)) {
			this->m_fpsLabel.SetText(this->m_fpsCounter.GetText());
			this->m_fpsLabel.Update(time);
		}
	}

	void IDeferredApplication::Render() {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		renderer->BeginScene();

		if (!this->m_loading) {
			Render3DScene(this->m_currentLevel->GetScene());
			Render2DScene(this->m_currentLevel->GetView());

			if (this->m_cursor.IsVisible()) {
				this->m_cursor.Render();
			}

		} else {
			renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
			Render2DScene(this->m_loadingScene);
		}
		this->m_fpsLabel.Render();

		// ------------------------------
		// Cleanup
		// ------------------------------
		renderer->SetFrameBuffer(nullptr);
		renderer->SetVertexDeclaration(nullptr);
		renderer->SetShaderProgram(nullptr);
		renderer->SetTexture(0, nullptr);
		renderer->SetTexture(1, nullptr);
		renderer->SetTexture(2, nullptr);
		renderer->SetTexture(3, nullptr);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);

		renderer->EndScene();
	}

	/**
	* Render a 3D scene
	*/
	void IDeferredApplication::Render3DScene(daidalosengine::C3DScene * scene3D) {

		// ------------------------------
		// Initialization
		// ------------------------------
		InitializeRendering(scene3D);

		// ------------------------------
		// Diffuse
		// ------------------------------
		DiffuseRendering(scene3D);

		// ------------------------------
		// Prepare the light rendering
		// ------------------------------
		PreLightRendering(scene3D);

		// ------------------------------
		// Forward rendering: spot lights
		// ------------------------------
		SpotLightsRendering(scene3D);

		// ------------------------------
		// Forward rendering: point lights
		// ------------------------------
		PointLightsRendering(scene3D);

		// ------------------------------
		// Forward rendering: directional lights
		// ------------------------------
		DirectionalLightsRendering(scene3D);

		// ------------------------------
		// Render the RT in the backbuffer
		// ------------------------------
		PostLightRendering(scene3D);

		// ------------------------------
		// Post processing
		// ------------------------------
		PostProcessRendering();

		// ------------------------------
		// Sky box mask + rendering
		// ------------------------------
		SkyboxRendering(scene3D);

		// ------------------------------
		// Transparent
		// ------------------------------
		TransparentRendering(scene3D);

		// ------------------------------
		// Particle System
		// ------------------------------
		ParticleSystemRendering(scene3D);

		// ------------------------------
		// Finalize
		// ------------------------------
		FinalizeDeferredRendering(scene3D);

		// ------------------------------
		// Diffuse - Top layer
		// ------------------------------
		DiffuseTopLayerRendering(scene3D);

		// ------------------------------
		// Brightness factor
		// ------------------------------
		//ApplyBrightness();

		// ------------------------------
		// Debug
		// ------------------------------
		if (GetOptions().GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_DEBUG, false)) {
			scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
			scene3D->RenderDebugInformation();
		}
	}

	/**
	* Render a 2D scene
	*/
	void IDeferredApplication::Render2DScene(daidalosengine::C2DScene * scene2D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// 2D elements: Prepare the renderer and draw
		// ------------------------------
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetAlphaReference(0x00000001);
		renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		renderer->SetVertexDeclaration(this->m_declaration2D.get());
		renderer->SetShaderProgram(this->m_shader2D.GetShaderProgram());
		this->m_shader2D.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_PROJ_MATRIX), scene2D->GetProjMatrix());
		scene2D->Render();
	}

	/**
	* Do initialization before rendering anything
	* @param scene3D The scene
	*/
	void IDeferredApplication::InitializeRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		renderer->SetFrameBuffer(nullptr);
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
	}

	/**
	* Render the skybox
	* @param scene3D The scene
	*/
	void IDeferredApplication::SkyboxRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Sky box mask
		// ------------------------------
		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, true);
		renderer->SetStencilFunction(daidalosengine::COMPARE_ALWAYS);
		renderer->SetStencilZFail(daidalosengine::STENCIL_OPERATION_KEEP);
		renderer->SetStencilFail(daidalosengine::STENCIL_OPERATION_KEEP);
		renderer->SetStencilPass(daidalosengine::STENCIL_OPERATION_INCREMENT_CLAMP);
		renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		scene3D->RenderSkyboxMask();

		// ------------------------------
		// Sky box
		// ------------------------------
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		renderer->SetStencilFunction(daidalosengine::COMPARE_LESS);
		renderer->SetStencilZFail(daidalosengine::STENCIL_OPERATION_KEEP);
		renderer->SetStencilFail(daidalosengine::STENCIL_OPERATION_KEEP);
		renderer->SetStencilPass(daidalosengine::STENCIL_OPERATION_KEEP);
		renderer->SetStencilRef(0);
		scene3D->RenderSkybox();
	}

	/**
	* Render the diffuse part
	* @param scene3D The scene
	*/
	void IDeferredApplication::DiffuseRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Deferred Shading : Fill
		// ------------------------------
		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("diffuseRT", 0);
		this->m_DSFB.AttachElement("normalRT", 1);
		this->m_DSFB.AttachElement("specularRT", 2);
		this->m_DSFB.AttachElement("depthRT", 3);
		this->m_DSFB.AttachDepth("depth");
		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD | daidalosengine::MESHBUFFER_ELEMENT_NTB);
		renderer->SetVertexDeclaration(this->m_declaration.get());
		renderer->SetShaderProgram(this->m_DSFill.GetShaderProgram());
		this->m_DSFill.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_DSFill.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->Render();
	}

	/**
	* Render the diffuse top layer part
	* @param scene3D The scene
	*/
	void IDeferredApplication::DiffuseTopLayerRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Forward rendering: diffuse top layer
		// ------------------------------
		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->Render(SCENE_TOP_LAYER_ID);
	}

	/**
	* Render the transparent part
	* @param scene3D The scene
	*/
	void IDeferredApplication::TransparentRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, false);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		renderer->SetShaderProgram(this->m_shaderTransparent.GetShaderProgram());
		this->m_shaderTransparent.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderTransparent.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->RenderTransparent();
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
	}

	/**
	* Do the post process rendering
	*/
	void IDeferredApplication::PostProcessRendering() {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		renderer->CopyTextureToTexture(this->m_DSFB.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());

		// Draw a quad with alpha blending and alpha test
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);

		renderer->SetAlphaReference(0x00000001);
		renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		renderer->SetVertexDeclaration(this->m_screenQuadFinalVD.get());
		
		renderer->SetVertexBuffer(0, this->m_screenQuadFinalVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
		renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
		renderer->SetTexture(0, this->m_texture2DPostProcess.GetTexture());
		renderer->SetTexture(1, nullptr);

		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("postProcessRT", 0);
		this->m_DSFB.AttachDepth("depth");
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());

		// Glow
		renderer->SetShaderProgram(this->m_shaderScreenGlow.GetShaderProgram());
		this->m_shaderScreenGlow.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_GLOW_FACTOR), 4.5F);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Blur
		renderer->SetShaderProgram(this->m_shaderScreenBlur.GetShaderProgram());
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_RESOLUTION), daidalosengine::CVector2F((float)renderer->GetResolution().x, (float)renderer->GetResolution().y));
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_RADIUS), 10);

		// Horizontal convolution
		renderer->CopyTextureToTexture(this->m_DSFB.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 0);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Vertical convolution
		renderer->CopyTextureToTexture(this->m_DSFB.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 1);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
	}

	/**
	* Apply the brightness
	*/
	void IDeferredApplication::ApplyBrightness() {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// Level brightness factor
		if (this->m_currentLevel->GetBrightness() != 0) {
			if (this->m_currentLevel->GetBrightness() > 0) {
				renderer->SetTexture(0, this->m_texture2DWhite.GetTexture());
				this->m_shaderScreen.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_ALPHA), this->m_currentLevel->GetBrightness());
			} else {
				renderer->SetTexture(0, this->m_texture2DBlack.GetTexture());
				this->m_shaderScreen.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_ALPHA), this->m_currentLevel->GetBrightness() * -1.0F);
			}

			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
			renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
			renderer->SetShaderProgram(this->m_shaderScreen.GetShaderProgram());
			renderer->SetVertexBuffer(0, this->m_screenQuadFinalVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
			renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
			renderer->SetFrameBuffer(nullptr);
			renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
		}
	}

	/**
	* Render the spot lights in the RT
	* @param scene3D The scene
	*/
	void IDeferredApplication::SpotLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		for(int i = 0; i < scene3D->GetNumberOfActiveSpotLights(); ++i) {
			if(scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]) {
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
				renderer->SetViewport(daidalosengine::CRectangleI(0, 0, GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				scene3D->SetActiveCamera(scene3D->GetActiveSpotLightsBuffer()->node[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
				scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION);
				this->m_DSFB.DetachAllElements();
				this->m_DSFB.AttachElement("lightDepthRT", 0);
				this->m_DSFB.AttachDepth("lightDepth");
				renderer->SetVertexDeclaration(this->m_lightGeometryVD.get());
				renderer->SetShaderProgram(this->m_DSShadowMap.GetShaderProgram());
				renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
				renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
				scene3D->Render();
			}

			// Add the light effect to the lightRT
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
			renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

			this->m_DSFB.DetachAllElements();
			this->m_DSFB.AttachElement("lightRT", 0);
			this->m_DSFB.AttachDepth("depth");

			renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
			renderer->SetShaderProgram(this->m_DSSpotLight.GetShaderProgram());
			renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
			renderer->SetVertexBuffer(0, this->m_screenQuadLightVB.GetBuffer(), sizeof(SScreenQuadVertexLight), 0, 4);
			renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));

			renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
			renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
			renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
			renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_DSSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_DSSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), GetOptions().GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512));
			this->m_DSSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
			scene3D->SendSpotLightDataToShader(scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]);
			renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}

	/**
	* Render the point lights in the RT
	* @param scene3D The scene
	*/
	void IDeferredApplication::PointLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		// Add the light effect to the lightRT
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
		renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

		renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
		renderer->SetShaderProgram(this->m_DSPointLight.GetShaderProgram());
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->SetVertexBuffer(0, this->m_screenQuadLightVB.GetBuffer(), sizeof(SScreenQuadVertexLight), 0, 4);
		renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));

		renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
		renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
		renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
		renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

		this->m_DSPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);

		for(int i = 0; i < scene3D->GetNumberOfActivePointLights(); ++i) {
			scene3D->SendPointLightDataToShader(scene3D->GetActivePointLightsBuffer()->visibilityOrder[i]);
			renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}

	/**
	* Render the directional lights in the RT
	* @param scene3D The scene
	*/
	void IDeferredApplication::DirectionalLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		for(int i = 0; i < scene3D->GetNumberOfActiveDirectionalLights(); ++i) {
			if(scene3D->GetActiveDirectionalLightsBuffer()->shadowCaster[i]) {
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
				renderer->SetViewport(daidalosengine::CRectangleI(0, 0, GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				scene3D->SetActiveCamera(scene3D->GetActiveDirectionalLightsBuffer()->node[i]);
				scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION);
				this->m_DSFB.DetachAllElements();
				this->m_DSFB.AttachElement("lightDepthRT", 0);
				this->m_DSFB.AttachDepth("lightDepth");
				renderer->SetVertexDeclaration(this->m_lightGeometryVD.get());
				renderer->SetShaderProgram(this->m_DSShadowMap.GetShaderProgram());
				renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
				renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
				scene3D->Render();
			}

			// Add the light effect to the lightRT
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
			renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

			this->m_DSFB.DetachAllElements();
			this->m_DSFB.AttachElement("lightRT", 0);
			this->m_DSFB.AttachDepth("depth");

			renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
			renderer->SetShaderProgram(this->m_DSDirectionalLight.GetShaderProgram());
			renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
			renderer->SetVertexBuffer(0, this->m_screenQuadLightVB.GetBuffer(), sizeof(SScreenQuadVertexLight), 0, 4);
			renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));

			renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
			renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
			renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
			renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_DSDirectionalLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_DSDirectionalLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), GetOptions().GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512));
			this->m_DSDirectionalLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), scene3D->GetActiveDirectionalLightsBuffer()->shadowCaster[i]);
			scene3D->SendDirectionalLightDataToShader(i);
			renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}

	/**
	* Prepare for light rendering
	* @param scene3D The scene
	*/
	void IDeferredApplication::PreLightRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Deferred Shading : Lights
		// ------------------------------
		SScreenQuadVertexLight lightScreenQuad[] =
		{
			{ renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_BOTTOM_RIGHT].vertex, renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_BOTTOM_RIGHT].texcoord, this->m_currentLevel->GetActiveCamera()->GetViewFrustumRelativePoint(daidalosengine::FRUSTUM_POINT_FAR_BOTTOM_RIGHT) },
			{ renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_BOTTOM_LEFT].vertex, renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_BOTTOM_LEFT].texcoord, this->m_currentLevel->GetActiveCamera()->GetViewFrustumRelativePoint(daidalosengine::FRUSTUM_POINT_FAR_BOTTOM_LEFT) },
			{ renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_TOP_LEFT].vertex, renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_TOP_LEFT].texcoord, this->m_currentLevel->GetActiveCamera()->GetViewFrustumRelativePoint(daidalosengine::FRUSTUM_POINT_FAR_TOP_LEFT) },
			{ renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_TOP_RIGHT].vertex, renderer->GetScreenQuad()[daidalosengine::SCREEN_QUAD_POINT_TOP_RIGHT].texcoord, this->m_currentLevel->GetActiveCamera()->GetViewFrustumRelativePoint(daidalosengine::FRUSTUM_POINT_FAR_TOP_RIGHT) }
		};
		this->m_screenQuadLightVB.Fill(lightScreenQuad, sizeof(SScreenQuadVertexLight), 4);

		// ------------------------------
		// Deferred Shading: Prepare the light RT
		// ------------------------------
		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("lightRT", 0);
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
	}

	/**
	* Post light rendering
	* @param scene3D The scene
	*/
	void IDeferredApplication::PostLightRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Deferred Shading: Draw
		// ------------------------------
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);

		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("postProcessRT", 0);
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		this->m_DSFB.AttachDepth("depth");
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());

		renderer->SetVertexDeclaration(this->m_screenQuadFinalVD.get());
		renderer->SetShaderProgram(this->m_DSFinal.GetShaderProgram());
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		renderer->SetVertexBuffer(0, this->m_screenQuadFinalVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
		renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
		renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("diffuseRT"));
		renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("lightRT"));
		renderer->SetTexture(2, nullptr);
		renderer->SetTexture(3, nullptr);

		this->m_DSFinal.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_AMBIENT_COLOR), scene3D->GetAmbientColor());
		this->m_DSFinal.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SKYBOX_AMBIENT_COLOR), scene3D->GetSkyAmbientColor());

		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
	}

	/**
	* Render the particle system
	* @param scene3D The scene
	*/
	void IDeferredApplication::ParticleSystemRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("postProcessRT", 0);
		this->m_DSFB.AttachDepth("depth");
		renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());

		scene3D->GetParticleSystem().SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->GetParticleSystem().RenderAll();
	}

	/**
	* Finalize the deferred rendering by drawing the RTs the the backbuffer
	* @param scene3D The scene
	*/
	void IDeferredApplication::FinalizeDeferredRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Deferred Shading: Draw the post process
		// ------------------------------
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);

		renderer->SetVertexDeclaration(this->m_screenQuadFinalVD.get());
		renderer->SetVertexBuffer(0, this->m_screenQuadFinalVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
		renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
		renderer->SetShaderProgram(this->m_DSPostProcessFinal.GetShaderProgram());
		renderer->SetFrameBuffer(nullptr);
		renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("postProcessRT"));
		renderer->SetTexture(1, nullptr);
		renderer->SetTexture(2, nullptr);
		renderer->SetTexture(3, nullptr);

		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
	}
}
