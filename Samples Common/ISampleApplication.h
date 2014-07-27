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

#ifndef __ISAMPLEAPPLICATION_H
#define __ISAMPLEAPPLICATION_H

#include <Scene/CSceneManager.h>
#include <Scene/CStaticMeshSceneNode.h>
#include <Scene/CAnimatedMeshSceneNode.h>
#include <Scene/CSpotLightSceneNode.h>
#include <Scene/CPointLightSceneNode.h>
#include <Scene/CDirectionalLightSceneNode.h>
#include <Medias/CBuffer.h>
#include <Medias/CShaderProgram.h>
#include <Medias/CTexture2D.h>
#include <Medias/CFrameBuffer.h>
#include <GUI/CGUILabel.h>
#include <GUI/CGUIButton.h>
#include <Core/Utility/CColor.h>
#include <Core/Utility/CFPSCounter.h>
#include <Core/IRenderer.h>
#include <Core/IDeclaration.h>
#include <Applications/IWin32Application.h>

namespace SAMPLES
{
	/**
	* Test application implementation
	*/
	class ISampleApplication : public DE::IWin32Application 
	{
	protected:
		DE::CSceneManager * m_scene;
		DE::DeclarationPtr m_declaration;
		DE::ICameraSceneNode * m_activeCamera;

		// Deferred Shading
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

		DE::CGUILabel m_fpsText;
		DE::CGUILabel m_positionText;
		DE::CGUIButton m_button;


		DE::CFPSCounter m_fpsCounter;

	public:
		/**
		* Constructor
		* @param hInstance Instance
		* @param nCmdShow Commands
		*/
		ISampleApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		virtual ~ISampleApplication();

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
		* Render the spot lights in the light RT
		*/
		void SpotLightsRendering();

		/**
		* Render the point lights in the light RT
		*/
		void PointLightsRendering();

		/**
		* Render the directional lights in the light RT
		*/
		void DirectionalLightsRendering();
	};
}

#endif
