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
#include <Scene/3D/CDirectionalLightSceneNode.h>

namespace SAMPLE1 {
	CSampleApplication::CSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : ISampleApplication(hInstance, nCmdShow) {
		this->m_scene->LoadScene("sample1.des");
		this->m_camera = std::unique_ptr<SAMPLES::CFreeCamera>(new SAMPLES::CFreeCamera((DE::ICameraSceneNode*)this->m_scene->GetNode("player")));
		this->m_camera->SetScene(this->m_scene);
		this->m_directionalLight = (DE::CDirectionalLightSceneNode*)this->m_scene->GetNode("directionalLight");
		this->m_dwarfNode = (DE::CAnimatedMeshSceneNode*)this->m_scene->GetNode("dwarf");
		this->m_scene->SetActiveCamera(this->m_camera->GetCameraNode());
		this->m_activeCamera = this->m_camera->GetCameraNode();
		this->m_scene->GetRootNode()->SetMeshShadingType(DE::MESH_SHADING_FLAT);

		MainLoop();
	}

	CSampleApplication::~CSampleApplication() {}

	void CSampleApplication::Update(const float time) {
		this->m_fpsCounter.Update(time);

		if(this->m_input->IsActionActive(SAMPLES::FPS_CAMERA_EXIT))
			this->m_alive = false;

		this->m_scene->SetActiveCamera(this->m_activeCamera);
		this->m_camera->Update(time);

		if(this->m_directionalLight != nullptr) {
			DE::CVector3F lightRotation = this->m_directionalLight->GetRotation();
			lightRotation.y += (40 * time) / 1000.0f;
			this->m_directionalLight->SetRotation(lightRotation);
		}

		if(this->m_dwarfNode->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED) {
			this->m_dwarfNode->StartAnimation(0.0f, (float)this->m_dwarfNode->GetNumberOfFrames(), 20.0f);
		}

		this->m_scene->UpdateAll(time);
	}
}
