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

#include <Core/Core.h>
#include "IForwardSampleApplication.h"

namespace SAMPLES {

	IForwardSampleApplication::IForwardSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : IWin32Application(hInstance, nCmdShow, "Daidalos Engine") {
		this->m_loading = false;
		this->m_scene3D = new DE::C3DScene();
		this->m_scene2D = new DE::C2DScene();
		this->m_loadingScene = new DE::C2DScene();

		this->m_cursor.SetTexture("cursor.png");
		this->m_cursor.SetVisible(false);
		this->m_scene2D->SetCursor(&this->m_cursor);

		DE::STexture2DFilterPolicy textureFilter2;
		textureFilter2.minFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.magFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.mipFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.addressU = DE::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.addressV = DE::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.maxAnisotropy = 0;

		// --------------------------------------------------------------------------------------------------
		// 2D Rendering 
		// --------------------------------------------------------------------------------------------------
		// Vertex declaration used by every 2d elements
		DE::SDeclarationElement decl2D[] = {
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declaration2D = this->m_renderer->CreateVertexDeclaration(decl2D);

		// Shader program used for 2d rendering
		this->m_shader2D.LoadFromFile("2d");


		// --------------------------------------------------------------------------------------------------
		// Forward Rendering 
		// --------------------------------------------------------------------------------------------------
		DE::SDeclarationElement declarationDiffuse[] = {
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_declarationDiffuse = this->m_renderer->CreateVertexDeclaration(declarationDiffuse);

		DE::SDeclarationElement declarationShadowMap[] = {
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationShadowMap = this->m_renderer->CreateVertexDeclaration(declarationShadowMap);

		DE::SDeclarationElement declarationLight[] = {
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2 },
			{ 0, DE::DECLARATION_USAGE_NORMAL0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_NORMAL1, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_NORMAL2, DE::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_declarationLight = this->m_renderer->CreateVertexDeclaration(declarationLight);

		this->m_shaderDiffuse.LoadFromFile("ForwardDiffuse");
		this->m_shaderShadowMap.LoadFromFile("ForwardShadowMap");
		this->m_shaderSpotLight.LoadFromFile("ForwardSpotLight");
		this->m_shaderPointLight.LoadFromFile("ForwardPointLight");
		this->m_shaderDirectionalLight.LoadFromFile("ForwardDirectionalLight");

		DE::SFrameBufferDeclaration fbd[] = {
			{ "lightDepthRT", DE::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)), DE::PIXEL_FORMAT_R32F, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		DE::SFrameBufferDepthDeclaration fbdd[] = {
			{ "lightDepth", DE::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)), DE::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferShadowMap.CreateFromDeclaration(fbd, fbdd);
		this->m_frameBufferShadowMap.AttachDepth("lightDepth");

		// ------------------------------------------------------------------------------------------------
		// The Hud
		// --------------------------------------------------------------------------------------------------
		this->m_hud = new CHudView(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		// ------------------------------------------------------------------------------------------------
		// The loading scene
		// --------------------------------------------------------------------------------------------------
		this->m_loadingView = new CLoadingView(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));
		this->m_loadingScene->AddToMainView(this->m_loadingView);
	}

	IForwardSampleApplication::~IForwardSampleApplication() {
		delete this->m_scene3D;
		delete this->m_scene2D;
		delete this->m_loadingScene;
		delete this->m_hud;
	}

	void IForwardSampleApplication::Update(const float time) {
		if(!this->m_loading) {
			this->m_fpsCounter.Update(time);

			this->m_cursor.Update(time);
			this->m_scene2D->Update(time);

		} else {
			this->m_loadingScene->Update(time);
		}
	}

	void IForwardSampleApplication::Render() {
		this->m_renderer->BeginScene();

		if(!this->m_loading) {
			Render3DScene(this->m_scene3D);
			Render2DScene(this->m_scene2D);

			if(this->m_cursor.IsVisible()) {
				this->m_cursor.Render();
			}

		} else {
			this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH | DE::SURFACE_TYPE_STENCIL, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
			Render2DScene(this->m_loadingScene);
		}
		
		// ------------------------------
		// Cleanup
		// ------------------------------
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetVertexDeclaration(nullptr);
		this->m_renderer->SetShaderProgram(nullptr);
		this->m_renderer->SetTexture(0, nullptr);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, false);

		this->m_renderer->EndScene();
	}

	/**
	* Load a level and show a loading screen
	* @param filename The level filename
	*/
	void IForwardSampleApplication::LoadLevel(const std::string & filename) {
		this->m_loading = true;
		this->m_scene3D->LoadScene(filename);
		this->m_activeCamera = (DE::ICameraSceneNode*)this->m_scene3D->GetNode("player");

		this->m_scene3D->GetRootNode()->SetMeshShadingType(DE::MESH_SHADING_FLAT);
		this->m_scene3D->SetActiveCamera(this->m_activeCamera);
		this->m_scene3D->UpdateAll(0);
		PostLoadLevel();

		this->m_loading = false;
	}

	/**
	* Do post level loading process
	*/
	void IForwardSampleApplication::PostLoadLevel() {
	}

	/**
	* Render a 3D scene
	*/
	void IForwardSampleApplication::Render3DScene(DE::C3DScene * scene3D) {
		// ------------------------------
		// Sky box mask
		// ------------------------------
		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_NONE);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_STENCILENABLE, true);
		this->m_renderer->SetStencilFunction(DE::COMPARE_ALWAYS);
		this->m_renderer->SetStencilZFail(DE::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilFail(DE::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilPass(DE::STENCIL_OPERATION_INCREMENT_CLAMP);
		scene3D->SetActiveCamera(this->m_activeCamera);
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH | DE::SURFACE_TYPE_STENCIL, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
		scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD);
		scene3D->RenderSkyboxMask();

		// ------------------------------
		// Sky box
		// ------------------------------
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		scene3D->SetActiveCamera(this->m_activeCamera);
		this->m_renderer->SetStencilFunction(DE::COMPARE_LESS);
		this->m_renderer->SetStencilZFail(DE::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilFail(DE::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilPass(DE::STENCIL_OPERATION_KEEP);
		this->m_renderer->SetStencilRef(0);
		scene3D->RenderSkybox();

		// ------------------------------
		// Forward rendering: diffuse
		// ------------------------------
		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_STENCILENABLE, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
		scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
		scene3D->RenderAll();

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
		// Particle System
		// ------------------------------
		scene3D->GetParticleSystem().SetActiveCamera(this->m_activeCamera);
		scene3D->GetParticleSystem().RenderAll();

		// ------------------------------
		// Debug
		// ------------------------------
		if(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_GENERAL_DEBUG, false)) {
			scene3D->SetActiveCamera(this->m_activeCamera);
			scene3D->RenderDebugInformation();
		}
	}

	/**
	* Render a 2D scene
	*/
	void IForwardSampleApplication::Render2DScene(DE::C2DScene * scene2D) {
		// ------------------------------
		// 2D elements: Prepare the renderer and draw
		// ------------------------------
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, DE::RENDER_STATE_BLEND_FUNCTION_DESTALPHA);
		this->m_renderer->SetVertexDeclaration(this->m_declaration2D.get());
		this->m_renderer->SetShaderProgram(this->m_shader2D.GetShaderProgram());
		this->m_shader2D.SetParameter(DE::SHADERPROGRAM_FLAG_VERTEX, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_PROJ_MATRIX), scene2D->GetProjMatrix());
		scene2D->Render();
	}

	void IForwardSampleApplication::SpotLightsRendering(DE::C3DScene * scene3D) {
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());
		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_NONE);

