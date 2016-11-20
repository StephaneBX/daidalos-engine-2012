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
#include "3D/CDoorEntity.h"
#include "3D/CFanEntity.h"
#include "3D/CSwitchEntity.h"
#include "3D/CTeleporterEntity.h"
#include "3D/CAlarmEntity.h"
#include "3D/CArmorBonusEntity.h"
#include "3D/CHealthBonusEntity.h"
#include "3D/CHandgunAmmoEntity.h"
#include "3D/CShotgunAmmoEntity.h"
#include "3D/CFlashlightEntity.h"
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
		this->m_shaderTransparent.LoadFromFile("ForwardTransparent");
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

		// --------------------------------------------------------------------------------------------------
		// Post process
		// --------------------------------------------------------------------------------------------------
		DE::SFrameBufferDeclaration fbdpp[] = {
			{ "postProcessRT", this->m_renderer->GetResolution(), DE::PIXEL_FORMAT_A8R8G8B8, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE }
		};
		DE::SFrameBufferDepthDeclaration fbddpp[] = {
			{ "postProcessDepth", this->m_renderer->GetResolution(), DE::PIXEL_FORMAT_DEPTH24S8 }
		};
		this->m_frameBufferPostProcess.CreateFromDeclaration(fbdpp, fbddpp);
		this->m_frameBufferPostProcess.AttachElement("postProcessRT", 0);
		this->m_frameBufferPostProcess.AttachDepth("postProcessDepth");

		DE::SDeclarationElement screenQuadDecl[] =
		{
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_screenQuadPostProcessVD = this->m_renderer->CreateVertexDeclaration(screenQuadDecl);
		this->m_screenQuadPostProcessVB = this->m_renderer->CreateVertexBuffer(sizeof(DE::SScreenQuadElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, this->m_renderer->GetScreenQuad());

		this->m_texture2DPostProcess.CreateEmpty("postProcessTexture2D", this->m_renderer->GetResolution(), DE::PIXEL_FORMAT_A8R8G8B8, &textureFilter2, DE::TEXTURE_FLAG_FRAMEBUFFER_TYPE);
		this->m_shaderScreenBlur.LoadFromFile("ScreenBlur");
		this->m_shaderScreenGlow.LoadFromFile("ScreenGlow");

		// ------------------------------------------------------------------------------------------------
		// The Hud
		// --------------------------------------------------------------------------------------------------
		this->m_hud = new CHudView(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));
		this->m_hud->SetDebugMode(this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_DEBUG, false));

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
		this->m_entities.clear();

		// Load the doors
		std::vector<DE::ISceneNode*> doorNode = this->m_scene3D->GetNodeArray("door_entity");
		for(auto it = doorNode.begin(); it != doorNode.end(); it++) {
			SAMPLES::CDoorEntity * doorEntity = new SAMPLES::CDoorEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CDoorEntity>(doorEntity));
		}

		// Load the fans
		std::vector<DE::ISceneNode*> fanNode = this->m_scene3D->GetNodeArray("fan_entity");
		for(auto it = fanNode.begin(); it != fanNode.end(); it++) {
			SAMPLES::CFanEntity * fanEntity = new SAMPLES::CFanEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get());
			fanEntity->DoAction();
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CFanEntity>(fanEntity));
		}

		// Load the switchs
		std::vector<DE::ISceneNode*> switchNode = this->m_scene3D->GetNodeArray("switch_entity");
		for(auto it = switchNode.begin(); it != switchNode.end(); it++) {
			SAMPLES::CSwitchEntity * switchEntity = new SAMPLES::CSwitchEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CSwitchEntity>(switchEntity));
		}

		/*if(switchNode.size() > 0) {
		switchNode[0]->AddTarget(fanNode[0].get();
		}*/

		// Load the teleporters
		std::vector<DE::ISceneNode*> teleporterNode = this->m_scene3D->GetNodeArray("teleporter_entity");
		for(auto it = teleporterNode.begin(); it != teleporterNode.end(); it++) {
			SAMPLES::CTeleporterEntity * teleporterEntity = new SAMPLES::CTeleporterEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CTeleporterEntity>(teleporterEntity));
		}

		// Load the alarms
		std::vector<DE::ISceneNode*> alarmNode = this->m_scene3D->GetNodeArray("alarm_entity");
		for(auto it = alarmNode.begin(); it != alarmNode.end(); it++) {
			SAMPLES::CAlarmEntity * alarmEntity = new SAMPLES::CAlarmEntity((DE::CStaticMeshSceneNode*)(*it), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CAlarmEntity>(alarmEntity));
		}

		// Load the armor bonus
		std::vector<DE::ISceneNode*> armorNode = this->m_scene3D->GetNodeArray("armor_bonus_entity");
		for(auto it = armorNode.begin(); it != armorNode.end(); it++) {
			SAMPLES::CArmorBonusEntity * armorBonusEntity = new SAMPLES::CArmorBonusEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CArmorBonusEntity>(armorBonusEntity));
		}

		// Load the health bonus
		std::vector<DE::ISceneNode*> healthNode = this->m_scene3D->GetNodeArray("health_bonus_entity");
		for(auto it = healthNode.begin(); it != healthNode.end(); it++) {
			SAMPLES::CHealthBonusEntity * healthBonusEntity = new SAMPLES::CHealthBonusEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CHealthBonusEntity>(healthBonusEntity));
		}

		// Load the handgun ammo
		std::vector<DE::ISceneNode*> handgunAmmoNode = this->m_scene3D->GetNodeArray("handgun_ammo_entity");
		for(auto it = handgunAmmoNode.begin(); it != handgunAmmoNode.end(); it++) {
			SAMPLES::CHandgunAmmoEntity * handgunAmmoEntity = new SAMPLES::CHandgunAmmoEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CHandgunAmmoEntity>(handgunAmmoEntity));
		}

		// Load the shotgun ammo
		std::vector<DE::ISceneNode*> shotgunAmmoNode = this->m_scene3D->GetNodeArray("shotgun_ammo_entity");
		for(auto it = shotgunAmmoNode.begin(); it != shotgunAmmoNode.end(); it++) {
			SAMPLES::CShotgunAmmoEntity * shotgunAmmoEntity = new SAMPLES::CShotgunAmmoEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get(), this->m_scene3D);
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CShotgunAmmoEntity>(shotgunAmmoEntity));
		}

		// Load the flashlight
		std::vector<DE::ISceneNode*> flashlightNode = this->m_scene3D->GetNodeArray("flashlight_entity");
		for(auto it = flashlightNode.begin(); it != flashlightNode.end(); it++) {
			SAMPLES::CFlashlightEntity * flashlightEntity = new SAMPLES::CFlashlightEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_player.get());
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CFlashlightEntity>(flashlightEntity));
		}
	}

	/**
	* Render a 3D scene
	*/
	void IForwardSampleApplication::Render3DScene(DE::C3DScene * scene3D) {

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
		// Transparent
		// ------------------------------
		TransparentRendering(scene3D);

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
		if(this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_DEBUG, false)) {
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
		this->m_renderer->SetAlphaReference(0x00000001);
		this->m_renderer->SetAlphaFunction(DE::COMPARE_GREATEREQUAL);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, DE::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		this->m_renderer->SetVertexDeclaration(this->m_declaration2D.get());
		this->m_renderer->SetShaderProgram(this->m_shader2D.GetShaderProgram());
		this->m_shader2D.SetParameter(DE::SHADERPROGRAM_FLAG_VERTEX, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_PROJ_MATRIX), scene2D->GetProjMatrix());
		scene2D->Render();
	}

	/**
	* Do initialization before rendering anything
	* @param scene3D The scene
	*/
	void IForwardSampleApplication::InitializeRendering(DE::C3DScene * scene3D) {
		scene3D->SetActiveCamera(this->m_activeCamera);
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH | DE::SURFACE_TYPE_STENCIL, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
	}

	/**
	* Render the skybox
	* @param scene3D The scene
	*/
	void IForwardSampleApplication::SkyboxRendering(DE::C3DScene * scene3D) {
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
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderDiffuse.GetShaderProgram());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderDiffuse.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
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
	}

	/**
	* Render the diffuse part
	* @param scene3D The scene
	*/
	void IForwardSampleApplication::DiffuseRendering(DE::C3DScene * scene3D) {
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
	}

	/**
	* Render the transparent part
	* @param scene3D The scene
	*/
	void IForwardSampleApplication::TransparentRendering(DE::C3DScene * scene3D) {
		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_STENCILENABLE, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, DE::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		scene3D->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD);
		this->m_renderer->SetVertexDeclaration(this->m_declarationDiffuse.get());
		this->m_renderer->SetShaderProgram(this->m_shaderTransparent.GetShaderProgram());
		this->m_shaderTransparent.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shaderTransparent.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
		scene3D->RenderTransparent();
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
	}

	/**
	* Do the post process rendering
	*/
	void IForwardSampleApplication::PostProcessRendering() {

		// Get the back buffer as a texture
		this->m_renderer->CopyBackBufferToTexture(this->m_texture2DPostProcess.GetTexture());

		// Draw a quad with alpha blending and alpha test
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, DE::RENDER_STATE_BLEND_FUNCTION_ONE);

		this->m_renderer->SetAlphaReference(0x00000001);
		this->m_renderer->SetAlphaFunction(DE::COMPARE_GREATEREQUAL);
		this->m_renderer->SetVertexDeclaration(this->m_screenQuadPostProcessVD.get());
		this->m_renderer->SetShaderProgram(this->m_shaderScreenBlur.GetShaderProgram());
		this->m_renderer->SetVertexBuffer(0, this->m_screenQuadPostProcessVB, sizeof(DE::SScreenQuadElement), 0, 4);
		this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB(), sizeof(int));
		this->m_renderer->SetTexture(0, this->m_texture2DPostProcess.GetTexture());

		// Blur the copied back buffer
		this->m_renderer->SetFrameBuffer(this->m_frameBufferPostProcess.GetFrameBuffer());
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
		this->m_shaderScreenBlur.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_RESOLUTION), DE::CVector2F((float)this->m_renderer->GetResolution().x, (float)this->m_renderer->GetResolution().y));
		this->m_shaderScreenBlur.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_BLUR_RADIUS), 15);

		// Horizontal convolution
		this->m_shaderScreenBlur.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 0);
		this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Vertical convolution
		this->m_renderer->CopyTextureToTexture(this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"), this->m_texture2DPostProcess.GetTexture());
		this->m_shaderScreenBlur.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_BLUR_CONVOLUTION_MODE), 1);
		this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// Draw back to the back buffer with blending
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetShaderProgram(this->m_shaderScreenGlow.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetTexture(0, this->m_frameBufferPostProcess.GetFrameBuffer()->GetTexture("postProcessRT"));
		this->m_shaderScreenGlow.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_GLOW_FACTOR), 6.5f);
		this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, false);
	}

	/**
	* Render the spot lights in the backbuffer
	* @param scene3D The scene
	*/
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
			scene3D->RenderTransparent();
		}
	}

	/**
	* Render the point lights in the backbuffer
	* @param scene3D The scene
	*/
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
			scene3D->RenderTransparent();
		}
	}

	/**
	* Render the directional lights in the backbuffer
	* @param scene3D The scene
	*/
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
			scene3D->RenderTransparent();
		}
	}
}