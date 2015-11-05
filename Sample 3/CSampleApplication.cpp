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

#include "CSampleApplication.h"

namespace SAMPLE3 {
	CSampleApplication::CSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : SAMPLES::IForwardSampleApplication(hInstance, nCmdShow) {
		this->m_scene3D->LoadScene("sample3.des");
		this->m_camera = std::unique_ptr<SAMPLES::CFPSCamera>(new SAMPLES::CFPSCamera((DE::ICameraSceneNode*)this->m_scene3D->GetNode("player")));
		this->m_camera->SetScene(this->m_scene3D);
		this->m_scene3D->SetActiveCamera(this->m_camera->GetCameraNode());
		this->m_scene3D->GetRootNode()->SetMeshShadingType(DE::MESH_SHADING_FLAT);
		this->m_activeCamera = this->m_camera->GetCameraNode();

		// Load the flashlight
		this->m_flashlight = std::unique_ptr<CFlashlightEntity>(new CFlashlightEntity(this->m_scene3D->GetNode("flashlight_entity"), this->m_scene3D->GetNode("flashlight_spot"), this->m_camera.get()));

		// Load the health bonus
		std::vector<DE::ISceneNode*> health = this->m_scene3D->GetNodeArray("health_entity");

		for(auto it = health.begin(); it != health.end(); it++) {
			CHealthEntity * entity = new CHealthEntity((*it), this->m_camera.get());
			this->m_health.push_back(std::unique_ptr<CHealthEntity>(entity));
		}

		MainLoop();
	}

	CSampleApplication::~CSampleApplication() {}

	void CSampleApplication::Update(const float time) {
		if(this->m_input->IsActionActive(SAMPLES::FPS_CAMERA_EXIT))
			this->m_alive = false;

		this->m_fpsCounter.Update(time);

		this->m_camera->Update(time);

		this->m_scene3D->UpdateAll(time);

		// Update the entities
		if(this->m_flashlight != nullptr) {
			this->m_flashlight->Update(time);
		}

		for(auto it = this->m_health.begin(); it != this->m_health.end(); it++) {
			(*it)->Update(time);
		}
	}
}