		for(int i = 0; i < scene3D->GetNumberOfActiveSpotLights(); ++i) {
			if(scene3D->GetActiveSpotLightsBuffer()->shadowCaster[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]) {
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
				this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				scene3D->SetActiveCamera(scene3D->GetActiveSpotLightsBuffer()->node[scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
				scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION);
				this->m_frameBufferShadowMap.DetachAllElements();
				this->m_frameBufferShadowMap.AttachElement("lightDepthRT", 0);
				this->m_frameBufferShadowMap.AttachDepth("lightDepth");
				this->m_renderer->SetVertexDeclaration(this->m_declarationShadowMap.get());
				this->m_renderer->SetShaderProgram(this->m_shaderShadowMap.GetShaderProgram());
				this->m_renderer->SetFrameBuffer(this->m_frameBufferShadowMap.GetFrameBuffer());
				this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
				scene3D->RenderAll();
			}

			// Add the light effect to the backbuffer
			this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_ONE, DE::RENDER_STATE_BLEND_FUNCTION_ONE);
			this->m_renderer->SetBlendOperation(DE::RENDER_STATE_BLEND_OPERATION_ADD);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, true);
			this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

			this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
			this->m_renderer->SetShaderProgram(this->m_shaderSpotLight.GetShaderProgram());
			this->m_renderer->SetFrameBuffer(nullptr);
			scene3D->SetActiveCamera(this->m_activeCamera);
			scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD | DE::MESHBUFFER_ELEMENT_NTB);

			this->m_renderer->SetTexture(2, this->m_frameBufferShadowMap.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_shaderSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_shaderSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512));
			this->m_shaderSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), scene3D->GetActiveSpotLightsBuffer()->shadowCaster[this->m_scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
			this->m_shaderSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
			this->m_shaderSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
			scene3D->SendSpotLightDataToShader(scene3D->GetActiveSpotLightsBuffer()->visibilityOrder[i]);
			scene3D->RenderAll();
		}
	}

	void IForwardSampleApplication::PointLightsRendering(DE::C3DScene * scene3D) {
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());

		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_ONE, DE::RENDER_STATE_BLEND_FUNCTION_ONE);
		this->m_renderer->SetBlendOperation(DE::RENDER_STATE_BLEND_OPERATION_ADD);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, true);
		this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
		this->m_renderer->SetShaderProgram(this->m_shaderPointLight.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
		scene3D->SetActiveCamera(this->m_activeCamera);
		scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD | DE::MESHBUFFER_ELEMENT_NTB);

		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);

		for(int i = 0; i < scene3D->GetNumberOfActivePointLights(); ++i) {
			// Add the light effect to the backbuffer
			scene3D->SendPointLightDataToShader(scene3D->GetActivePointLightsBuffer()->visibilityOrder[i]);
			scene3D->RenderAll();
		}
	}

	void IForwardSampleApplication::DirectionalLightsRendering(DE::C3DScene * scene3D) {
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());

		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_ONE, DE::RENDER_STATE_BLEND_FUNCTION_ONE);
		this->m_renderer->SetBlendOperation(DE::RENDER_STATE_BLEND_OPERATION_ADD);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		this->m_renderer->SetVertexDeclaration(this->m_declarationLight.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDirectionalLight.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
		scene3D->SetActiveCamera(this->m_activeCamera);
		scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD | DE::MESHBUFFER_ELEMENT_NTB);

		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);

		for(int i = 0; i < scene3D->GetNumberOfActiveDirectionalLights(); ++i) {
			// Add the light effect to the backbuffer
			scene3D->SendDirectionalLightDataToShader(i);
			scene3D->RenderAll();
		}
	}
}