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

#include <sstream>
#include "Scene\Particles\Initializers\CColorParticleInitializer.h"
#include "Scene\Particles\Initializers\CLifeParticleInitializer.h"
#include "Scene\Particles\Initializers\CRotationParticleInitializer.h"
#include "Scene\Particles\Initializers\CSizeParticleInitializer.h"
#include "Scene\Particles\Initializers\CLinearVelocityParticleInitializer.h"
#include "Scene\Particles\Initializers\CSphereVelocityParticleInitializer.h"
#include "Scene\Particles\Updaters\CColorParticleUpdater.h"
#include "Scene\Particles\Updaters\CGravityParticleUpdater.h"
#include "Scene\Particles\Updaters\CLifeParticleUpdater.h"
#include "Scene\Particles\Updaters\CRotationParticleUpdater.h"
#include "Scene\Particles\Updaters\CSizeParticleUpdater.h"
#include "Scene\Particles\Updaters\CVelocityParticleUpdater.h"
#include "CSampleApplication.h"

namespace SAMPLE2 {
	CSampleApplication::CSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : SAMPLES::IForwardSampleApplication(hInstance, nCmdShow) {
		this->m_scene3D->LoadScene("sample2.des");
		this->m_camera = std::unique_ptr<SAMPLES::CFPSCamera>(new SAMPLES::CFPSCamera((DE::ICameraSceneNode*)this->m_scene3D->GetNode("player")));
		this->m_camera->SetScene(this->m_scene3D);
		this->m_spotLight = (DE::CSpotLightSceneNode*)this->m_scene3D->GetNode("spotLight1");
		this->m_scene3D->SetActiveCamera(this->m_camera->GetCameraNode());
		this->m_activeCamera = this->m_camera->GetCameraNode();

		this->m_scene3D->GetRootNode()->SetMeshShadingType(DE::MESH_SHADING_FLAT);

		// Load the doors
		std::vector<DE::ISceneNode*> doorNode = this->m_scene3D->GetNodeArray("door_entity");
		for(auto it = doorNode.begin(); it != doorNode.end(); it++) {
			SAMPLES::CDoorEntity * doorEntity = new SAMPLES::CDoorEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_camera.get(), this->m_scene3D);
			this->m_doors.push_back(std::unique_ptr<SAMPLES::CDoorEntity>(doorEntity));
		}

