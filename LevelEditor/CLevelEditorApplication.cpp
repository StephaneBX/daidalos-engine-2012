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

#include <Core/CMediaManager.h>
#include "CEditorSceneLoader.h"
#include "CLevelEditorApplication.h"

namespace EDITOR {

	#define EDITOR_NODE_NAME_CAMERA "__editor_node_camera"
	#define EDITOR_NODE_NAME_CURRENT "__editor_node_current"
	#define EDITOR_NODE_NAME_MULTIBRUSH "__editor_node_multibrush"

	enum EMeshCategorySelectionMode {
		MESH_CATEGORY_SELECTION_MODE_NONE,
		MESH_CATEGORY_SELECTION_MODE_GROUND,
		MESH_CATEGORY_SELECTION_MODE_WALL,
		MESH_CATEGORY_SELECTION_MODE_CEILING
	};

	CLevelEditorApplication::CLevelEditorApplication(const HINSTANCE hInstance, const int nCmdShow) : IForwardSampleApplication(hInstance, nCmdShow) {

		// Replace the default level loader
		DE::CMediaManager::GetInstance()->AddLoader(new CEditorSceneLoader(), "des");

		this->m_viewMode = true;
		this->m_drawSkybox = true;
		this->m_drawLights = true;
		this->m_meshCategorySelectionMode = MESH_CATEGORY_SELECTION_MODE_NONE;
		this->m_wallMeshEditIndex = 0;
		this->m_wallTextureEditIndex = 0;
		this->m_cameraPosition = DE::CVector3F(0, 4, 0);
		this->m_meshNode = nullptr;
		//std::thread(&CLevelEditorApplication::LoadLevel, this, "sample1.des").detach();

		LoadLevel("sample1.des");

		MainLoop();
	}

	CLevelEditorApplication::~CLevelEditorApplication() {}

	void CLevelEditorApplication::Update(const float time) {

		if(!this->m_loading) {

			if(this->m_input->IsActionActive(EDITOR_ACTION_LIGHTS)) {
				this->m_drawLights = !this->m_drawLights;
				this->m_input->FreezeAction(EDITOR_ACTION_LIGHTS);
				this->m_scene3D->GetRootNode()->SetUseGlobalAmbientColor(!this->m_drawLights);
			}

			if(this->m_viewMode) {
				UpdateViewMode(time);
			} else {
				UpdateEditMode(time);
			}
			
			this->m_scene3D->SetActiveCamera(this->m_activeCamera);
			this->m_scene3D->UpdateAll(time);

			this->m_cursor.Update(time);
			this->m_scene2D->Update(time);

			this->m_cameraPosition = DE::CVector3F(this->m_activeCamera->GetPosition());
			this->m_cameraRotation = DE::CVector3F(this->m_activeCamera->GetRotation());

			if(this->m_input->IsActionActive(EDITOR_ACTION_REFRESH) && this->m_viewMode) {
				std::thread(&CLevelEditorApplication::LoadLevel, this, "sample1.des").detach();
			}

		} else {
			this->m_loadingScene->Update(time);
		}
	}

	/**
	* Update the view mode
	* @param time Elapsed time
	*/
	void CLevelEditorApplication::UpdateViewMode(const float time) {

		// Skybox state
		if(this->m_input->IsActionActive(EDITOR_ACTION_SKYBOX)) {
			this->m_drawSkybox = !this->m_drawSkybox;
			this->m_input->FreezeAction(EDITOR_ACTION_SKYBOX);
		}

		// Update the 3D scene
		if(!this->m_cursor.IsVisible()) {
			this->m_viewCamera->Update(time);
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_EXIT)) {
			this->m_alive = false;
		}

		/*// Cursor state
		if(this->m_input->IsActionActive(VIEW_CAMERA_CURSOR)) {
			this->m_cursor.SetVisible(!this->m_cursor.IsVisible());
			this->m_input->FreezeAction(VIEW_CAMERA_CURSOR);
		}*/

