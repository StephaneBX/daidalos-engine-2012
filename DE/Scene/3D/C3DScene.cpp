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

#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Core/CMediaManager.h"
#include "../../Core/Utility/CArraySorter.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/ILevelSceneNode.h"
#include "../../Scene/3D/C3DScene.h"

#define LIGHTNING_UPDATE_DELAY 20.0f

namespace DE {

	C3DScene::C3DScene() {
		LOG_TRACE();

		this->m_scene = nullptr;
	}

	C3DScene::~C3DScene() {
		LOG_TRACE();

		SAFE_DELETE(this->m_scene);
	}

	void C3DScene::LoadScene(const std::string & sceneFile) {
		LOG_TRACE();

		SAFE_DELETE(this->m_scene);
		this->m_scene = CMediaManager::GetInstance()->LoadMediaFromFile<I3DScene>(sceneFile);
	}

	void C3DScene::SaveScene(const std::string & destination) {
		LOG_TRACE();

		CMediaManager::GetInstance()->SaveMediaToFile(this->m_scene, destination);
	}

	void C3DScene::ClearAll() {
		LOG_TRACE();
	}

	void C3DScene::AddSceneNode(ISceneNode * parent, ISceneNode * node) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->AddSceneNode(parent, node);
	}

	void C3DScene::SetActiveCamera(ICameraSceneNode * camera) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SetActiveCamera(camera);
	}

	void C3DScene::UpdateAll(const float time) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->UpdateAll(time);
	}

	void C3DScene::RenderSkyboxMask() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->RenderSkyboxMask();
	}

	void C3DScene::RenderSkybox() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->RenderSkybox();
	}

	void C3DScene::RenderAll() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->RenderAll();
	}

	void C3DScene::RenderDebugInformation() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->RenderDebugInformation();
	}

	void C3DScene::SetMeshFormat(const int format) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SetMeshFormat(format);
	}

	void C3DScene::SendSpotLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SendSpotLightDataToShader(id);
	}

	void C3DScene::SendPointLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SendPointLightDataToShader(id);
	}

	void C3DScene::SendDirectionalLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SendDirectionalLightDataToShader(id);
	}

	void C3DScene::SetBrushCollision(const int brushId, const bool collision) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		this->m_scene->SetBrushCollision(brushId, collision);
	}

	const bool C3DScene::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->Slide(shape, movement);
	}

	const bool C3DScene::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetCollisionPoint(shape, movement, point);
	}

	const int C3DScene::GetNumberOfActiveSpotLights() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNumberOfActiveSpotLights();
	}

	const int C3DScene::GetNumberOfActivePointLights() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNumberOfActivePointLights();
	}

	const int C3DScene::GetNumberOfActiveDirectionalLights() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNumberOfActiveDirectionalLights();
	}

	const int C3DScene::GetNumberOfVisibleTriangles() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNumberOfVisibleTriangles();
	}

	const std::string & C3DScene::GetFilename() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetFilename();
	}

	const CVector4F & C3DScene::GetAmbientColor() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetAmbientColor();
	}

	const CVector4F & C3DScene::GetSkyAmbientColor() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetSkyAmbientColor();
	}

	const CAABoundingBoxF C3DScene::GetSceneBoundingBox() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetSceneBoundingBox();
	}

	CParticleSystem & C3DScene::GetParticleSystem() {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetParticleSystem();
	}

	ISceneNode * C3DScene::GetNode(const std::string & name) const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNode(name);
	}


	I3DScene * C3DScene::GetScene() const {
		LOG_TRACE();

		return this->m_scene;
	}

	std::vector<ISceneNode*> C3DScene::GetNodeArray(const std::string & name) const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNodeArray(name);
	}

	std::vector<ISceneNode*> C3DScene::GetNodeArray(ESceneNodeType nodeType) const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetNodeArray(nodeType);
	}

	ILevelSceneNode * C3DScene::GetRootNode() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetRootNode();
	}

	ICameraSceneNode * C3DScene::GetActiveCamera() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetActiveCamera();
	}

	SSPotLightsShaderData * C3DScene::GetActiveSpotLightsBuffer() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetActiveSpotLightsBuffer();
	}

	SPointLightsShaderData * C3DScene::GetActivePointLightsBuffer() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetActivePointLightsBuffer();
	}

	SDirectionalLightsShaderData * C3DScene::GetActiveDirectionalLightsBuffer() const {
		LOG_TRACE();

		Assert(this->m_scene != nullptr);
		return this->m_scene->GetActiveDirectionalLightsBuffer();
	}
}
