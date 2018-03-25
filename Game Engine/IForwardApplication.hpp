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

#ifndef __GAMEENGINE_IFORWARDAPPLICATION_HPP
#define __GAMEENGINE_IFORWARDAPPLICATION_HPP

#include <thread>
#include <Scene/3D/C3DScene.hpp>
#include <Scene/2D/C2DScene.hpp>
#include <Scene/2D/CLabel.hpp>
#include <Scene/Particles/CParticleSystem.hpp>
#include <Medias/CShaderProgram.hpp>
#include <Medias/CFrameBuffer.hpp>
#include <Core/Utility/CFPSCounter.hpp>
#include <Core/IRenderer.hpp>
#include <Core/IDeclaration.hpp>
#include <Applications/IWin32Application.hpp>
#include "2D/CLoadingView.hpp"
#include "3D/IEntity.hpp"
#include "IPlayer.hpp"
#include "Level/ILevel.hpp"

namespace daidalosgameengine {
	/**
	* Forward rendering application
	*/
	class IForwardApplication : public daidalosengine::IWin32Application, public daidalosgameengine::ILevelListener {
	private:
		bool m_loading;
		bool m_cullEnabled;
		std::unique_ptr<ILevel> m_currentLevel;
		daidalosengine::C2DScene * m_loadingScene;
		daidalosengine::CCursor m_cursor;

		// 2D Rendering 
		daidalosengine::DeclarationPtr m_declaration2D;
		daidalosengine::CShaderProgram m_shader2D;

		// Forward Rendering
		daidalosengine::DeclarationPtr m_declarationDiffuse;
		daidalosengine::DeclarationPtr m_declarationShadowMap;
		daidalosengine::DeclarationPtr m_declarationLight;
		daidalosengine::CShaderProgram m_shaderDiffuse;
		daidalosengine::CShaderProgram m_shaderTransparent;
		daidalosengine::CShaderProgram m_shaderShadowMap;
		daidalosengine::CShaderProgram m_shaderSpotLight;
		daidalosengine::CShaderProgram m_shaderPointLight;
		daidalosengine::CShaderProgram m_shaderDirectionalLight;
		daidalosengine::CFrameBuffer m_frameBufferShadowMap;

		// Post process
		daidalosengine::CFrameBuffer m_frameBufferPostProcess;
		daidalosengine::DeclarationPtr m_screenQuadPostProcessVD;
		daidalosengine::CBuffer m_screenQuadPostProcessVB;
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
		IForwardApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		virtual ~IForwardApplication();

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
		* Render the spot lights in the backbuffer
		* @param scene3D The scene
		*/
		virtual void SpotLightsRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the point lights in the backbuffer
		* @param scene3D The scene
		*/
		virtual void PointLightsRendering(daidalosengine::C3DScene * scene3D);

		/**
		* Render the directional lights in the backbuffer
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
		* Check the laoding state
		* @return true if the level is loading
		*/
		const bool IsLoading() const;

		/**
		* Check if the cull is enabled
		* @return true if the cull is enabled
		*/
		const bool IsCullEnabled() const;

		/**
		* Get the current level
		* @return the current level
		*/
		daidalosgameengine::ILevel * GetCurrentLevel() const;

		/**
		* Get the loading scene
		* @return The loading scene
		*/
		daidalosengine::C2DScene * GetLoadingScene() const;

		/**
		* Get the cursor
		* @return The cursor
		*/
		daidalosengine::CCursor & GetCursor();

		/**
		* Set the loading state
		* @param loading The loading state
		*/
		void SetLoading(const bool loading);

		/**
		* Set the cull
		* @param cullEnabled The cull
		*/
		void SetCullEnabled(const bool cullEnabled);

		/**
		* Set the current level
		* @param level The level
		*/
		void SetLevel(daidalosgameengine::ILevel * level);
	};
}

#endif