		// edit mode state
		if(this->m_input->IsActionActive(EDITOR_ACTION_CHANGE_MODE)) {
			this->m_viewMode = false;
			this->m_drawSkybox = false;
			this->m_input->FreezeAction(EDITOR_ACTION_CHANGE_MODE);
			this->m_activeCamera = this->m_editCamera->GetCameraNode();
			UpdateNodesVisibility();
		}
	}

	/**
	* Update the edit mode
	* @param time Elapsed time
	*/
	void CLevelEditorApplication::UpdateEditMode(const float time) {
		this->m_editCamera->Update(time);

		if(this->m_input->IsActionActive(EDITOR_ACTION_CAMERA_LAYER_UP) || this->m_input->IsActionActive(EDITOR_ACTION_CAMERA_LAYER_DOWN)) {
			this->m_input->FreezeAction(EDITOR_ACTION_CAMERA_LAYER_UP);
			this->m_input->FreezeAction(EDITOR_ACTION_CAMERA_LAYER_DOWN);
			UpdateNodesVisibility();
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_SELECT_GROUND)) {
			this->m_input->FreezeAction(EDITOR_ACTION_SELECT_GROUND);
			this->m_meshCategorySelectionMode = MESH_CATEGORY_SELECTION_MODE_GROUND;
			UpdateMeshNode();
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_SELECT_WALL)) {
			this->m_input->FreezeAction(EDITOR_ACTION_SELECT_WALL);
			this->m_meshCategorySelectionMode = MESH_CATEGORY_SELECTION_MODE_WALL;
			UpdateMeshNode();
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_SELECT_CEILING)) {
			this->m_input->FreezeAction(EDITOR_ACTION_SELECT_CEILING);
			this->m_meshCategorySelectionMode = MESH_CATEGORY_SELECTION_MODE_CEILING;
			UpdateMeshNode();
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_PUT_MESH)) {
			this->m_input->FreezeAction(EDITOR_ACTION_PUT_MESH);
			
			if(this->m_meshNode != nullptr) {
				DE::CVector3F rotation = this->m_meshNode->GetRotation();
				this->m_meshNode->SetName(EDITOR_NODE_NAME_MULTIBRUSH);
				this->m_meshNode = nullptr;
				UpdateMeshNode(rotation);
			}
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_DELETE_MESH)) {
			this->m_input->FreezeAction(EDITOR_ACTION_DELETE_MESH);

			std::vector<DE::ISceneNode *> nodesToRemove = GetNodesOnBlock(DE::CVector2I((int)this->m_editCamera->GetCameraNode()->GetPosition().x, (int)this->m_editCamera->GetCameraNode()->GetPosition().z), (int)this->m_editCamera->GetLayer());
			for(auto it = nodesToRemove.begin(); it != nodesToRemove.end(); it++) {
				this->m_scene3D->GetRootNode()->RemoveChild((*it));
			}
		}

		if(this->m_meshNode != nullptr) {
			this->m_meshNode->SetPosition(DE::CVector3F(this->m_editCamera->GetCameraNode()->GetPosition().x, this->m_editCamera->GetLayer() * EDIT_CAMERA_LAYER_HEIGHT, this->m_editCamera->GetCameraNode()->GetPosition().z));

			// Next texture
			if(this->m_input->IsActionActive(EDITOR_ACTION_TEXTURE_NEXT)) {
				this->m_input->FreezeAction(EDITOR_ACTION_TEXTURE_NEXT);
				UpdateTextureNext();
			}

			// Next mesh
			if(this->m_input->IsActionActive(EDITOR_ACTION_MESH_NEXT)) {
				this->m_input->FreezeAction(EDITOR_ACTION_MESH_NEXT);
				UpdateMeshNext();
			}

			// Rotate mesh
			if(this->m_input->IsActionActive(EDITOR_ACTION_MESH_ROTATE)) {
				this->m_input->FreezeAction(EDITOR_ACTION_MESH_ROTATE);
				this->m_meshNode->SetRotation(DE::CVector3F(0, (this->m_meshNode->GetRotation().y + 90), 0));
			}
		}

		// Close edit mode
		if(this->m_input->IsActionActive(EDITOR_ACTION_CHANGE_MODE)) {
			this->m_viewMode = true;
			this->m_input->FreezeAction(EDITOR_ACTION_CHANGE_MODE);
			this->m_activeCamera = this->m_viewCamera->GetCameraNode();
			UpdateNodesVisibility();
		}
	}

	/**
	* Update the selection mesh node
	* @param rotation The rotation
	*/
	void CLevelEditorApplication::UpdateMeshNode(const DE::CVector3F & rotation) {
		std::string meshFilename;
		std::string textureDiffuseFilename;
		std::string textureNormalFilename;

		switch(this->m_meshCategorySelectionMode) {
		case MESH_CATEGORY_SELECTION_MODE_GROUND:
			meshFilename = this->m_configuration.GetGroundMeshFilenames()[this->m_groundMeshEditIndex];
			textureDiffuseFilename = this->m_configuration.GetGroundTextureDiffuseFilenames()[this->m_groundTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetGroundTextureNormalFilenames()[this->m_groundTextureEditIndex];
			break;

		case MESH_CATEGORY_SELECTION_MODE_WALL:
			meshFilename = this->m_configuration.GetWallMeshFilenames()[this->m_wallMeshEditIndex];
			textureDiffuseFilename = this->m_configuration.GetWallTextureDiffuseFilenames()[this->m_wallTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetWallTextureNormalFilenames()[this->m_wallTextureEditIndex];
			break;

		case MESH_CATEGORY_SELECTION_MODE_CEILING:
			meshFilename = this->m_configuration.GetCeilingMeshFilenames()[this->m_ceilingMeshEditIndex];
			textureDiffuseFilename = this->m_configuration.GetCeilingTextureDiffuseFilenames()[this->m_ceilingTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetCeilingTextureNormalFilenames()[this->m_ceilingTextureEditIndex];
			break;
		default:
			break;
		}

		// Update or create a new node
		if(this->m_meshNode != nullptr) {
			this->m_meshNode->SetMesh(meshFilename);
			
		} else {
			this->m_meshNode = new DE::CStaticMeshSceneNode(this->m_scene3D->GetScene(), this->m_scene3D->GetRootNode(), EDITOR_NODE_NAME_CURRENT, meshFilename, DE::CVector3F(), rotation, DE::CVector3F(1.0f, 1.0f, 1.0f));
			this->m_meshNode->SetId(0);
			this->m_meshNode->SetMeshShadingType(DE::MESH_SHADING_FLAT);
			this->m_scene3D->GetRootNode()->AddChild(this->m_meshNode);
		}
		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, textureDiffuseFilename, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, textureNormalFilename, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
	}

	/**
	* Update the visibility of the nodes
	*/
	void CLevelEditorApplication::UpdateNodesVisibility() {

		// Get all the mesh nodes
		std::vector<DE::ISceneNode *> nodes = this->m_scene3D->GetNodeArray(DE::SCENE_NODE_STATICMESH);
		std::vector<DE::ISceneNode *> animatedMeshNodes = this->m_scene3D->GetNodeArray(DE::SCENE_NODE_ANIMATEDMESH);
		nodes.insert(nodes.end(), animatedMeshNodes.begin(), animatedMeshNodes.end());

		if(this->m_viewMode) {
			// Everything is visible
			for(auto it = nodes.begin(); it != nodes.end(); it++) {
				(*it)->SetEnabled(true);
			}

			if(this->m_meshNode != nullptr) {
				this->m_meshNode->SetEnabled(false);
			}
			
		} else {
			// Filter to only see the current layer
			for(auto it = nodes.begin(); it != nodes.end(); it++) {
				if((*it)->GetPosition().y > (((this->m_editCamera->GetLayer() - 1) * EDIT_CAMERA_LAYER_HEIGHT) - EDIT_CAMERA_LAYER_HEIGHT) && (*it)->GetPosition().y < (((this->m_editCamera->GetLayer() + 1) * EDIT_CAMERA_LAYER_HEIGHT) + EDIT_CAMERA_LAYER_HEIGHT)) {
					(*it)->SetEnabled(true);
				} else {
					(*it)->SetEnabled(false);
				}
			}

			if(this->m_meshNode != nullptr) {
				this->m_meshNode->SetEnabled(true);
			}
		}
	}

	/**
	* Update the next texture to apply depending on the mesh category selection
	*/
	void CLevelEditorApplication::UpdateTextureNext() {

		std::string textureDiffuseFilename;
		std::string textureNormalFilename;

		switch(this->m_meshCategorySelectionMode) {
		case MESH_CATEGORY_SELECTION_MODE_GROUND:
			this->m_groundTextureEditIndex = (this->m_groundTextureEditIndex + 1) % this->m_configuration.GetGroundTextureDiffuseFilenames().size();
			textureDiffuseFilename = this->m_configuration.GetGroundTextureDiffuseFilenames()[this->m_groundTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetGroundTextureNormalFilenames()[this->m_groundTextureEditIndex];
			break;

		case MESH_CATEGORY_SELECTION_MODE_WALL:
			this->m_wallTextureEditIndex = (this->m_wallTextureEditIndex + 1) % this->m_configuration.GetWallTextureDiffuseFilenames().size();
			textureDiffuseFilename = this->m_configuration.GetWallTextureDiffuseFilenames()[this->m_wallTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetWallTextureNormalFilenames()[this->m_wallTextureEditIndex];
			break;

		case MESH_CATEGORY_SELECTION_MODE_CEILING:
			this->m_ceilingTextureEditIndex = (this->m_ceilingTextureEditIndex + 1) % this->m_configuration.GetCeilingTextureDiffuseFilenames().size();
			textureDiffuseFilename = this->m_configuration.GetCeilingTextureDiffuseFilenames()[this->m_ceilingTextureEditIndex];
			textureNormalFilename = this->m_configuration.GetCeilingTextureNormalFilenames()[this->m_ceilingTextureEditIndex];
			break;
		default:
			break;
		}

		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, textureDiffuseFilename, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, textureNormalFilename, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
	}

	/**
	* Update the next mesh to apply depending on the mesh category selection
	*/
	void CLevelEditorApplication::UpdateMeshNext() {

		switch(this->m_meshCategorySelectionMode) {
		case MESH_CATEGORY_SELECTION_MODE_GROUND:
			this->m_groundMeshEditIndex = (this->m_groundMeshEditIndex + 1) % this->m_configuration.GetGroundMeshFilenames().size();
			break;

		case MESH_CATEGORY_SELECTION_MODE_WALL:
			this->m_wallMeshEditIndex = (this->m_wallMeshEditIndex + 1) % this->m_configuration.GetWallMeshFilenames().size();
			break;

		case MESH_CATEGORY_SELECTION_MODE_CEILING:
			this->m_ceilingMeshEditIndex = (this->m_ceilingMeshEditIndex + 1) % this->m_configuration.GetCeilingMeshFilenames().size();
			break;
		default:
			break;
		}

		if(this->m_meshNode != nullptr) {
			UpdateMeshNode();
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

		DE::CProjectionCameraSceneNode * viewCameraNode = new DE::CProjectionCameraSceneNode(this->m_scene3D->GetScene(), this->m_scene3D->GetRootNode(), EDITOR_NODE_NAME_CAMERA, DE::CVector3F(), DE::CVector3F(), DE::DegreeToRadian(55), this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y, 0.1f, 200.0f);
		this->m_scene3D->GetRootNode()->AddChild(viewCameraNode);

		this->m_viewCamera = std::unique_ptr<CViewCamera>(new CViewCamera(viewCameraNode));
		this->m_viewCamera->SetScene(this->m_scene3D);
		this->m_viewCamera->SetPosition(this->m_cameraPosition);
		this->m_viewCamera->SetRotation(this->m_cameraRotation);

		DE::CProjectionCameraSceneNode * editCameraNode = new DE::CProjectionCameraSceneNode(this->m_scene3D->GetScene(), this->m_scene3D->GetRootNode(), EDITOR_NODE_NAME_CAMERA, DE::CVector3F(0, 100, 0), DE::CVector3F(80, 0, 0), DE::DegreeToRadian(55), this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y, 0.1f, 200.0f);
		this->m_scene3D->GetRootNode()->AddChild(editCameraNode);

		this->m_editCamera = std::unique_ptr<CEditCamera>(new CEditCamera(editCameraNode));
		this->m_editCamera->SetScene(this->m_scene3D);

		this->m_scene3D->GetRootNode()->SetAmbientColor(DE::CVector4F(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_scene3D->GetRootNode()->SetUseGlobalAmbientColor(!this->m_drawLights);
		this->m_activeCamera = viewCameraNode;

		//
	}

	/**
	* Get the node atching the position, the laer and the name
	* @param position The position
	* @param layer The layer
	* @param name The name
	* @return A vector of nodes
	*/
	std::vector<DE::ISceneNode*> CLevelEditorApplication::GetNodesOnBlock(DE::CVector2I position, int layer) {
		std::vector<DE::ISceneNode*> node;
		std::vector<DE::ISceneNode*> nodes = this->m_scene3D->GetNodeArray(EDITOR_NODE_NAME_MULTIBRUSH);

		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			if((*it)->GetPosition().x > (position.x - EDIT_CAMERA_STEP) && (*it)->GetPosition().x < (position.x + EDIT_CAMERA_STEP) && (*it)->GetPosition().z >(position.y - EDIT_CAMERA_STEP) && (*it)->GetPosition().z < (position.y + EDIT_CAMERA_STEP) && (*it)->GetPosition().y >((layer * EDIT_CAMERA_LAYER_HEIGHT) - EDIT_CAMERA_LAYER_HEIGHT) && (*it)->GetPosition().y < ((layer * EDIT_CAMERA_LAYER_HEIGHT) + EDIT_CAMERA_LAYER_HEIGHT)) {
				node.push_back(*it);
			}
		}
		return node;
	}
}
