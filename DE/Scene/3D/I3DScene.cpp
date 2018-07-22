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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Memory/CExceptionAssert.hpp"
#include "../../Core/CMediaManager.hpp"
#include "../../Core/Utility/CArraySorter.hpp"
#include "../../Scene/3D/ILevelSceneNode.hpp"
#include "../../Scene/3D/I3DScene.hpp"

#define LIGHTNING_UPDATE_DELAY 20.0F

namespace daidalosengine {

	I3DScene::I3DScene(const std::string & filename) {
		LOG_TRACE();

		this->m_root = nullptr;
		this->m_filename = filename;
		this->m_activeSpotLights = new SSPotLightsShaderData;
		this->m_activePointLights = new SPointLightsShaderData;
		this->m_activeDirectionalLights = new SDirectionalLightsShaderData;
		this->m_activeCamera = nullptr;
		this->m_skybox.SetScene(this);
		this->m_lighteningUpdateTimer = 0;

		this->m_options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		// Create the debug data
		const auto renderer = IRenderer::GetRenderer();
		this->m_debugShader.LoadFromFile("simple");

		// Create the vertex declaration
		daidalosengine::SDeclarationElement declaration[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 }
		};
		this->m_debugVertexDeclaration = renderer->CreateVertexDeclaration(declaration);
	}

	I3DScene::~I3DScene() {
		LOG_TRACE();

		SAFE_DELETE(this->m_root);
		SAFE_DELETE(this->m_activeSpotLights);
		SAFE_DELETE(this->m_activePointLights);
		SAFE_DELETE(this->m_activeDirectionalLights);
	}

	void I3DScene::ClearAll() {
		LOG_TRACE();
	}

	void I3DScene::SetFilename(const std::string & filename) {
		LOG_TRACE();
		this->m_filename = filename;
	}

	void I3DScene::SetRoot(ISceneNode * root) {
		LOG_TRACE();

		SAFE_DELETE(this->m_root);
		this->m_root = root;

		// Update the skybox
		if(!dynamic_cast<ILevelSceneNode*>(this->m_root)->GetSkyName().empty()) {
			this->m_skybox.SetName(dynamic_cast<ILevelSceneNode*>(this->m_root)->GetSkyName());
		}
	}

	void I3DScene::AddSceneNode(ISceneNode * parent, ISceneNode * node) {
		LOG_TRACE();

		if(parent == nullptr) {
			this->m_root->AddChild(node);
		} else {
			parent->AddChild(node);
		}
	}

	void I3DScene::SetActiveCamera(ICameraSceneNode * camera) {
		LOG_TRACE();

		this->m_activeCamera = camera;
		this->m_particleSystem.SetActiveCamera(camera);
	}

	/**
	* Set the particle system atlas texture
	* @param filename The filename
	* @param size The atlas size
	*/
	void I3DScene::SetParticleSystemAtlasTexture(const std::string & filename, const CVector2I & size) {
		this->m_particleSystem.SetAtlasTexture(filename, size);
	}

	/**
	* Set the particle system particle buffer size
	* @param size The size
	*/
	void I3DScene::SetParticleSystemSize(const int size) {
		this->m_particleSystem.InitializeSystem(size);
	}

	void I3DScene::UpdateAll(const float time) {
		LOG_TRACE();

		// Reset the lights buffers
		this->m_activeSpotLights->numberOfLights = 0;
		std::fill(this->m_activeSpotLights->enabled, this->m_activeSpotLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);
		std::fill(this->m_activeSpotLights->visibilityOrder, this->m_activeSpotLights->visibilityOrder + MAX_LIGHTS_IN_SHADERS, -1);
		std::fill(this->m_activeSpotLights->distanceToCamera, this->m_activeSpotLights->distanceToCamera + MAX_LIGHTS_IN_SHADERS, this->m_activeCamera->GetFarDistance() * 200);
		std::fill(this->m_activeSpotLights->node, this->m_activeSpotLights->node + MAX_LIGHTS_IN_SHADERS, nullptr);

		this->m_activePointLights->numberOfLights = 0;
		std::fill(this->m_activePointLights->enabled, this->m_activePointLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);
		std::fill(this->m_activePointLights->visibilityOrder, this->m_activePointLights->visibilityOrder + MAX_LIGHTS_IN_SHADERS, -1);
		std::fill(this->m_activePointLights->distanceToCamera, this->m_activePointLights->distanceToCamera + MAX_LIGHTS_IN_SHADERS, this->m_activeCamera->GetFarDistance() * 200);

		this->m_activeDirectionalLights->numberOfLights = 0;
		std::fill(this->m_activeDirectionalLights->enabled, this->m_activeDirectionalLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);
		std::fill(this->m_activeDirectionalLights->node, this->m_activeDirectionalLights->node + MAX_LIGHTS_IN_SHADERS, nullptr);

		this->m_lighteningUpdateTimer = LIGHTNING_UPDATE_DELAY;

		// Update the active camera first, as it will be necessary for other nodes updates
		if (this->m_activeCamera != nullptr) {
			this->m_activeCamera->Update(time);
		}

		// Update the nodes
		Assert(this->m_root != nullptr);
		this->m_root->Update(time);

		// Update the particle system
		this->m_particleSystem.Update(time);

		// Sort the lights
		SortLights();
		ActiveLightsCheck();
	}

	void I3DScene::RenderSkyboxMask() {
		LOG_TRACE();

		Assert(this->m_root != nullptr);
		Assert(this->m_activeCamera != nullptr);

		const auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		// Global parameters of the scene
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());

		this->m_root->RenderSkyboxMask();
	}

	void I3DScene::RenderSkybox() {
		LOG_TRACE();

		if(this->m_skybox.IsVisible()) {
			this->m_skybox.Render();
		}
	}

	void I3DScene::Render(const int filterId) {
		LOG_TRACE();

		Assert(this->m_root != nullptr);
		Assert(this->m_activeCamera != nullptr);

		const auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		// Global parameters of the scene
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());

		this->m_root->Render(filterId);
	}

	/**
	* Render all the transparent nodes
	*/
	void I3DScene::RenderTransparent() {
		LOG_TRACE();

		Assert(this->m_root != nullptr);
		Assert(this->m_activeCamera != nullptr);

		const auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		// Global parameters of the scene
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());

		this->m_root->RenderTransparent();
	}

	/**
	* Render the particle system
	*/
	void I3DScene::RenderParticleSystem() {
		LOG_TRACE();

		this->m_particleSystem.RenderAll();
	}

	void I3DScene::RenderDebugInformation() {
		LOG_TRACE();

		Assert(this->m_root != nullptr);
		Assert(this->m_activeCamera != nullptr);

		auto renderer = IRenderer::GetRenderer();

		// Disable writing to the z-buffer
		renderer->SetRenderState(RENDER_STATE_TYPE_DEPTHTEST, false);

		// Set the renderer
		renderer->SetVertexDeclaration(this->m_debugVertexDeclaration.get());
		renderer->SetShaderProgram(this->m_debugShader.GetShaderProgram());

		// Global parameters of the scene
		this->m_debugShader.SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());

		this->m_root->RenderDebugInformation();
	}

	void I3DScene::SetMeshFormat(const int format) {

		LOG_TRACE();
		Assert(this->m_root != nullptr);
		this->m_root->SetMeshFormat(format);
	}

	void I3DScene::SendSpotLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_POSITION), this->m_activeSpotLights->position[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_DIRECTION), this->m_activeSpotLights->direction[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_RRFT), this->m_activeSpotLights->rangeRadiusFalloffTightness[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_COLOR), this->m_activeSpotLights->color[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_VIEWPROJ), this->m_activeSpotLights->node[id]->GetViewProjMatrix());

		renderer->SetScissor(this->m_activeSpotLights->scissor[id]);
	}

	void I3DScene::SendPointLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, "pointLightPositionRange", this->m_activePointLights->positionRange[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, "pointLightColorFalloff", this->m_activePointLights->colorFalloff[id]);

		renderer->SetScissor(this->m_activePointLights->scissor[id]);
	}

	void I3DScene::SendDirectionalLightDataToShader(const int id) {
		LOG_TRACE();

		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		const auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_DIRECTION), this->m_activeDirectionalLights->direction[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_COLOR), this->m_activeDirectionalLights->color[id]);
	}

	void I3DScene::SetBrushCollision(const int brushId, const bool collision) {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->SetBrushCollision(brushId, collision);
	}

	bool I3DScene::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->Slide(shape, movement);
	}

	bool I3DScene::GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetCollisionPoint(position, movement, point);
	}

	bool I3DScene::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetCollisionPoint(shape, movement, point);
	}

	int I3DScene::GetNumberOfActiveSpotLights() const {
		LOG_TRACE();

		return this->m_activeSpotLights->numberOfLights;
	}

	int I3DScene::GetNumberOfActivePointLights() const {
		LOG_TRACE();

		return this->m_activePointLights->numberOfLights;
	}

	int I3DScene::GetNumberOfActiveDirectionalLights() const {
		LOG_TRACE();

		return this->m_activeDirectionalLights->numberOfLights;
	}

	int I3DScene::GetNumberOfVisibleTriangles() const {
		LOG_TRACE();

		Assert(this->m_root != nullptr);

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetNumberOfVisibleTriangles();
	}

	const std::string & I3DScene::GetFilename() const {
		LOG_TRACE();

		return this->m_filename;
	}

	const CVector4F & I3DScene::GetAmbientColor() const {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetAmbientColor();
	}

	const CVector4F & I3DScene::GetSkyAmbientColor() const {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetSkyAmbientColor();
	}

	CAABoundingBoxF I3DScene::GetSceneBoundingBox() {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root)->GetLevelBoundingBox();
	}

	CParticleSystem & I3DScene::GetParticleSystem() {
		LOG_TRACE();

		return this->m_particleSystem;
	}

	ISceneNode * I3DScene::GetRoot() const {
		LOG_TRACE();

		return this->m_root;
	}

	ISceneNode * I3DScene::GetNode(const std::string & name) const {
		LOG_TRACE();

		Assert(this->m_root != nullptr);

		return FindNode(this->m_root, name);
	}

	std::vector<ISceneNode*> I3DScene::GetNodeArray(const std::string & name) const {
		LOG_TRACE();

		Assert(this->m_root != nullptr);

		std::vector<ISceneNode*> nodes;
		FindAllNodes(this->m_root, name, nodes);

		return nodes;
	}

	std::vector<ISceneNode*> I3DScene::GetNodeArray(ESceneNodeType nodeType) const {
		LOG_TRACE();

		Assert(this->m_root != nullptr);

		std::vector<ISceneNode*> nodes;
		FindAllNodes(this->m_root, nodeType, nodes);

		return nodes;
	}
	
	std::vector<ISceneNode*> I3DScene::GetNodeArray(const int id) const {
		LOG_TRACE();

		Assert(this->m_root != nullptr);

		std::vector<ISceneNode*> nodes;
		FindAllNodes(this->m_root, id, nodes);

		return nodes;
	}

	ILevelSceneNode * I3DScene::GetRootNode() const {
		LOG_TRACE();

		return dynamic_cast<ILevelSceneNode*>(this->m_root);
	}

	ICameraSceneNode * I3DScene::GetActiveCamera() const {
		LOG_TRACE();

		return this->m_activeCamera;
	}

	SSPotLightsShaderData * I3DScene::GetActiveSpotLightsBuffer() const {
		LOG_TRACE();

		return this->m_activeSpotLights;
	}

	SPointLightsShaderData * I3DScene::GetActivePointLightsBuffer() const {
		LOG_TRACE();

		return this->m_activePointLights;
	}

	SDirectionalLightsShaderData * I3DScene::GetActiveDirectionalLightsBuffer() const {
		LOG_TRACE();

		return this->m_activeDirectionalLights;
	}

	void I3DScene::SortLights() {
		LOG_TRACE();

		CArraySorter<float> sorter;

		// Sort the spot lights
		for(auto i = 0; i < this->m_activeSpotLights->numberOfLights; i++) {
			this->m_activeSpotLights->visibilityOrder[i] = i;
			this->m_activeSpotLights->distanceToCamera[i] = CVector3F(this->m_activeSpotLights->position[i]).Distance(this->m_activeCamera->GetAbsolutePosition());
		}
		sorter.QuickSort(this->m_activeSpotLights->distanceToCamera, this->m_activeSpotLights->visibilityOrder, 0, this->m_activeSpotLights->numberOfLights, ARRAY_SORTER_ORDER_ASC);

		// Sort the point lights
		for(auto i = 0; i < this->m_activePointLights->numberOfLights; i++) {
			this->m_activePointLights->visibilityOrder[i] = i;
			this->m_activePointLights->distanceToCamera[i] = CVector3F(this->m_activePointLights->positionRange[i]).Distance(this->m_activeCamera->GetAbsolutePosition());
		}
		sorter.QuickSort(this->m_activePointLights->distanceToCamera, this->m_activePointLights->visibilityOrder, 0, this->m_activePointLights->numberOfLights, ARRAY_SORTER_ORDER_ASC);
	}

	void I3DScene::ActiveLightsCheck() {
		LOG_TRACE();

		// Check the number of active spot light versus the maximum allowed in the options
		const auto maxSpotLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_MAX_SPOTLIGHT_SOURCES, this->m_activeSpotLights->numberOfLights);
		if(maxSpotLight < this->m_activeSpotLights->numberOfLights) {
			this->m_activeSpotLights->numberOfLights = maxSpotLight;
		}

		// Check the number of active point light versus the maximum allowed in the options
		const auto maxPointLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_MAX_POINTLIGHT_SOURCES, this->m_activePointLights->numberOfLights);
		if(maxPointLight < this->m_activePointLights->numberOfLights) {
			this->m_activePointLights->numberOfLights = maxPointLight;
		}

		// Check the number of active directional light versus the maximum allowed in the options
		const auto maxDirectionalLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTING, APPLICATION_OPTION_LIGHTING_MAX_DIRECTIONALLIGHT_SOURCES, this->m_activeDirectionalLights->numberOfLights);
		if(maxDirectionalLight < this->m_activeDirectionalLights->numberOfLights) {
			this->m_activeDirectionalLights->numberOfLights = maxDirectionalLight;
		}
	}

	void I3DScene::FindAllNodes(ISceneNode * currentNode, const std::string & name, std::vector<ISceneNode*> & vector) const {
		LOG_TRACE();

		if(currentNode->GetName() == name) {
			vector.push_back(currentNode);
		}

		for (auto it : currentNode->GetChildren()) {
			FindAllNodes(it, name, vector);
		}
	}

	void I3DScene::FindAllNodes(ISceneNode * currentNode, ESceneNodeType nodeType, std::vector<ISceneNode*> & vector) const {
		LOG_TRACE();

		if(currentNode->GetType() == nodeType) {
			vector.push_back(currentNode);
		}

		for (auto it : currentNode->GetChildren()) {
			FindAllNodes(it, nodeType, vector);
		}
	}

	void I3DScene::FindAllNodes(ISceneNode * currentNode, const int id, std::vector<ISceneNode*> & vector) const {
		LOG_TRACE();

		if(currentNode->GetId() == id) {
			vector.push_back(currentNode);
		}

		for (auto it : currentNode->GetChildren()) {
			FindAllNodes(it, id, vector);
		}
	}

	ISceneNode * I3DScene::FindNode(ISceneNode * currentNode, const std::string & name) const {
		LOG_TRACE();

		if(currentNode->GetName() == name) {
			return currentNode;
		}

		for (auto it : currentNode->GetChildren()) {
			const auto result = FindNode(it, name);
			if (result != nullptr) {
				return result;
			}
		}

		return nullptr;
	}
}
