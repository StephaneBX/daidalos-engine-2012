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
#include <Scene/ICameraSceneNode.h>
#include <Scene/CSpotLightSceneNode.h>
#include <Scene/IMeshSceneNode.h>
#include <Core/Core.h>
#include <Core/Logger/CLoggerFile.h>
#include <Core/Utility/CMatrix.h>
#include <Core/CGUIManager.h>
#include <Medias/CMaterial.h>
#include <Medias/CStaticMesh.h>
#include <Medias/CFont.h>
#include "ISampleApplication.h"

namespace SAMPLES
{
	/**
	* Test structure
	*/
	struct STest
	{
		std::string aaa;
		int bbb;
	};

	/**
	* Screen quad used for deferred shading
	*/
	struct SScreenQuadVertexLight
	{
		DE::CVector3F vertex;
		DE::CVector2F texcoord;
		DE::CVector3F viewDistance;
	};

	struct SLightVertexGeometry
	{
		DE::CVector3F vertex;
	};

	ISampleApplication::ISampleApplication(const HINSTANCE hInstance, const int nCmdShow) : IWin32Application(hInstance, nCmdShow, "Daidalos Engine")
	{
		this->m_scene = DE::CSceneManager::GetInstance();

		DE::STexture2DFilterPolicy textureFilter2;
		textureFilter2.minFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.magFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.mipFilter = DE::TEXTURE_FILTER_TYPE_POINT;
		textureFilter2.addressU = DE::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.addressV = DE::TEXTURE_ADDRESS_CLAMP;
		textureFilter2.maxAnisotropy = 0;

		// --------------------------------------------------------------------------------------------------
		// Deferred Shading
		// --------------------------------------------------------------------------------------------------

		// Main Frame buffer
		DE::SFrameBufferDeclaration fbd[] =
		{
			{"diffuseRT", this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_A8R8G8B8, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE},
			{"normalRT",this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_A8R8G8B8, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE},
			{"specularRT", this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_A8R8G8B8, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE},
			{"depthRT", this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_R32F, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE},
			{"lightRT", this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_A8R8G8B8, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE},
			{"lightDepthRT", DE::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)), DE::PIXEL_FORMAT_R32F, textureFilter2, DE::FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE}
		};
		DE::SFrameBufferDepthDeclaration fbdd[] = 
		{
			{"depth", this->m_renderer->GetInternalResolution(), DE::PIXEL_FORMAT_DEPTH24S8},
			{"lightDepth", DE::CVector2I(this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)), DE::PIXEL_FORMAT_DEPTH24S8}
		};
		this->m_DSFB.CreateFromDeclaration(fbd, fbdd);
		this->m_DSFB.AttachDepth("depth");

		// Light geometry
		DE::SDeclarationElement lightGeometryDecl[] =
		{
			 {0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3}
		};
		this->m_lightGeometryVD = this->m_renderer->CreateVertexDeclaration(lightGeometryDecl);

		// Light screen quad
		DE::SDeclarationElement screenQuadDecl1[] =
		{
			{0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2},
			{0, DE::DECLARATION_USAGE_TEXCOORD1, DE::DECLARATION_TYPE_FLOAT3}
		};
		this->m_screenQuadLightVD = this->m_renderer->CreateVertexDeclaration(screenQuadDecl1);
		DE::SDeclarationElement screenQuadDecl2[] =
		{
			{0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2}
		};
		this->m_screenQuadLightVD2 = this->m_renderer->CreateVertexDeclaration(screenQuadDecl2);
		this->m_screenQuadLightVB = this->m_renderer->CreateVertexBuffer(sizeof(SScreenQuadVertexLight), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, nullptr);

		// Final screen quad
		DE::SDeclarationElement screenQuadDecl3[] =
		{
			{0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2}
		};
		this->m_screenQuadFinalVD = this->m_renderer->CreateVertexDeclaration(screenQuadDecl3);
		this->m_screenQuadFinalVB = this->m_renderer->CreateVertexBuffer(sizeof(DE::SScreenQuadElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, this->m_renderer->GetScreenQuad());

		// Shaders
		this->m_DSFill.LoadFromFile("DS_fill_normal_mapping");
		this->m_DSShadowMap.LoadFromFile("DS_shadowMap");
		this->m_DSSpotLight.LoadFromFile("DS_spotLight");
		this->m_DSPointLight.LoadFromFile("DS_pointLight");
		this->m_DSDirectionalLight.LoadFromFile("DS_directionalLight");
		this->m_DSFinal.LoadFromFile("DS_final");


		// --------------------------------------------------------------------------------------------------
		// Deferred Shading
		// --------------------------------------------------------------------------------------------------
		DE::SDeclarationElement decl[] =
		{
			{0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2},
			{0, DE::DECLARATION_USAGE_NORMAL0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_NORMAL1, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_NORMAL2, DE::DECLARATION_TYPE_FLOAT3}
		};
		this->m_declaration = this->m_renderer->CreateVertexDeclaration(decl);

		
		// ------------------------------------------------------------------------------------------------
		// The texts
		// --------------------------------------------------------------------------------------------------
		DE::CGUIManager::GetInstance()->SetCursorTexture("cursor.png");
		DE::CGUIManager::GetInstance()->SetCursorVisible(true);

		this->m_positionText.SetBounds(DE::CRectangleI(5,0,200,30));
		this->m_positionText.SetSize(25);
		this->m_positionText.SetColor(DE::CColor(1.0f, 0.0f, 0.0f, 1.0f));
		this->m_positionText.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_positionText.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		DE::CGUIManager::GetInstance()->AddToMainView(this->m_positionText.GetGUIElement());

		this->m_fpsText.SetBounds(DE::CRectangleI((int)(this->m_renderer->GetResolution().x - 150),0,150,30));
		this->m_fpsText.SetSize(30);
		this->m_fpsText.SetColor(DE::CColor(255, 255, 102, 255));
		this->m_fpsText.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_fpsText.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		DE::CGUIManager::GetInstance()->AddToMainView(this->m_fpsText.GetGUIElement());


		// Button test
		this->m_button.SetDefaultTexture("button1.png");
		this->m_button.SetFocusedTexture("button3.png");
		this->m_button.SetClickedTexture("button2.png");
		this->m_button.SetBounds(DE::CRectangleI(100,200,350,100));
		this->m_button.SetImageInset(DE::CInsetF(0.2f, 0.2f, 0.2f, 0.2f));
		DE::CGUIManager::GetInstance()->AddToMainView(this->m_button.GetGUIElement());
	}

	ISampleApplication::~ISampleApplication() 
	{
		DE::CSceneManager::Kill();
	}

	void ISampleApplication::Render()
	{
		this->m_renderer->BeginScene();

		// ------------------------------
		// Sky box
		// ------------------------------
		this->m_renderer->SetCullType(DE::RENDER_STATE_CULL_TYPE_CCW);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_scene->SetActiveCamera(this->m_activeCamera);
		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("diffuseRT", 0);
		this->m_DSFB.AttachElement("normalRT", 1);
		this->m_DSFB.AttachElement("specularRT", 2);
		this->m_DSFB.AttachElement("depthRT", 3);
		this->m_DSFB.AttachDepth("depth");
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
		this->m_scene->RenderSkybox();


		// ------------------------------
		// Deferred Shading : Fill
		// ------------------------------
		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("diffuseRT", 0);
		this->m_DSFB.AttachElement("normalRT", 1);
		this->m_DSFB.AttachElement("specularRT", 2);
		this->m_DSFB.AttachElement("depthRT", 3);
		this->m_DSFB.AttachDepth("depth");
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
		this->m_scene->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION | DE::MESHBUFFER_ELEMENT_TEXCOORD | DE::MESHBUFFER_ELEMENT_NTB);
		this->m_renderer->SetVertexDeclaration(this->m_declaration.get());
		this->m_renderer->SetShaderProgram(this->m_DSFill.GetShaderProgram());
		this->m_DSFill.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_DSFill.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7f);
		this->m_scene->RenderAll();

		// ------------------------------
		// Deferred Shading : Lights
		// ------------------------------
		SScreenQuadVertexLight lightScreenQuad[] =
		{
			{this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_BOTTOM_RIGHT].vertex, this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_BOTTOM_RIGHT].texcoord, this->m_activeCamera->GetViewFrustumRelativePoint(DE::FRUSTUM_POINT_FAR_BOTTOM_RIGHT)},
			{this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_BOTTOM_LEFT].vertex, this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_BOTTOM_LEFT].texcoord, this->m_activeCamera->GetViewFrustumRelativePoint(DE::FRUSTUM_POINT_FAR_BOTTOM_LEFT)},
			{this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_TOP_LEFT].vertex, this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_TOP_LEFT].texcoord, this->m_activeCamera->GetViewFrustumRelativePoint(DE::FRUSTUM_POINT_FAR_TOP_LEFT)},
			{this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_TOP_RIGHT].vertex, this->m_renderer->GetScreenQuad()[DE::SCREEN_QUAD_POINT_TOP_RIGHT].texcoord, this->m_activeCamera->GetViewFrustumRelativePoint(DE::FRUSTUM_POINT_FAR_TOP_RIGHT)}
		};
		this->m_screenQuadLightVB.Fill(lightScreenQuad, sizeof(SScreenQuadVertexLight), 4);

		// ------------------------------
		// Deferred Shading: Prepare the light RT
		// ------------------------------
		this->m_DSFB.DetachAllElements();
		this->m_DSFB.AttachElement("lightRT", 0);
		this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_ONE, DE::RENDER_STATE_BLEND_FUNCTION_ONE);

		// ------------------------------
		// Deferred Shading: Spot lights
		// ------------------------------
		SpotLightsRendering();

		// ------------------------------
		// Deferred Shading: Point lights
		// ------------------------------
		PointLightsRendering();

		// ------------------------------
		// Deferred Shading: Directional lights
		// ------------------------------
		DirectionalLightsRendering();

		// ------------------------------
		// Deferred Shading: Draw
		// ------------------------------
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
		this->m_renderer->SetBlendFunction(DE::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, DE::RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);

		this->m_renderer->SetVertexDeclaration(this->m_screenQuadFinalVD.get());
		this->m_renderer->SetShaderProgram(this->m_DSFinal.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(nullptr);
		this->m_renderer->SetVertexBuffer(0, this->m_screenQuadFinalVB, sizeof(DE::SScreenQuadElement), 0, 4);
		this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB(), sizeof(int));
		this->m_renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("diffuseRT"));
		this->m_renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("lightRT"));

		this->m_DSFinal.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_AMBIENT_COLOR), this->m_scene->GetAmbientColor());
		this->m_DSFinal.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SKYBOX_AMBIENT_COLOR), this->m_scene->GetSkyAmbientColor());

		this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);

		// ------------------------------
		// Debug
		// ------------------------------
		this->m_scene->SetActiveCamera(this->m_activeCamera);
		this->m_scene->RenderDebugInformation();
		

		// ------------------------------
		// GUI: Prepare the renderer and draw
		// ------------------------------
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_gui->Render();
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);

		// ------------------------------
		// Cleanup
		// ------------------------------
		this->m_renderer->SetVertexDeclaration(nullptr);
		this->m_renderer->SetShaderProgram(nullptr);
		this->m_renderer->SetTexture(0, nullptr);
	
		this->m_renderer->EndScene();
	}

	void ISampleApplication::SpotLightsRendering()
	{
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());

		for(int i = 0 ; i < this->m_scene->GetNumberOfActiveSpotLights() ; ++i)
		{
			if(this->m_scene->GetActiveSpotLightsBuffer()->shadowCaster[this->m_scene->GetActiveSpotLightsBuffer()->visibilityOrder[i]])
			{
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
				this->m_renderer->SetViewport(DE::CRectangleI(0,0, this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				this->m_scene->SetActiveCamera(this->m_scene->GetActiveSpotLightsBuffer()->node[this->m_scene->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
				this->m_scene->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION);
				this->m_DSFB.DetachAllElements();
				this->m_DSFB.AttachElement("lightDepthRT", 0);
				this->m_DSFB.AttachDepth("lightDepth");
				this->m_renderer->SetVertexDeclaration(this->m_lightGeometryVD.get());
				this->m_renderer->SetShaderProgram(this->m_DSShadowMap.GetShaderProgram());
				this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
				this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
				this->m_scene->RenderAll();
			}

			// Add the light effect to the lightRT
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, true);
			this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

			this->m_DSFB.DetachAllElements();
			this->m_DSFB.AttachElement("lightRT", 0);
			this->m_DSFB.AttachDepth("depth");

			this->m_renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
			this->m_renderer->SetShaderProgram(this->m_DSSpotLight.GetShaderProgram());
			this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
			this->m_renderer->SetVertexBuffer(0, this->m_screenQuadLightVB, sizeof(SScreenQuadVertexLight), 0, 4);
			this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB(), sizeof(int));

			this->m_renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
			this->m_renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
			this->m_renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
			this->m_renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_DSSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_DSSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512));
			this->m_DSSpotLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), this->m_scene->GetActiveSpotLightsBuffer()->shadowCaster[this->m_scene->GetActiveSpotLightsBuffer()->visibilityOrder[i]]);
			this->m_scene->SendSpotLightDataToShader(this->m_scene->GetActiveSpotLightsBuffer()->visibilityOrder[i]);
			this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}

	void ISampleApplication::PointLightsRendering()
	{
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());

		// Add the light effect to the lightRT
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, true);
		this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

		this->m_renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
		this->m_renderer->SetShaderProgram(this->m_DSPointLight.GetShaderProgram());
		this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
		this->m_renderer->SetVertexBuffer(0, this->m_screenQuadLightVB, sizeof(SScreenQuadVertexLight), 0, 4);
		this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB(), sizeof(int));

		this->m_renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
		this->m_renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
		this->m_renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
		this->m_renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

		this->m_DSPointLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);

		for(int i = 0 ; i < this->m_scene->GetNumberOfActivePointLights() ; ++i)
		{
			this->m_scene->SendPointLightDataToShader(this->m_scene->GetActivePointLightsBuffer()->visibilityOrder[i]);
			this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}

	void ISampleApplication::DirectionalLightsRendering()
	{
		DE::CVector4F cameraPositionDistance(this->m_activeCamera->GetPosition(), this->m_activeCamera->GetFarDistance());

		for(int i = 0 ; i < this->m_scene->GetNumberOfActiveDirectionalLights() ; ++i)
		{
			if(this->m_scene->GetActiveDirectionalLightsBuffer()->shadowCaster[i])
			{
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, true);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, false);
				this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, false);
				this->m_renderer->SetViewport(DE::CRectangleI(0,0, this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512), this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512)));

				// Fill the shadow map
				this->m_scene->SetActiveCamera(this->m_scene->GetActiveDirectionalLightsBuffer()->node[i]);
				this->m_scene->SetMeshFormat(DE::MESHBUFFER_ELEMENT_POSITION);
				this->m_DSFB.DetachAllElements();
				this->m_DSFB.AttachElement("lightDepthRT", 0);
				this->m_DSFB.AttachDepth("lightDepth");
				this->m_renderer->SetVertexDeclaration(this->m_lightGeometryVD.get());
				this->m_renderer->SetShaderProgram(this->m_DSShadowMap.GetShaderProgram());
				this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
				this->m_renderer->Clear(DE::SURFACE_TYPE_TARGET | DE::SURFACE_TYPE_DEPTH, DE::CColor(0.0f, 0.0f, 0.0f, 0.0f));
				this->m_scene->RenderAll();
			}

			// Add the light effect to the lightRT
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_DEPTHTEST, false);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHABLEND, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_ALPHATEST, true);
			this->m_renderer->SetRenderState(DE::RENDER_STATE_TYPE_SCISSORTEST, true);
			this->m_renderer->SetViewport(DE::CRectangleI(0, 0, this->m_renderer->GetResolution().x, this->m_renderer->GetResolution().y));

			this->m_DSFB.DetachAllElements();
			this->m_DSFB.AttachElement("lightRT", 0);
			this->m_DSFB.AttachDepth("depth");

			this->m_renderer->SetVertexDeclaration(this->m_screenQuadLightVD.get());
			this->m_renderer->SetShaderProgram(this->m_DSDirectionalLight.GetShaderProgram());
			this->m_renderer->SetFrameBuffer(this->m_DSFB.GetFrameBuffer());
			this->m_renderer->SetVertexBuffer(0, this->m_screenQuadLightVB, sizeof(SScreenQuadVertexLight), 0, 4);
			this->m_renderer->SetIndexBuffer(this->m_renderer->GetQuadIB(), sizeof(int));

			this->m_renderer->SetTexture(0, this->m_DSFB.GetFrameBuffer()->GetTexture("normalRT"));
			this->m_renderer->SetTexture(1, this->m_DSFB.GetFrameBuffer()->GetTexture("specularRT"));
			this->m_renderer->SetTexture(2, this->m_DSFB.GetFrameBuffer()->GetTexture("depthRT"));
			this->m_renderer->SetTexture(3, this->m_DSFB.GetFrameBuffer()->GetTexture("lightDepthRT"));

			this->m_DSDirectionalLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_CAMERA_POSITION_DISTANCE), cameraPositionDistance);
			this->m_DSDirectionalLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_MAP_SIZE), this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_SHADOW_MAP_SIZE, 512));
			this->m_DSDirectionalLight.SetParameter(DE::SHADERPROGRAM_FLAG_PIXEL, this->m_renderer->GetShaderProgramParameterString(DE::SHADERPROGRAM_PARAMETER_SHADOW_CASTER), this->m_scene->GetActiveDirectionalLightsBuffer()->shadowCaster[i]);
			this->m_scene->SendDirectionalLightDataToShader(i);
			this->m_renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}
	}
}
