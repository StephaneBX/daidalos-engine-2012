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

#ifndef __GAMEENGINE_IFORWARDSAMPLEAPPLICATION_HPP
#define __GAMEENGINE_IFORWARDSAMPLEAPPLICATION_HPP

#include <thread>
#include <Scene/3D/C3DScene.h>
#include <Scene/2D/C2DScene.h>
#include <Scene/2D/CLabel.h>
#include <Scene/Particles/CParticleSystem.h>
#include <Medias/CShaderProgram.h>
#include <Medias/CFrameBuffer.h>
#include <Core/Utility/CFPSCounter.h>
#include <Core/IRenderer.h>
#include <Core/IDeclaration.h>
#include <Applications/IWin32Application.h>
#include "2D/CLoadingView.hpp"
#include "3D/IEntity.hpp"
#include "IPlayer.hpp"
#include "Level/ILevel.hpp"

namespace GE {
	/**
	* Test application implementation with forward rendering
	*/
	class IForwardSampleApplication : public DE::IWin32Application, public GE::ILevelListener {
	protected:
		bool m_loading;
		std::unique_ptr<ILevel> m_currentLevel;
		DE::C2DScene * m_loadingScene;
		DE::CCursor m_cursor;

		// 2D Rendering 
		DE::DeclarationPtr m_declaration2D;
		DE::CShaderProgram m_shader2D;

		// Forward Rendering
		DE::DeclarationPtr m_declarationDiffuse;
		DE::DeclarationPtr m_declarationShadowMap;
		DE::DeclarationPtr m_declarationLight;
		DE::CShaderProgram m_shaderDiffuse;
		DE::CShaderProgram m_shaderTransparent;
		DE::CShaderProgram m_shaderShadowMap;
		DE::CShaderProgram m_shaderSpotLight;
		DE::CShaderProgram m_shaderPointLight;
		DE::CShaderProgram m_shaderDirectionalLight;
		DE::CFrameBuffer m_frameBufferShadowMap;

		// Post process
		DE::CFrameBuffer m_frameBufferPostProcess;
		DE::DeclarationPtr m_screenQuadPostProcessVD;
		DE::CBuffer m_screenQuadPostProcessVB;
		DE::CTexture2D m_texture2DPostProcess;
		DE::CShaderProgram m_shaderScreenBlur;
		DE::CShaderProgram m_shaderScreenGlow;

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
		IForwardSampleApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		virtual ~IForwardSampleApplication();

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
		* Render the spot lights in the backbuffer
		* @param scene3D The scene
		*/
		virtual void SpotLightsRendering(DE::C3DScene * scene3D);

		/**
		* Render the point lights in the backbuffer
		* @param scene3D The scene
		*/
		virtual void PointLightsRendering(DE::C3DScene * scene3D);

		/**
		* Render the directional lights in the backbuffer
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
	};
}

#endif
