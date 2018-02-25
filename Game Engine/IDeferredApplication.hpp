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

#ifndef __GAMEENGINE_IDEFERREDAPPLICATION_HPP
#define __GAMEENGINE_IDEFERREDAPPLICATION_HPP

#include <Scene/3D/C3DScene.hpp>
#include <Scene/3D/CStaticMeshSceneNode.hpp>
#include <Scene/3D/CAnimatedMeshSceneNode.hpp>
#include <Scene/3D/CSpotLightSceneNode.hpp>
#include <Scene/3D/CPointLightSceneNode.hpp>
#include <Scene/3D/CDirectionalLightSceneNode.hpp>
#include <Scene/2D/C2DScene.hpp>
#include <Medias/CBuffer.hpp>
#include <Medias/CShaderProgram.hpp>
#include <Medias/CTexture2D.hpp>
#include <Medias/CFrameBuffer.hpp>
#include <Scene/2D/CLabel.hpp>
#include <Scene/2D/CButton.hpp>
#include <Scene/2D/CCheckbox.hpp>
#include <Core/Utility/CColor.hpp>
#include <Core/Utility/CFPSCounter.hpp>
#include <Core/IRenderer.hpp>
#include <Core/IDeclaration.hpp>
#include <Applications/IWin32Application.hpp>
#include "2D/CLoadingView.hpp"
#include "3D/IEntity.hpp"
#include "IPlayer.hpp"
#include "Level/ILevel.hpp"

namespace daidalosgameengine
{
	/**
	* Deferred rendering application
	*/
	class IDeferredApplication : public daidalosengine::IWin32Application, public daidalosgameengine::ILevelListener {
	protected:
		bool m_loading;
		std::unique_ptr<ILevel> m_currentLevel;
		daidalosengine::C2DScene * m_loadingScene;
		daidalosengine::CCursor m_cursor;

		// 2D Rendering 
		daidalosengine::DeclarationPtr m_declaration2D;
		daidalosengine::CShaderProgram m_shader2D;

		// Deferred Shading
		daidalosengine::DeclarationPtr m_declaration;
		daidalosengine::DeclarationPtr m_declarationDiffuse;
		daidalosengine::CFrameBuffer m_DSFB;
		daidalosengine::CBuffer m_screenQuadLightVB;
		daidalosengine::CBuffer m_screenQuadFinalVB;
		daidalosengine::DeclarationPtr m_lightGeometryVD;
		daidalosengine::DeclarationPtr m_screenQuadLightVD;
		daidalosengine::DeclarationPtr m_screenQuadLightVD2;
		daidalosengine::DeclarationPtr m_screenQuadFinalVD;
		daidalosengine::CShaderProgram m_DSFill;
		daidalosengine::CShaderProgram m_DSShadowMap;
		daidalosengine::CShaderProgram m_DSSpotLight;
		daidalosengine::CShaderProgram m_DSPointLight;
		daidalosengine::CShaderProgram m_DSDirectionalLight;
		daidalosengine::CShaderProgram m_DSFinal;
		daidalosengine::CShaderProgram m_DSPostProcessFinal;
		daidalosengine::CShaderProgram m_shaderTransparent;
		daidalosengine::CShaderProgram m_shaderDiffuse;

		// Post process
		daidalosengine::CTexture2D m_texture2DPostProcess;
		daidalosengine::CShaderProgram m_shaderScreenBlur;
		daidalosengine::CShaderProgram m_shaderScreenGlow;

		// Misc
		daidalosengine::CTexture2D m_texture2DWhite;
		daidalosengine::CTexture2D m_texture2DBlack;
		daidalosengine::CShaderProgram m_shaderScreen;

		// Hud
		daidalosengine::CFPSCounter m_fpsCounter;
		daidalosengine::CLabel m_fpsLabel;
		CLoadingView * m_loadingView;

	public:
		/**
		* Constructor
		* @param hInstance Instance
		* @param nCmdShow Commands
		*/
		IDeferredApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		virtual ~IDeferredApplication();

		/**
		* Update the application
		* @param time Elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Render the application
		*/
		virtual void Render();

	protected:
		/**
		* Load a level and show a loading screen
		* Should be called from a new thread
		* @param filename The level filename
		*/
		virtual void LoadLevel(const std::string & filename) = 0;

		/**
		* Render a 3D scene
		* @param scene3D The scene
		*/
		virtual void Render3DScene(daidalosengine::C3DScene * scene3D);

		/**
		* Render a 2D scene
		* @param scene2D The scene
		*/
		virtual void Render2DScene(daidalosengine::C2DScene * scene2D);

		/**
		* Do initialization before rendering anything
		* @param scene3D The scene
		*/
		virtual void InitializeRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the skybox
		* @param scene3D The scene
		*/
		virtual void SkyboxRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the diffuse part
		* @param scene3D The scene
		*/
		virtual void DiffuseRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the diffuse top layer part
		* @param scene3D The scene
		*/
		virtual void DiffuseTopLayerRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the transparent part
		* @param scene3D The scene
		*/
		virtual void TransparentRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the spot lights in the RT
		* @param scene3D The scene
		*/
		virtual void SpotLightsRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the point lights in the RT
		* @param scene3D The scene
		*/
		virtual void PointLightsRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the directional lights in the RT
		* @param scene3D The scene
		*/
		virtual void DirectionalLightsRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Do the post process rendering
		*/
		virtual void PostProcessRendering();

		/**
		* Apply the brightness
		*/
		virtual void ApplyBrightness();

		/**
		* Callback when the level is fully loaded
		*/
		virtual void OnLevelLoaded() = 0;

		/**
		* Callback when the next level is triggered
		*/
		virtual void OnTriggerNextLevel(const std::string & nextLevel) = 0;

		/**
		* Prepare for light rendering
		* @param scene3D The scene
		*/
		void PreLightRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Post light rendering
		* @param scene3D The scene
		*/
		void PostLightRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the particle system
		* @param scene3D The scene
		*/
		void ParticleSystemRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Finalize the deferred rendering by drawing the RTs the the backbuffer
		* @param scene3D The scene
		*/
		void FinalizeDeferredRendering(daidalosengine::C3DScene * scene3D);
	};
}

#endif
