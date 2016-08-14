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

#include "CLevelEditorApplication.h"

namespace EDITOR {
	CLevelEditorApplication::CLevelEditorApplication(const HINSTANCE hInstance, const int nCmdShow) : IForwardSampleApplication(hInstance, nCmdShow) {

		this->m_drawSkybox = true;
		this->m_drawLights = true;
		this->m_cameraPosition = DE::CVector3F(0, 4, 0);
		std::thread(&CLevelEditorApplication::LoadLevel, this, "sample1.des").detach();

		MainLoop();
	}

	CLevelEditorApplication::~CLevelEditorApplication() {}

	void CLevelEditorApplication::Update(const float time) {

		if(!this->m_loading) {

			if(this->m_input->IsActionActive(VIEW_CAMERA_EXIT)) {
				this->m_alive = false;
			}

			if(this->m_input->IsActionActive(VIEW_CAMERA_CURSOR)) {
				this->m_cursor.SetVisible(!this->m_cursor.IsVisible());
				this->m_input->FreezeAction(VIEW_CAMERA_CURSOR);
			}

			if(this->m_input->IsActionActive(VIEW_CAMERA_SKYBOX)) {
				this->m_drawSkybox = !this->m_drawSkybox;
				this->m_input->FreezeAction(VIEW_CAMERA_SKYBOX);
			}

			if(this->m_input->IsActionActive(VIEW_CAMERA_LIGHTS)) {
				this->m_drawLights = !this->m_drawLights;
				this->m_input->FreezeAction(VIEW_CAMERA_LIGHTS);
			}

			// Update the 3D scene
			if(!this->m_cursor.IsVisible()) {
				this->m_camera->Update(time);
			}
			
			this->m_scene3D->SetActiveCamera(this->m_activeCamera);
			this->m_scene3D->UpdateAll(time);

			this->m_cursor.Update(time);
			this->m_scene2D->Update(time);

			this->m_cameraPosition = DE::CVector3F(this->m_activeCamera->GetPosition());
			this->m_cameraRotation = DE::CVector3F(this->m_activeCamera->GetRotation());

			if(this->m_input->IsActionActive(VIEW_CAMERA_REFRESH)) {
				std::thread(&CLevelEditorApplication::LoadLevel, this, "sample1.des").detach();
			}

		} else {
			this->m_loadingScene->Update(time);
		}
	}

	/**
	* Render a 3D scene
	*/
	void CLevelEditorApplication::Render3DScene(DE::C3DScene * scene3D) {

		// ------------------------------
		// Initialization
		// ------------------------------
		InitializeRendering(scene3D);

		// ------------------------------
		// Sky box mask + rendering
		// ------------------------------
		if(this->m_drawSkybox) {
			SkyboxRendering(scene3D);
		}

		// ------------------------------
		// Diffuse
		// ------------------------------
		DiffuseRendering(scene3D);

		// Lights
		if(this->m_drawLights) {
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
		}

		// ------------------------------
		// Particle System
		// ------------------------------
		scene3D->GetParticleSystem().SetActiveCamera(this->m_activeCamera);
		scene3D->GetParticleSystem().RenderAll();
	}

	/**
	* Do post level loading initialization
	*/
	void CLevelEditorApplication::PostLoadLevel() {
		this->m_camera = std::unique_ptr<CViewCamera>(new CViewCamera(this->m_activeCamera));
		this->m_camera->SetScene(this->m_scene3D);
		this->m_camera->SetPosition(this->m_cameraPosition);
		this->m_camera->SetRotation(this->m_cameraRotation);
	}
}