		// Load the fans
		std::vector<DE::ISceneNode*> fanNode = this->m_scene3D->GetNodeArray("fan_entity");
		for(auto it = fanNode.begin(); it != fanNode.end(); it++) {
			SAMPLES::CFanEntity * fanEntity = new SAMPLES::CFanEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_camera.get());
			fanEntity->DoAction();
			this->m_fans.push_back(std::unique_ptr<SAMPLES::CFanEntity>(fanEntity));
		}

		// Load the switchs
		std::vector<DE::ISceneNode*> switchNode = this->m_scene3D->GetNodeArray("switch_entity");
		for(auto it = switchNode.begin(); it != switchNode.end(); it++) {
			SAMPLES::CSwitchEntity * switchEntity = new SAMPLES::CSwitchEntity((DE::CAnimatedMeshSceneNode*)(*it), this->m_camera.get(), this->m_scene3D);
			this->m_switchs.push_back(std::unique_ptr<SAMPLES::CSwitchEntity>(switchEntity));
		}
		this->m_switchs.at(0)->AddTarget(this->m_fans.at(0).get());

		// Smoke test
		DE::CParticleEmitter * emitter = this->m_particleSystem.InstanciateEmitter();
		emitter->SetLife(0);
		emitter->SetLifeMax(99999999.0f);
		emitter->SetPosition(DE::CVector3F(0, 0.5f, 20));
		emitter->SetTextureId(0);
		emitter->SetEmissionRate(60);

		emitter->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.5f, 0.5f, 0.5f, 0.3f), DE::CColor(0.5f, 0.5f, 0.5f, 0.0f)));
		emitter->AddInitializer(new DE::CLifeParticleInitializer(0,10000));
		emitter->AddInitializer(new DE::CRotationParticleInitializer(-45,45));
		emitter->AddInitializer(new DE::CSizeParticleInitializer(1,6));
		emitter->AddInitializer(new DE::CSphereVelocityParticleInitializer(0.5f,1));

		emitter->AddUpdater(new DE::CColorParticleUpdater());
		emitter->AddUpdater(new DE::CGravityParticleUpdater(DE::CVector3F(0,0.5f,0)));
		emitter->AddUpdater(new DE::CLifeParticleUpdater());
		emitter->AddUpdater(new DE::CRotationParticleUpdater());
		emitter->AddUpdater(new DE::CSizeParticleUpdater());
		emitter->AddUpdater(new DE::CVelocityParticleUpdater(0.05f));

		// 
		DE::CParticleEmitter * emitter2 = this->m_particleSystem.InstanciateEmitter();
		emitter2->SetLife(0);
		emitter2->SetLifeMax(99999999.0f);
		emitter2->SetPosition(DE::CVector3F(0, 0.1f, 30));
		emitter2->SetTextureId(1);
		emitter2->SetEmissionRate(200);

		emitter2->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.7f, 0.25f, 0.25f, 0.5f), DE::CColor(0.7f, 0.25f, 0.25f, 0.0f)));
		emitter2->AddInitializer(new DE::CLifeParticleInitializer(500, 2000));
		emitter2->AddInitializer(new DE::CSizeParticleInitializer(0.2f, 0.5f));
		emitter2->AddInitializer(new DE::CLinearVelocityParticleInitializer(DE::CVector3F(0.3f, 1.0f, 0.0f), 0.2f, 12.0f, 15.0f));

		emitter2->AddUpdater(new DE::CColorParticleUpdater());
		emitter2->AddUpdater(new DE::CGravityParticleUpdater(DE::CVector3F(0, -5.0f, 0)));
		emitter2->AddUpdater(new DE::CLifeParticleUpdater());
		emitter2->AddUpdater(new DE::CSizeParticleUpdater());
		emitter2->AddUpdater(new DE::CVelocityParticleUpdater(0.5f));

		MainLoop();
	}

	CSampleApplication::~CSampleApplication() {}

	void CSampleApplication::Update(const float time) {
		this->m_fpsCounter.Update(time);

		if(this->m_input->IsActionActive(SAMPLES::FPS_CAMERA_EXIT) || this->m_input->IsGamepadActionActive(0, SAMPLES::FPS_CAMERA_EXIT)) {
			this->m_alive = false;
		}

		this->m_camera->Update(time);

		if(this->m_spotLight) {
			DE::CVector3F lightRotation = this->m_spotLight->GetRotation();
			lightRotation.y += (40 * time) / 1000.0f;
			this->m_spotLight->SetRotation(lightRotation);
		}

		this->m_scene3D->SetActiveCamera(this->m_activeCamera);
		this->m_scene3D->UpdateAll(time);

		// Update the doors
		for(auto it = this->m_doors.begin(); it != this->m_doors.end(); it++) {
			(*it)->Update(time);
		}

		// Update the switch
		for(auto it = this->m_switchs.begin(); it != this->m_switchs.end(); it++) {
			(*it)->Update(time);
		}

		// Update the fans
		for(auto it = this->m_fans.begin(); it != this->m_fans.end(); it++) {
			(*it)->Update(time);
		}

		// Update the particle system
		this->m_particleSystem.SetActiveCamera(this->m_camera->GetCameraNode());
		this->m_particleSystem.Update(time);

		// Update the position
		std::stringstream ss;
		ss << "P: " << (int)this->m_camera->GetCameraNode()->GetAbsolutePosition().x << " : " << (int)this->m_camera->GetCameraNode()->GetAbsolutePosition().y << " : " << (int)this->m_camera->GetCameraNode()->GetAbsolutePosition().z << " - G:" << this->m_scene3D->GetNumberOfVisibleTriangles();
		this->m_positionText.SetText(ss.str());

		// Update the fps
		this->m_fpsCounter.Update(time);
		this->m_fpsText.SetText(this->m_fpsCounter.GetText());
	}
}
