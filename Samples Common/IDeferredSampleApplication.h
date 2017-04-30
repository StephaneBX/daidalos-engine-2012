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

#ifndef __IDEFERREDSAMPLEAPPLICATION_H
#define __IDEFERREDSAMPLEAPPLICATION_H

#include <Scene/3D/C3DScene.h>
#include <Scene/3D/CStaticMeshSceneNode.h>
#include <Scene/3D/CAnimatedMeshSceneNode.h>
#include <Scene/3D/CSpotLightSceneNode.h>
#include <Scene/3D/CPointLightSceneNode.h>
#include <Scene/3D/CDirectionalLightSceneNode.h>
#include <Scene/2D/C2DScene.h>
#include <Medias/CBuffer.h>
#include <Medias/CShaderProgram.h>
#include <Medias/CTexture2D.h>
#include <Medias/CFrameBuffer.h>
#include <Scene/2D/CLabel.h>
#include <Scene/2D/CButton.h>
#include <Scene/2D/CCheckbox.h>
#include <Core/Utility/CColor.h>
#include <Core/Utility/CFPSCounter.h>
#include <Core/IRenderer.h>
#include <Core/IDeclaration.h>
#include <Applications/IWin32Application.h>
#include "2D/CHudView.h"
#include "2D/CLoadingView.h"
#include "3D/IEntity.h"
#include "IPlayer.h"
#include "Level/ILevel.h"

namespace SAMPLES
{
	/**
	* Test application implementation with deferred rendering
	*/
	class IDeferredSampleApplication : public DE::IWin32Application, public SAMPLES::ILevelListener {
	protected:
		bool m_loading;
		std::unique_ptr<ILevel> m_currentLevel;
		DE::C2DScene * m_loadingScene;
		DE::CCursor m_cursor;

		// 2D Rendering 
		DE::DeclarationPtr m_declaration2D;
		DE::CShaderProgram m_shader2D;

		// Deferred Shading
		DE::DeclarationPtr m_declaration;
		DE::DeclarationPtr m_declarationDiffuse;
		DE::CFrameBuffer m_DSFB;
		DE::CBuffer m_screenQuadLightVB;
		DE::CBuffer m_screenQuadFinalVB;
		DE::DeclarationPtr m_lightGeometryVD;
		DE::DeclarationPtr m_screenQuadLightVD;
		DE::DeclarationPtr m_screenQuadLightVD2;
		DE::DeclarationPtr m_screenQuadFinalVD;
		DE::CShaderProgram m_DSFill;
		DE::CShaderProgram m_DSShadowMap;
		DE::CShaderProgram m_DSSpotLight;
		DE::CShaderProgram m_DSPointLight;
		DE::CShaderProgram m_DSDirectionalLight;
		DE::CShaderProgram m_DSFinal;
		DE::CShaderProgram m_DSPostProcessFinal;
		DE::CShaderProgram m_shaderTransparent;
		DE::CShaderProgram m_shaderDiffuse;

		// Hud
		DE::CFPSCounter m_fpsCounter;
		DE::CLabel m_fpsLabel;
		CLoadingView * m_loadingView;

	public:
		/**
		* Constructor
		* @param hInstance Instance
		* @param nCmdShow Commands
		*/
		IDeferredSampleApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		virtual ~IDeferredSampleApplication();

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
		virtual void Render3DScene(DE::C3DScene * scene3D);

		/**
		* Render a 2D scene
		* @param scene2D The scene
		*/
		virtual void Render2DScene(DE::C2DScene * scene2D);

		/**
		* Do initialization before rendering anything
		* @param scene3D The scene
		*/
		virtual void InitializeRendering(DE::C3DScene * scene3D);

		/**
		* Render the skybox
		* @param scene3D The scene
		*/
		virtual void SkyboxRendering(DE::C3DScene * scene3D);

		/**
		* Render the diffuse part
		* @param scene3D The scene
		*/
		virtual void DiffuseRendering(DE::C3DScene * scene3D);

		/**
		* Render the diffuse top layer part
		* @param scene3D The scene
		*/
		virtual void DiffuseTopLayerRendering(DE::C3DScene * scene3D);

		/**
		* Render the transparent part
		* @param scene3D The scene
		*/
		virtual void TransparentRendering(DE::C3DScene * scene3D);

		/**
		* Render the spot lights in the RT
		* @param scene3D The scene
		*/
		virtual void SpotLightsRendering(DE::C3DScene * scene3D);

		/**
		* Render the point lights in the RT
		* @param scene3D The scene
		*/
		virtual void PointLightsRendering(DE::C3DScene * scene3D);

		/**
		* Render the directional lights in the RT
		* @param scene3D The scene
		*/
		virtual void DirectionalLightsRendering(DE::C3DScene * scene3D);

		/**
		* Do the post process rendering
		*/
		virtual void PostProcessRendering();

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
		void PreLightRendering(DE::C3DScene * scene3D);

		/**
		* Post light rendering
		* @param scene3D The scene
		*/
		void PostLightRendering(DE::C3DScene * scene3D);

		/**
		* Render the particle system
		* @param scene3D The scene
		*/
		void ParticleSystemRendering(DE::C3DScene * scene3D);

		/**
		* Finalize the deferred rendering by drawing the RTs the the backbuffer
		* @param scene3D The scene
		*/
		void FinalizeDeferredRendering(DE::C3DScene * scene3D);
	};
}

#endif
