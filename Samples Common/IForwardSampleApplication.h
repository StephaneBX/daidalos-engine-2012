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

#ifndef __IFORWARDSAMPLEAPPLICATION_H
#define __IFORWARDSAMPLEAPPLICATION_H

#include <Scene/3D/C3DScene.h>
#include <Scene/2D/C2DScene.h>
#include <Scene/2D/CLabel.h>
#include <Medias/CShaderProgram.h>
#include <Medias/CFrameBuffer.h>
#include <Core/Utility/CFPSCounter.h>
#include <Core/IRenderer.h>
#include <Core/IDeclaration.h>
#include <Applications/IWin32Application.h>

namespace SAMPLES {
	/**
	* Test application implementation
	*/
	class IForwardSampleApplication : public DE::IWin32Application {
	protected:
		DE::C3DScene * m_scene3D;
		DE::C2DScene * m_scene2D;
		DE::ICameraSceneNode * m_activeCamera;
		DE::CCursor m_cursor;

		// 2D Rendering 
		DE::DeclarationPtr m_declaration2D;
		DE::CShaderProgram m_shader2D;

		// Forward Rendering
		DE::DeclarationPtr m_declarationDiffuse;
		DE::DeclarationPtr m_declarationShadowMap;
		DE::DeclarationPtr m_declarationLight;
		DE::CShaderProgram m_shaderDiffuse;
		DE::CShaderProgram m_shaderShadowMap;
		DE::CShaderProgram m_shaderSpotLight;
		DE::CShaderProgram m_shaderPointLight;
		DE::CShaderProgram m_shaderDirectionalLight;
		DE::CFrameBuffer m_frameBufferShadowMap;

		// Debug
		DE::CLabel m_fpsText;
		DE::CFPSCounter m_fpsCounter;
		DE::CLabel m_positionText;

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
		virtual void Update(const float time) = 0;

		/**
		* Render the application
		*/
		void Render();

	protected:
		/**
		* Render the spot lights in the backbuffer
		*/
		void SpotLightsRendering();

		/**
		* Render the point lights in the backbuffer
		*/
		void PointLightsRendering();

		/**
		* Render the directional lights in the backbuffer
		*/
		void DirectionalLightsRendering();
	};
}

#endif
