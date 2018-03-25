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

		// --------------------------------------------------------------------------------------------------
		// 2D Rendering 
		// --------------------------------------------------------------------------------------------------
		// Vertex declaration used by every 2d elements
		daidalosengine::SDeclarationElement decl2D[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declaration2D = this->m_renderer->CreateVertexDeclaration(decl2D);

		// Shader program used for 2d rendering
		this->m_shader2D.LoadFromFile("2d");


		// --------------------------------------------------------------------------------------------------
		// Forward Rendering 
		// --------------------------------------------------------------------------------------------------
		daidalosengine::SDeclarationElement declarationDiffuse[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declarationDiffuse = this->m_renderer->CreateVertexDeclaration(declarationDiffuse);

		daidalosengine::SDeclarationElement declarationShadowMap[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationShadowMap = this->m_renderer->CreateVertexDeclaration(declarationShadowMap);

		daidalosengine::SDeclarationElement declarationLight[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL1, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_NORMAL2, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationLight = this->m_renderer->CreateVertexDeclaration(declarationLight);

		

		this->m_shaderDiffuse.LoadFromFile("ForwardDiffuse");
		this->m_shaderTransparent.LoadFromFile("ForwardTransparent");
		this->m_shaderShadowMap.LoadFromFile("ForwardShadowMap");
		this->m_shaderSpotLight.LoadFromFile("ForwardSpotLight");
		this->m_shaderPointLight.LoadFromFile("ForwardPointLight");
		this->m_shaderDirectionalLight.LoadFromFile("ForwardDirectionalLight");

		daidalosengine::SFrameBufferDeclaration fbd[] = {
			{ "lightDepthRT", daidalosengine::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_R32F, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		daidalosengine::SFrameBufferDepthDeclaration fbdd[] = {
			{ "lightDepth", daidalosengine::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)), daidalosengine::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferShadowMap.CreateFromDeclaration(fbd, fbdd);
		this->m_frameBufferShadowMap.AttachDepth("lightDepth");

		// --------------------------------------------------------------------------------------------------
		// Post process
		// --------------------------------------------------------------------------------------------------
		daidalosengine::SFrameBufferDeclaration fbdpp[] = {
			{ "postProcessRT", this->m_renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, textureFilter2, daidalosengine::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		daidalosengine::SFrameBufferDepthDeclaration fbddpp[] = {
			{ "postProcessDepth", this->m_renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferPostProcess.CreateFromDeclaration(fbdpp, fbddpp);
		this->m_frameBufferPostProcess.AttachElement("postProcessRT", 0);
		this->m_frameBufferPostProcess.AttachDepth("postProcessDepth");

		daidalosengine::SDeclarationElement screenQuadDecl[] =
		{
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_screenQuadPostProcessVD = this->m_renderer->CreateVertexDeclaration(screenQuadDecl);
		this->m_screenQuadPostProcessVB = this->m_renderer->CreateVertexBuffer(sizeof(daidalosengine::SScreenQuadElement), 4, daidalosengine::BUFFER_DYNAMIC | daidalosengine::BUFFER_WRITEONLY, this->m_renderer->GetScreenQuad());

		this->m_texture2DPostProcess.CreateEmpty("postProcessTexture2D", this->m_renderer->GetResolution(), daidalosengine::PIXEL_FORMAT_A8R8G8B8, &textureFilter2, daidalosengine::TEXTURE_FLAG_FRAMEBUFFER_TYPE);
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
		this->m_loadingView = new CLoadingView(daidalosengine::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));
		this->m_loadingScene->AddToMainView(this->m_loadingView);
	}

	IForwardApplication::~IForwardApplication() {
		// Remove the cinematic loader
		daidalosengine::CMediaManager::GetInstance()->RemoveLoader<ICinematicEvent>();

		delete this->m_loadingScene;
	}

	void IForwardApplication::Update(const float time) {
		this->m_fpsCounter.Update(time);

		if(this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_SHOW_FPS, false)) {
			this->m_fpsLabel.SetText(this->m_fpsCounter.GetText());
			this->m_fpsLabel.Update(time);
		}
	}

	void IForwardApplication::Render() {
		this->m_renderer->BeginScene();

		if(!this->m_loading) {
			Render3DScene(this->m_currentLevel->GetScene());
			Render2DScene(this->m_currentLevel->GetView());

			if(this->m_cursor.IsVisible()) {
				this->m_cursor.Render();
			}

		} else {
			this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
			Render2DScene(this->m_loadingScene);
		}
		this->m_fpsLabel.Render();
		
		// ------------------------------
		// Cleanup
		// ------------------------------
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetVertexDeclaration(nullptr);
		this->m_renderer->SetShaderProgram(nullptr);
		this->m_renderer->SetTexture(0, nullptr);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);

		this->m_renderer->EndScene();
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
		if(this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_DEBUG, false)) {
			scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
			scene3D->RenderDebugInformation();
		}
	}

	/**
	* Render a 2D scene
	*/
	void IForwardApplication::Render2DScene(daidalosengine::C2DScene * scene2D) {
		// ------------------------------
		// 2D elements: Prepare the renderer and draw
		// ------------------------------
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetAlphaReference(0x00000001);
		this->m_renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		this->m_renderer->SetVertexDeclaration(this->m_declaration2D.get());
		this->m_renderer->SetShaderProgram(this->m_shader2D.GetShaderProgram());
		this->m_shader2D.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_PROJ_MATRIX), scene2D->GetProjMatrix());
		scene2D->Render();
	}

	/**
	* Do initialization before rendering anything
	* @param scene3D The scene
	*/
	void IForwardApplication::InitializeRendering(daidalosengine::C3DScene * scene3D) {
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH | daidalosengine::SURFACE_TYPE_STENCIL, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
	}

	/**
	* Render the skybox
	* @param scene3D The scene
	*/
	void IForwardApplication::SkyboxRendering(daidalosengine::C3DScene * scene3D) {
		// ------------------------------
		// Sky box mask
		// ------------------------------
		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, true);
		this->m_renderer->SetStencilFunction(daidalosengine::COMPARE_ALWAYS);
		this->m_renderer->SetStencilZFail(daidalosengine::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilFail(daidalosengine::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilPass(daidalosengine::STENCIL_OPERATION_INCREMENT_CLAMP);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		scene3D->RenderSkyboxMask();

		// ------------------------------
		// Sky box
		// ------------------------------
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
		this->m_renderer->SetStencilFunction(daidalosengine::COMPARE_LESS);
		this->m_renderer->SetStencilZFail(daidalosengine::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilFail(daidalosengine::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilPass(daidalosengine::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilRef(0);
		scene3D->RenderSkybox();
	}

	/**
	* Render the diffuse part
	* @param scene3D The scene
	*/
	void IForwardApplication::DiffuseRendering(daidalosengine::C3DScene * scene3D) {
		// ------------------------------
		// Forward rendering: diffuse
		// ------------------------------
		if (this->m_cullEnabled) {
			this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		} else {
			this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);
		}

		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->Render();
	}

	/**
	* Render the diffuse top layer part
	* @param scene3D The scene
	*/
	void IForwardApplication::DiffuseTopLayerRendering(daidalosengine::C3DScene * scene3D) {
		// ------------------------------
		// Forward rendering: diffuse top layer
		// ------------------------------
		this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->Render(SCENE_TOP_LAYER_ID);
	}

	/**
	* Render the transparent part
	* @param scene3D The scene
	*/
	void IForwardApplication::TransparentRendering(daidalosengine::C3DScene * scene3D) {
		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_STENCILENABLE, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderTransparent.GetShaderProgram());
		this->m_shaderTransparent.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_shaderTransparent.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
		scene3D->RenderTransparent();
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
	}

	/**
	* Do the post process rendering
	*/
	void IForwardApplication::PostProcessRendering() {

		// Get the back buffer as a texture
		this->m_renderer->CopyBackBufferToTexture(this->m_texture2DPostProcess.GetTexture());

		// Draw a quad with alpha blending and alpha test
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);

		this->m_renderer->SetAlphaReference(0x00000001);
		this->m_renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		this->m_renderer->SetVertexDeclaration(this->m_screenQuadPostProcessVD.get());
		this->m_renderer->SetShaderProgram(this->m_shaderScreenBlur.GetShaderProgram());
		this->m_renderer->SetVertexBuffer(0, this->m_screenQuadPostProcessVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
		this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB().GetBuffer(), sizeof(int));
		this->m_renderer->SetTexture(0, this->m_texture2DPostProcess.GetTexture());

		// Blur the copied back buffer
		this->m_renderer->SetFrameBuffer(this->m_frameBufferPostProcess.GetFrameBuffer());
		this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_RESOLUTION), daidalosengine::CVector2F((float)this->m_renderer->GetResolution().x, (float)this->m_renderer->GetResolution().y));
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_RADIUS), 15);

		// Horizontal convolution
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 0);
		this->m_renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Vertical convolution
		this->m_renderer->CopyTextureToTexture(this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());
		this->m_shaderScreenBlur.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 1);
		this->m_renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Draw back to the back buffer with blending
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetShaderProgram(this->m_shaderScreenGlow.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetTexture(0, this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"));
		this->m_shaderScreenGlow.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_GLOW_FACTOR), 7.5F);
		this->m_renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
	}

	/**
	* Apply the brightness
	*/
	void IForwardApplication::ApplyBrightness() {

		// Level brightness factor
		if (this->m_currentLevel->GetBrightness() != 0) {
			if (this->m_currentLevel->GetBrightness() > 0) {
				this->m_renderer->SetTexture(0, this->m_texture2DWhite.GetTexture());
				this->m_shaderScreen.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_ALPHA), this->m_currentLevel->GetBrightness());
			} else {
				this->m_renderer->SetTexture(0, this->m_texture2DBlack.GetTexture());
				this->m_shaderScreen.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_ALPHA), this->m_currentLevel->GetBrightness() * -1.0F);
			}

			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, false);
			this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, daidalosengine::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
			this->m_renderer->SetShaderProgram(this->m_shaderScreen.GetShaderProgram());
			this->m_renderer->SetVertexBuffer(0, this->m_screenQuadPostProcessVB.GetBuffer(), sizeof(daidalosengine::SScreenQuadElement), 0, 4);
			this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB().GetBuffer(), sizeof(int));
			this->m_renderer->SetFrameBuffer(nullptr);
			this->m_renderer->DrawIndexedPrimitives(daidalosengine::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
		}
	}

	/**
	* Render the spot lights in the backbuffer
	* @param scene3D The scene
	*/
	void IForwardApplication::SpotLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_NONE);

		// Clear the shadow map framebuffer
		this->m_frameBufferShadowMap.DetachAllElements();
		this->m_frameBufferShadowMap.AttachElement("lightDepthRT", 0);
		this->m_frameBufferShadowMap.AttachDepth("lightDepth");
		this->m_renderer->SetFrameBuffer(this->m_frameBufferShadowMap.GetFrameBuffer());
		this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));

		// Loop through the lights
		for(int i = 0; i < scene3D->GetNumberOfActiveSpotLights(); ++i) {
			if(this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_ENABLED, false) && scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]) {
				this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
				this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, false);
				this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
				this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
				this->m_renderer->SetViewport(daidalosengine::CRectangleI(0, 0, this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				scene3D->SetActiveCamera(scene3D->GetActiveSpotLightsBuffer()->node[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
				scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION);
				this->m_frameBufferShadowMap.DetachAllElements();
				this->m_frameBufferShadowMap.AttachElement("lightDepthRT", 0);
				this->m_frameBufferShadowMap.AttachDepth("lightDepth");
				this->m_renderer->SetVertexDeclaration(this->m_declarationShadowMap.get());
				this->m_renderer->SetShaderProgram(this->m_shaderShadowMap.GetShaderProgram());
				this->m_renderer->SetFrameBuffer(this->m_frameBufferShadowMap.GetFrameBuffer());
				this->m_renderer->Clear(daidalosengine::SURFACE_TYPE_TARGET | daidalosengine::SURFACE_TYPE_DEPTH, daidalosengine::CColor(0.0F, 0.0F, 0.0F, 0.0F));
				scene3D->Render();
			}

			// Add the light effect to the backbuffer
			this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
			this->m_renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, false);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
			this->m_renderer->SetViewport(daidalosengine::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

			this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
			this->m_renderer->SetShaderProgram(this->m_shaderSpotLight.GetShaderProgram());
			this->m_renderer->SetFrameBuffer(nullptr);
			scene3D->SetActiveCamera(this->m_currentLevel->GetActiveCamera());
			scene3D->SetMeshFormat(daidalosengine::MESHBUFFER_ELEMENT_POSITION | daidalosengine::MESHBUFFER_ELEMENT_TEXCOORD | daidalosengine::MESHBUFFER_ELEMENT_NTB);

			this->m_renderer->SetTexture(2, this->m_frameBufferShadowMap.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_SHADOW_MAP_SIZE, 512));
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_currentLevel->GetActiveCamera()->GetFarDistance());
			this->m_shaderSpotLight.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);
			scene3D->SendSpotLightDataToShader(scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]);
			scene3D->Render();
			this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
		}
	}

	/**
	* Render the point lights in the backbuffer
	* @param scene3D The scene
	*/
	void IForwardApplication::PointLightsRendering(daidalosengine::C3DScene * scene3D) {
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
		this->m_renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, true);
		this->m_renderer->SetViewport(daidalosengine::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
		this->m_renderer->SetShaderProgram(this->m_shaderPointLight.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
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
		daidalosengine::CVector4F cameraPositionDistance(this->m_currentLevel->GetActiveCamera()->GetPosition(), this->m_currentLevel->GetActiveCamera()->GetFarDistance());

		this->m_renderer->SetCullType(daidalosengine::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE, daidalosengine::RENDER_STATE_BLEND_FUNCTION_ONE);
		this->m_renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetViewport(daidalosengine::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDirectionalLight.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
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