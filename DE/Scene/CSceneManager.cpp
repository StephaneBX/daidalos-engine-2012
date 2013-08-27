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

#include "../Core/Memory/CExceptionAssert.h"
#include "../Core/CMediaManager.h"
#include "../Core/Utility/CArraySorter.h"
#include "../Scene/ILevelSceneNode.h"
#include "../Scene/CSceneManager.h"

#define LIGHTNING_UPDATE_DELAY 20.0f

namespace DE
{
	CSceneManager::~CSceneManager()
	{
		SAFE_DELETE(this->m_root);
		SAFE_DELETE(this->m_activeSpotLights);
		SAFE_DELETE(this->m_activePointLights);
		SAFE_DELETE(this->m_activeDirectionalLights);
	}

	void CSceneManager::LoadScene(const std::string & sceneFile)
	{
		SAFE_DELETE(this->m_root);
		this->m_root = CMediaManager::GetInstance()->LoadMediaFromFile<ISceneNode>(sceneFile);

		this->m_activeCamera = nullptr;
		this->m_lighteningUpdateTimer = 0;
	}

	void CSceneManager::ClearAll()
	{

	}

	void CSceneManager::AddSceneNode(ISceneNode * parent, ISceneNode * node)
	{
		if(parent == nullptr)
			this->m_root->AddChild(node);
		else
			parent->AddChild(node);
	}
	
	void CSceneManager::SetActiveCamera(ICameraSceneNode * camera)
	{
		this->m_activeCamera = camera;
	}

	void CSceneManager::UpdateAll(const float time)
	{
		// Reset the lights buffers
		this->m_activeSpotLights->numberOfLights = 0;
		std::fill(this->m_activeSpotLights->enabled, this->m_activeSpotLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);
		std::fill(this->m_activeSpotLights->visibilityOrder, this->m_activeSpotLights->visibilityOrder + MAX_LIGHTS_IN_SHADERS, -1);
		std::fill(this->m_activeSpotLights->distanceToCamera, this->m_activeSpotLights->distanceToCamera + MAX_LIGHTS_IN_SHADERS, this->m_activeCamera->GetFarDistance() * 2);
		std::fill(this->m_activeSpotLights->node, this->m_activeSpotLights->node + MAX_LIGHTS_IN_SHADERS, nullptr);

		this->m_activePointLights->numberOfLights = 0;
		std::fill(this->m_activePointLights->enabled, this->m_activePointLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);
		std::fill(this->m_activePointLights->visibilityOrder, this->m_activePointLights->visibilityOrder + MAX_LIGHTS_IN_SHADERS, -1);
		std::fill(this->m_activePointLights->distanceToCamera, this->m_activePointLights->distanceToCamera + MAX_LIGHTS_IN_SHADERS, this->m_activeCamera->GetFarDistance() * 2);

		this->m_activeDirectionalLights->numberOfLights = 0;
		std::fill(this->m_activeDirectionalLights->enabled, this->m_activeDirectionalLights->enabled + MAX_LIGHTS_IN_SHADERS, 0);

		this->m_lighteningUpdateTimer = LIGHTNING_UPDATE_DELAY;

		// Update the active camera first, as it will be necessary for other nodes updates
		if(this->m_activeCamera != nullptr)
			this->m_activeCamera->Update(time);

		// Update the nodes
		Assert(this->m_root != nullptr);
		this->m_root->Update(time);

		// Sort the lights
		SortLights();

		ActiveLightsCheck();
	}

	void CSceneManager::RenderAll()
	{
		Assert(this->m_root != nullptr);
		Assert(this->m_activeCamera != nullptr);

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		// Global parameters of the scene
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());

