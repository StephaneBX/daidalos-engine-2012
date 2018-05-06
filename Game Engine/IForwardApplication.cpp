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

#include <Core/Core.hpp>
#include <Core/CMediaManager.hpp>
#include "IForwardApplication.hpp"
#include "Loaders/CCinematicEventLoader.hpp"
#include "Event/ICinematicEvent.hpp"

namespace daidalosgameengine {

	IForwardApplication::IForwardApplication(const HINSTANCE hInstance, const int nCmdShow) : IWin32Application(hInstance, nCmdShow, "Daidalos Engine") {
		// Cinematic loader
		daidalosengine::CMediaManager::GetInstance()->AddLoader(new CCinematicEventLoader(), "dgc");
		daidalosengine::CMediaManager::GetInstance()->AddSearchPath("Resources/Cinematics/");


		this->m_loading = false;
		this->m_cullEnabled = true;
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
		daidalosengine::SDeclarationElement decl2D[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declaration2D = renderer->CreateVertexDeclaration(decl2D);

		// Shader program used for 2d rendering
		this->m_shader2D.LoadFromFile("2d");


		// --------------------------------------------------------------------------------------------------
		// Forward Rendering 
		// --------------------------------------------------------------------------------------------------
		daidalosengine::SDeclarationElement declarationDiffuse[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declarationDiffuse = renderer->CreateVertexDeclaration(declarationDiffuse);

		daidalosengine::SDeclarationElement declarationShadowMap[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationShadowMap = renderer->CreateVertexDeclaration(declarationShadowMap);

		daidalosengine::SDeclarationElement declarationLight[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL1, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL2, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationLight = renderer->CreateVertexDeclaration(declarationLight);

		

		this->m_shaderDiffuse.LoadFromFile("ForwardDiffuse");
		this->m_shaderTransparent.LoadFromFile("ForwardTransparent");
		this->m_shaderShadowMap.LoadFromFile("ForwardShadowMap");
		this->m_shaderSpotLight.LoadFromFile("ForwardSpotLight");
		this->m_shaderPointLight.LoadFromFile("ForwardPointLight");
		this->m_shaderDirectionalLight.LoadFromFile("ForwardDirectionalLight");

		daidalosengine::SFrameBufferDeclaration fbd[] = {
			{ "lightDepthRT", daidalosengine::CVector2I(GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_R32F, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		daidalosengine::SFrameBufferDepthDeclaration fbdd[] = {
			{ "lightDepth", daidalosengine::CVector2I(GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferShadowMap.CreateFromDeclaration(fbd, fbdd);
		this->m_frameBufferShadowMap.AttachDepth("lightDepth");

		// --------------------------------------------------------------------------------------------------
		// Post process
		// --------------------------------------------------------------------------------------------------
		daidalosengine::SFrameBufferDeclaration fbdpp[] = {
			{ "postProcessRT", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		daidalosengine::SFrameBufferDepthDeclaration fbddpp[] = {
			{ "postProcessDepth", renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferPostProcess.CreateFromDeclaration(fbdpp, fbddpp);
		this->m_frameBufferPostProcess.AttachElement("postProcessRT", 0);
		this->m_frameBufferPostProcess.AttachDepth("postProcessDepth");

		daidalosengine::SDeclarationElement screenQuadDecl[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_screenQuadPostProcessVD = renderer->CreateVertexDeclaration(screenQuadDecl);
		this->m_screenQuadPostProcessVB = renderer->CreateVertexBuffer(sizeof(daidalosengine::SScreenQuadElement), 4, daidalosengine::BUFFER_DYNAMIC | daidalosengine::BUFFER_WRITEONLY, renderer->GetScreenQuad());

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

	IForwardApplication::~IForwardApplication() {
		// Remove the cinematic loader
		daidalosengine::CMediaManager::GetInstance()->RemoveLoader<ICinematicEvent>();

		delete this->m_loadingScene;
	}

	void IForwardApplication::Update(const float time) {
		this->m_fpsCounter.Update(time);

		if(GetOptions().GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_SHOW_FPS, false)) {
			this->m_fpsLabel.SetText(this->m_fpsCounter.GetText());
			this->m_fpsLabel.Update(time);
		}
	}

	void IForwardApplication::Render() {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		renderer->BeginScene();

		if(!this->m_loading) {
			Render3DScene(this->m_currentLevel->GetScene());
			Render2DScene(this->m_currentLevel->GetView());

			if(this->m_cursor.IsVisible()) {
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
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);

		renderer->EndScene();
	}

	/**
	* Render a 3D scene
	*/
	void IForwardApplication::Render3DScene(daidalosengine::C3DScene * scene3D) {

		// ------------------------------
		// Initialization
		// ------------------------------
		InitializeRendering(scene3D);
		
		// ------------------------------
		// Sky box mask + rendering
		// ------------------------------
		SkyboxRendering(scene3D);

		// ------------------------------
		// Diffuse
		// ------------------------------
		DiffuseRendering(scene3D);

		// ------------------------------
		// Post processing
		// ------------------------------
		PostProcessRendering();

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
		// Transparent
		// ------------------------------
		TransparentRendering(scene3D);

		// ------------------------------
		// Particle System
		// ------------------------------
		scene3D->GetParticleSystem().SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->GetParticleSystem().RenderAll();

		// ------------------------------
		// Diffuse - Top layer
		// ------------------------------
		DiffuseTopLayerRendering(scene3D);

		// ------------------------------
		// Brightness factor
		// ------------------------------
		ApplyBrightness();

		// ------------------------------
		// Debug
		// ------------------------------
		if(GetOptions().GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_DEBUG, false)) {
			scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
			scene3D->RenderDebugInformation();
		}
	}

	/**
	* Render a 2D scene
	*/
	void IForwardApplication::Render2DScene(daidalosengine::C2DScene * scene2D) {
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
	void IForwardApplication::InitializeRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		renderer->SetFrameBuffer(nullptr);
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
	}

	/**
	* Render the skybox
	* @param scene3D The scene
	*/
	void IForwardApplication::SkyboxRendering(daidalosengine::C3DScene * scene3D) {
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
		renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
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
	void IForwardApplication::DiffuseRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Forward rendering: diffuse
		// ------------------------------
		if (this->m_cullEnabled) {
			renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		} else {
			renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);
		}

		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->Render();
	}

	/**
	* Render the diffuse top layer part
	* @param scene3D The scene
	*/
	void IForwardApplication::DiffuseTopLayerRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// ------------------------------
		// Forward rendering: diffuse top layer
		// ------------------------------
		renderer->Clear(daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
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
	void IForwardApplication::TransparentRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
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
	void IForwardApplication::PostProcessRendering() {

		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		// Get the back buffer as a texture
		renderer->CopyBackBufferToTexture(this->m_texture2DPostProcess.GetTexture());

		// Draw a quad with alpha blending and alpha test
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);

		renderer->SetAlphaReference(0x00000001);
		renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		renderer->SetVertexDeclaration(this->m_screenQuadPostProcessVD.get());
		renderer->SetShaderProgram(this->m_shaderScreenBlur.GetShaderProgram());
		renderer->SetVertexBuffer(0, this->m_screenQuadPostProcessVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
		renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
		renderer->SetTexture(0, this->m_texture2DPostProcess.GetTexture());

		// Blur the copied back buffer
		renderer->SetFrameBuffer(this->m_frameBufferPostProcess.GetFrameBuffer());
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_RESOLUTION), daidalosengine::CVector2F((float)renderer->GetResolution().x, (float)renderer->GetResolution().y));
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_RADIUS), 15);

		// Horizontal convolution
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 0);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Vertical convolution
		renderer->CopyTextureToTexture(this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 1);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Draw back to the back buffer with blending
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetShaderProgram(this->m_shaderScreenGlow.GetShaderProgram());
		renderer->SetFrameBuffer(nullptr);
		renderer->SetTexture(0, this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"));
		this->m_shaderScreenGlow.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_GLOW_FACTOR), 7.5F);
		renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
	}

	/**
	* Apply the brightness
	*/
	void IForwardApplication::ApplyBrightness() {
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
			renderer->SetVertexBuffer(0, this->m_screenQuadPostProcessVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
			renderer->SetIndexBuffer(renderer->GetQuadIB().GetBuffer(), sizeof(int));
			renderer->SetFrameBuffer(nullptr);
			renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
		}
	}

	/**
	* Render the spot lights in the backbuffer
	* @param scene3D The scene
	*/
	void IForwardApplication::SpotLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);

		// Clear the shadow map framebuffer
		this->m_frameBufferShadowMap.DetachAllElements();
		this->m_frameBufferShadowMap.AttachElement("lightDepthRT", 0);
		this->m_frameBufferShadowMap.AttachDepth("lightDepth");
		renderer->SetFrameBuffer(this->m_frameBufferShadowMap.GetFrameBuffer());
		renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));

		// Loop through the lights
		for(int i = 0; i < scene3D->GetNumberOfActiveSpotLights(); ++i) {
			if(GetOptions().GetValueBool(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_ENABLED, false) && scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]) {
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
				renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
				renderer->SetViewport(daidalosengine::CRectangleI(0, 0, GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), GetOptions().GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				scene3D->SetActiveCamera(scene3D->GetActiveSpotLightsBuffer()->node[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
				scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION);
				this->m_frameBufferShadowMap.DetachAllElements();
				this->m_frameBufferShadowMap.AttachElement("lightDepthRT", 0);
				this->m_frameBufferShadowMap.AttachDepth("lightDepth");
				renderer->SetVertexDeclaration(this->m_declarationShadowMap.get());
				renderer->SetShaderProgram(this->m_shaderShadowMap.GetShaderProgram());
				renderer->SetFrameBuffer(this->m_frameBufferShadowMap.GetFrameBuffer());
				renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
				scene3D->Render();
			}

			// Add the light effect to the backbuffer
			renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
			renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, false);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
			renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

			renderer->SetVertexDeclaration(this->m_declarationLight.get());
			renderer->SetShaderProgram(this->m_shaderSpotLight.GetShaderProgram());
			renderer->SetFrameBuffer(nullptr);
			scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
			scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD | daidalosengine::MESHBUFFER_ELEMENT_NTB);

			renderer->SetTexture(2, this->m_frameBufferShadowMap.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), GetOptions().GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512));
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
			scene3D->SendSpotLightDataToShader(scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]);
			scene3D->Render();
			renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
		}
	}

	/**
	* Render the point lights in the backbuffer
	* @param scene3D The scene
	*/
	void IForwardApplication::PointLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
		renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
		renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

		renderer->SetVertexDeclaration(this->m_declarationLight.get());
		renderer->SetShaderProgram(this->m_shaderPointLight.GetShaderProgram());
		renderer->SetFrameBuffer(nullptr);
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD | daidalosengine::MESHBUFFER_ELEMENT_NTB);

		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);

		for(int i = 0; i < scene3D->GetNumberOfActivePointLights(); ++i) {
			// Add the light effect to the backbuffer
			scene3D->SendPointLightDataToShader(scene3D->GetActivePointLightsBuffer()->visibilityOrder[i]);
			scene3D->Render();
		}
	}

	/**
	* Render the directional lights in the backbuffer
	* @param scene3D The scene
	*/
	void IForwardApplication::DirectionalLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::IRenderer* renderer = daidalosengine::IRenderer::GetRenderer();

		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
		renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		renderer->SetViewport(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));

		renderer->SetVertexDeclaration(this->m_declarationLight.get());
		renderer->SetShaderProgram(this->m_shaderDirectionalLight.GetShaderProgram());
		renderer->SetFrameBuffer(nullptr);
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD | daidalosengine::MESHBUFFER_ELEMENT_NTB);

		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderPointLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);

		for(int i = 0; i < scene3D->GetNumberOfActiveDirectionalLights(); ++i) {
			// Add the light effect to the backbuffer
			scene3D->SendDirectionalLightDataToShader(i);
			scene3D->Render();
		}
	}

	/**
	* Check the laoding state
	* @return true if the level is loading
	*/
	const bool IForwardApplication::IsLoading() const {
		return this->m_loading;
	}

	/**
	* Check if the cull is enabled
	* @return true if the cull is enabled
	*/
	const bool IForwardApplication::IsCullEnabled() const {
		return this->m_cullEnabled;
	}

	/**
	* Get the current level
	* @return the current level
	*/
	daidalosgameengine::ILevel * IForwardApplication::GetCurrentLevel() const {
		return this->m_currentLevel.get();
	}

	/**
	* Get the loading scene
	* @return The loading scene
	*/
	daidalosengine::C2DScene * IForwardApplication::GetLoadingScene() const  {
		return this->m_loadingScene;
	}

	/**
	* Get the cursor
	* @return The cursor
	*/
	daidalosengine::CCursor & IForwardApplication::GetCursor() {
		return this->m_cursor;
	}

	/**
	* Set the loading state
	* @param loading The loading state
	*/
	void IForwardApplication::SetLoading(const bool loading) {
		this->m_loading = loading;
	}

	/**
	* Set the cull
	* @param cullEnabled The cull
	*/
	void IForwardApplication::SetCullEnabled(const bool cullEnabled) {
		this->m_cullEnabled = cullEnabled;
	}

	/**
	* Set the current level
	* @param level The level
	*/
	void IForwardApplication::SetLevel(daidalosgameengine::ILevel * level) {
		this->m_currentLevel = std::unique_ptr<daidalosgameengine::ILevel>(level);
	}
}