		this->m_root->Render();
	}

	void CSceneManager::SetMeshFormat(const int format)
	{
		Assert(this->m_root != nullptr);
		this->m_root->SetMeshFormat(format);
	}

	void CSceneManager::SendSpotLightDataToShader(const int id)
	{
		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_POSITION), this->m_activeSpotLights->position[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_DIRECTION), this->m_activeSpotLights->direction[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_RRFT), this->m_activeSpotLights->rangeRadiusFalloffTightness[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_COLOR), this->m_activeSpotLights->color[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SPOTLIGHT_VIEWPROJ), this->m_activeSpotLights->node[id]->GetViewProjMatrix());

		renderer->SetScissor(this->m_activeSpotLights->scissor[id]);
	}

	void CSceneManager::SendPointLightDataToShader(const int id)
	{
		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, "pointLightPositionRange", this->m_activePointLights->positionRange[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, "pointLightColorFalloff", this->m_activePointLights->colorFalloff[id]);

		renderer->SetScissor(this->m_activePointLights->scissor[id]);
	}

	void CSceneManager::SendDirectionalLightDataToShader(const int id)
	{
		Assert(id >= 0 && id < MAX_LIGHTS_IN_SHADERS);

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_POSITION), this->m_activeDirectionalLights->position[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_DIRECTION), this->m_activeDirectionalLights->direction[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_COLOR), this->m_activeDirectionalLights->color[id]);
		shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_DIRECTIONALLIGHT_VIEWPROJ), this->m_activeDirectionalLights->node[id]->GetViewProjMatrix());
	}

	const bool CSceneManager::Slide(IShape3DF & shape, const CVector3F & movement)
	{
		return static_cast<ILevelSceneNode*>(this->m_root)->Slide(shape, movement);
	}

	const bool CSceneManager::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point)
	{
		return static_cast<ILevelSceneNode*>(this->m_root)->GetCollisionPoint(shape, movement, point);
	}

	const int CSceneManager::GetNumberOfActiveSpotLights() const
	{
		return this->m_activeSpotLights->numberOfLights;
	}

	const int CSceneManager::GetNumberOfActivePointLights() const
	{
		return this->m_activePointLights->numberOfLights;
	}

	const int CSceneManager::GetNumberOfActiveDirectionalLights() const
	{
		return this->m_activeDirectionalLights->numberOfLights;
	}

	const int CSceneManager::GetNumberOfVisibleTriangles() const
	{
		Assert(this->m_root != nullptr);

		return static_cast<ILevelSceneNode*>(this->m_root)->GetNumberOfVisibleTriangles();
	}

	const CVector4F & CSceneManager::GetAmbientColor() const
	{
		return static_cast<ILevelSceneNode*>(this->m_root)->GetAmbientColor();
	}

	ISceneNode * CSceneManager::GetNode(const std::string & name) const
	{
		Assert(this->m_root != nullptr);

		return FindNode(this->m_root, name);
	}

	std::vector<ISceneNode*> CSceneManager::GetNodeArray(const std::string & name) const
	{
		Assert(this->m_root != nullptr);

		std::vector<ISceneNode*> nodes;
		FindAllNodes(m_root, name, nodes);

		return nodes;
	}

	ILevelSceneNode * CSceneManager::GetRootNode() const
	{
		return static_cast<ILevelSceneNode*>(this->m_root);
	}

	ICameraSceneNode * CSceneManager::GetActiveCamera() const
	{
		return this->m_activeCamera;
	}

	SSPotLightsShaderData * CSceneManager::GetActiveSpotLightsBuffer() const
	{
		return this->m_activeSpotLights;
	}

	SPointLightsShaderData * CSceneManager::GetActivePointLightsBuffer() const
	{
		return this->m_activePointLights;
	}

	SDirectionalLightsShaderData * CSceneManager::GetActiveDirectionalLightsBuffer() const
	{
		return this->m_activeDirectionalLights;
	}

	CSceneManager::CSceneManager()
	{
		this->m_root = new ISceneNode(nullptr);
		this->m_activeSpotLights = new SSPotLightsShaderData;
		this->m_activePointLights = new SPointLightsShaderData;
		this->m_activeDirectionalLights = new SDirectionalLightsShaderData;
		this->m_activeCamera = nullptr;

		this->m_options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);
	}

	void CSceneManager::SortLights()
	{
		CArraySorter<float> sorter;

		// Sort the spot lights
		for(int i = 0 ; i < this->m_activeSpotLights->numberOfLights ; i++)
		{
			this->m_activeSpotLights->visibilityOrder[i] = i;
			this->m_activeSpotLights->distanceToCamera[i] = CVector3F(this->m_activeSpotLights->position[i]).Distance(this->m_activeCamera->GetAbsolutePosition());
		}
		sorter.QuickSort(this->m_activeSpotLights->distanceToCamera, this->m_activeSpotLights->visibilityOrder, 0, this->m_activeSpotLights->numberOfLights);

		// Sort the point lights
		for(int i = 0 ; i < this->m_activePointLights->numberOfLights ; i++)
		{
			this->m_activePointLights->visibilityOrder[i] = i;
			this->m_activePointLights->distanceToCamera[i] = CVector3F(this->m_activePointLights->positionRange[i]).Distance(this->m_activeCamera->GetAbsolutePosition());
		}
		sorter.QuickSort(this->m_activePointLights->distanceToCamera, this->m_activePointLights->visibilityOrder, 0, this->m_activePointLights->numberOfLights);
	}

	void CSceneManager::ActiveLightsCheck()
	{
		// Check the number of active spot light versus the maximum allowed in the options
		int maxSpotLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_MAX_SPOTLIGHT_SOURCES, this->m_activeSpotLights->numberOfLights);
		if(maxSpotLight < this->m_activeSpotLights->numberOfLights) 
		{
			this->m_activeSpotLights->numberOfLights = maxSpotLight;
		}

		// Check the number of active point light versus the maximum allowed in the options
		int maxPointLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_MAX_POINTLIGHT_SOURCES, this->m_activePointLights->numberOfLights);
		if(maxPointLight < this->m_activePointLights->numberOfLights) 
		{
			this->m_activePointLights->numberOfLights = maxPointLight;
		}

		// Check the number of active directional light versus the maximum allowed in the options
		int maxDirectionalLight = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_LIGHTENING, APPLICATION_OPTION_LIGHTENING_MAX_DIRECTIONALLIGHT_SOURCES, this->m_activeDirectionalLights->numberOfLights);
		if(maxDirectionalLight < this->m_activeDirectionalLights->numberOfLights) 
		{
			this->m_activeDirectionalLights->numberOfLights = maxDirectionalLight;
		}
	}

	void CSceneManager::FindAllNodes(ISceneNode * currentNode, const std::string & name, std::vector<ISceneNode*> & vector) const
	{
		if(currentNode->GetName() == name)
			vector.push_back(currentNode);

		for(auto it = currentNode->GetChildren().begin() ; it != currentNode->GetChildren().end() ; ++it)
		{
			FindAllNodes(*it, name, vector);
		}
	}

	ISceneNode * CSceneManager::FindNode(ISceneNode * currentNode, const std::string & name) const
	{
		if(currentNode->GetName() == name)
			return currentNode;

		ISceneNode * result;
		for(auto it = currentNode->GetChildren().begin() ; it != currentNode->GetChildren().end() ; ++it)
		{
			result = FindNode(*it, name);
			if(result != nullptr)
				return result;
		}

		return nullptr;
	}
}
