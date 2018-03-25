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

#include <Core/CMediaManager.hpp>
#include <Scene/3D/ISceneNode.hpp>
#include "Loaders/CEditorSceneLoader.hpp"
#include "CEditorLevel.hpp"
#include "CLevelEditorApplication.hpp"

namespace daidaloseditor {

	enum EMeshCategorySelectionMode {
		MESH_CATEGORY_SELECTION_MODE_NONE,
		MESH_CATEGORY_SELECTION_MODE_GROUND,
		MESH_CATEGORY_SELECTION_MODE_WALL,
		MESH_CATEGORY_SELECTION_MODE_CEILING
	};

	CLevelEditorApplication::CLevelEditorApplication(const HINSTANCE hInstance, const int nCmdShow) : IForwardApplication(hInstance, nCmdShow) {

		// Replace the default level loader
		daidalosengine::CMediaManager::GetInstance()->AddLoader(new CEditorSceneLoader(), "des");

		// Tmp
		daidalosengine::CMediaManager::GetInstance()->AddSearchPath("Resources/Textures/Tank/");

		this->m_viewMode = true;
		this->m_drawSkybox = true;
		this->m_drawLights = true;
		this->m_boundingBox = true;
		this->m_meshCategorySelectionMode = MESH_CATEGORY_SELECTION_MODE_NONE;
		this->m_wallMeshEditIndex = 0;
		this->m_wallTextureEditIndex = 0;
		this->m_cameraPosition = daidalosengine::CVector3F(0, 4, 0);
		this->m_meshNode = nullptr;
		SetLoading(true);
		SetCullEnabled(true);
		
		this->m_consoleView = new CConsoleView(5);
		this->m_consoleView->AddListener(this);

		std::thread(&CLevelEditorApplication::LoadLevel, this, "minimal.des").detach();

		MainLoop();
	}

	CLevelEditorApplication::~CLevelEditorApplication() {}

	/**
	* Load a level and show a loading screen
	* Should be called from a new thread
	* @param filename The level filename
	*/
	void CLevelEditorApplication::LoadLevel(const std::string & filename) {
		SetLevel(new CEditorLevel());
		GetCurrentLevel()->AddListener(this);
		GetCurrentLevel()->LoadLevel(filename);
	}

	void CLevelEditorApplication::Update(const float time) {

		if(!IsLoading()) {

			// Console
			if(this->m_input->IsActionActive(EDITOR_ACTION_CONSOLE)) {
				this->m_input->FreezeAction(EDITOR_ACTION_CONSOLE);
				this->m_consoleView->SetVisible(!this->m_consoleView->IsVisible());
			}

			// The console view pauses everything
			if(!this->m_consoleView->IsVisible()) {
				// Lights state
				if(this->m_input->IsActionActive(EDITOR_ACTION_LIGHTS)) {
					this->m_drawLights = !this->m_drawLights;
					this->m_input->FreezeAction(EDITOR_ACTION_LIGHTS);
					GetCurrentLevel()->GetScene()->GetRootNode()->SetUseGlobalAmbientColor(!this->m_drawLights);
				}

				// Culling state
				if(this->m_input->IsActionActive(EDITOR_ACTION_CULLING)) {
					SetCullEnabled(!IsCullEnabled());
					this->m_input->FreezeAction(EDITOR_ACTION_CULLING);
				}

				// Bounding box
				if(this->m_input->IsActionActive(EDITOR_ACTION_BOUNDINGBOX)) {
					this->m_boundingBox = !this->m_boundingBox;
					this->m_input->FreezeAction(EDITOR_ACTION_BOUNDINGBOX);

					std::vector<daidalosengine::ISceneNode*> nodes = GetCurrentLevel()->GetScene()->GetNodeArray(EDITOR_NODE_NAME_BOUNDINGBOX);
					for(auto it = nodes.begin(); it != nodes.end(); it++) {
						(*it)->SetEnabled(this->m_boundingBox);
					}
				}

				if(this->m_viewMode) {
					UpdateViewMode(time);
				} else {
					UpdateEditMode(time);
				}

				GetCurrentLevel()->GetScene()->SetActiveCamera(this->m_activeCamera);
				GetCurrentLevel()->SetActiveCamera(this->m_activeCamera);
				GetCurrentLevel()->Update(time);

				GetCursor().Update(time);
				//this->m_scene2D->Update(time);

				this->m_cameraPosition = daidalosengine::CVector3F(this->m_activeCamera->GetPosition());
				this->m_cameraRotation = daidalosengine::CVector3F(this->m_activeCamera->GetRotation());

				if(this->m_input->IsActionActive(EDITOR_ACTION_REFRESH) && this->m_viewMode) {
					SetLoading(true);
					std::thread(&CLevelEditorApplication::LoadLevel, this, GetCurrentLevel()->GetScene()->GetFilename()).detach();
				}
			} else {
				GetCurrentLevel()->GetView()->Update(time);
			}

		} else {
			GetLoadingScene()->Update(time);
		}
	}

	/**
	* Called when a load command is inputed in the console
	* @param filename The level filename to load
	*/
	void CLevelEditorApplication::OnConsoleCommandLoad(const std::string filename) {
		SetLoading(true);
		std::thread(&CLevelEditorApplication::LoadLevel, this, filename).detach();
	}

	/**
	* Called when a save command is inputed in the console
	* @param filename The level filename to save the current scene into
	*/
	void CLevelEditorApplication::OnConsoleCommandSave(const std::string filename) {
		GetCurrentLevel()->GetScene()->SaveScene(filename);
	}

	/**
	* Update the view mode
	* @param time Elapsed time
	*/
	void CLevelEditorApplication::UpdateViewMode(const float time) {

		// Update the 3D scene
		//if(!this->m_cursor.IsVisible()) {
			this->m_viewCamera->Update(time);
		//}

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
				daidalosengine::CVector3F rotation = this->m_meshNode->GetRotation();
				this->m_meshNode->SetName(EDITOR_NODE_NAME_MULTIBRUSH);
				this->m_meshNode->SetSaved(true);

				if(this->m_meshNode->GetMeshTextureName(daidalosengine::TEXTURE_TYPE_DIFFUSE).compare(EDITOR_SKY_TEXURE_NAME) == 0) {
					this->m_meshNode->SetId(SCENE_SKYBOX_ID);
				}

				this->m_meshNode = nullptr;
				UpdateMeshNode(rotation);
			}
		}

		if(this->m_input->IsActionActive(EDITOR_ACTION_DELETE_MESH)) {
			this->m_input->FreezeAction(EDITOR_ACTION_DELETE_MESH);

			std::vector<daidalosengine::ISceneNode *> nodesToRemove = GetNodesOnBlock(daidalosengine::CVector2I((int)this->m_editCamera->GetCameraNode()->GetPosition().x, (int)this->m_editCamera->GetCameraNode()->GetPosition().z), (int)this->m_editCamera->GetLayer());
			for(auto it = nodesToRemove.begin(); it != nodesToRemove.end(); it++) {
				GetCurrentLevel()->GetScene()->GetRootNode()->RemoveChild(*it);
			}
		}

		if(this->m_meshNode != nullptr) {
			this->m_meshNode->SetPosition(daidalosengine::CVector3F(this->m_editCamera->GetCameraNode()->GetPosition().x, this->m_editCamera->GetLayer() * EDIT_CAMERA_LAYER_HEIGHT, this->m_editCamera->GetCameraNode()->GetPosition().z));

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
				this->m_meshNode->SetRotation(daidalosengine::CVector3F(0, (this->m_meshNode->GetRotation().y + 90), 0));
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
	void CLevelEditorApplication::UpdateMeshNode(const daidalosengine::CVector3F & rotation) {
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
			this->m_meshNode = new daidalosengine::CStaticMeshSceneNode(GetCurrentLevel()->GetScene()->GetScene(), GetCurrentLevel()->GetScene()->GetRootNode(), EDITOR_NODE_NAME_CURRENT, meshFilename, daidalosengine::CVector3F(), rotation, daidalosengine::CVector3F(1.0F, 1.0F, 1.0F));
			this->m_meshNode->SetId(0);
			this->m_meshNode->SetSaved(false);
			this->m_meshNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
			GetCurrentLevel()->GetScene()->GetRootNode()->AddChild(this->m_meshNode);
		}
		this->m_meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, textureDiffuseFilename, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, textureNormalFilename, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
	}

	/**
	* Update the visibility of the nodes
	*/
	void CLevelEditorApplication::UpdateNodesVisibility() {

		// Get all the mesh nodes
		std::vector<daidalosengine::ISceneNode *> nodes = GetCurrentLevel()->GetScene()->GetNodeArray(daidalosengine::SCENE_NODE_STATICMESH);
		std::vector<daidalosengine::ISceneNode *> animatedMeshNodes = GetCurrentLevel()->GetScene()->GetNodeArray(daidalosengine::SCENE_NODE_ANIMATEDMESH);
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

		this->m_meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, textureDiffuseFilename, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, textureNormalFilename, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
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
	void CLevelEditorApplication::Render3DScene(daidalosengine::C3DScene * scene3D) {

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

		// ------------------------------
		// Transparent
		// ------------------------------
		TransparentRendering(scene3D);

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
	* Get the node matching the position and the layer
	* @param position The position
	* @param layer The layer
	* @return A vector of nodes
	*/
	std::vector<daidalosengine::ISceneNode*> CLevelEditorApplication::GetNodesOnBlock(daidalosengine::CVector2I position, int layer) {
		std::vector<daidalosengine::ISceneNode*> node;
		std::vector<daidalosengine::ISceneNode*> nodes = GetCurrentLevel()->GetScene()->GetNodeArray(EDITOR_NODE_NAME_MULTIBRUSH);

		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			if((*it)->GetPosition().x > (position.x - EDIT_CAMERA_STEP) && (*it)->GetPosition().x < (position.x + EDIT_CAMERA_STEP) && (*it)->GetPosition().z >(position.y - EDIT_CAMERA_STEP) && (*it)->GetPosition().z < (position.y + EDIT_CAMERA_STEP) && (*it)->GetPosition().y >((layer * EDIT_CAMERA_LAYER_HEIGHT) - EDIT_CAMERA_LAYER_HEIGHT) && (*it)->GetPosition().y < ((layer * EDIT_CAMERA_LAYER_HEIGHT) + EDIT_CAMERA_LAYER_HEIGHT)) {
				node.push_back(*it);
			}
		}
		return node;
	}

	/**
	* Callback when the level is fully loaded
	*/
	void CLevelEditorApplication::OnLevelLoaded() {

		// Add the cameras
		daidalosengine::CProjectionCameraSceneNode * viewCameraNode = new daidalosengine::CProjectionCameraSceneNode(GetCurrentLevel()->GetScene()->GetScene(), GetCurrentLevel()->GetScene()->GetRootNode(), "viewCameraNode", daidalosengine::CVector3F(), daidalosengine::CVector3F(), daidalosengine::DegreeToRadian(55), this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y, 0.1F, 200.0F);
		viewCameraNode->SetSaved(false);
		GetCurrentLevel()->GetScene()->GetRootNode()->AddChild(viewCameraNode);

		this->m_viewCamera = std::unique_ptr<CViewCamera>(new CViewCamera(viewCameraNode));
		this->m_viewCamera->SetScene(GetCurrentLevel()->GetScene());
		this->m_viewCamera->SetPosition(this->m_cameraPosition);
		this->m_viewCamera->SetRotation(this->m_cameraRotation);

		daidalosengine::CProjectionCameraSceneNode * editCameraNode = new daidalosengine::CProjectionCameraSceneNode(GetCurrentLevel()->GetScene()->GetScene(), GetCurrentLevel()->GetScene()->GetRootNode(), "editCameraNode", daidalosengine::CVector3F(0, 100, 0), daidalosengine::CVector3F(80, 0, 0), daidalosengine::DegreeToRadian(55), this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y, 0.1F, 200.0F);
		editCameraNode->SetSaved(false);
		GetCurrentLevel()->GetScene()->GetRootNode()->AddChild(editCameraNode);

		// Add the player marker
		daidalosengine::ISceneNode * playerNode = GetCurrentLevel()->GetScene()->GetNode("player");
		if(playerNode != nullptr) {
			daidalosengine::CStaticMeshSceneNode * playerMeshNode = new daidalosengine::CStaticMeshSceneNode(GetCurrentLevel()->GetScene()->GetScene(), GetCurrentLevel()->GetScene()->GetRootNode(), "playerMarker", "cube.obj", daidalosengine::CVector3F(playerNode->GetAbsolutePosition().x, playerNode->GetAbsolutePosition().y - 3, playerNode->GetAbsolutePosition().z), daidalosengine::CVector3F(), daidalosengine::CVector3F(2, 4, 2));
			playerMeshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, EDITOR_PLAYER_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			playerMeshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, EDITOR_PLAYER_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			playerMeshNode->SetSaved(false);
			GetCurrentLevel()->GetScene()->GetRootNode()->AddChild(playerMeshNode);
		}

		this->m_editCamera = std::unique_ptr<CEditCamera>(new CEditCamera(editCameraNode));
		this->m_editCamera->SetScene(GetCurrentLevel()->GetScene());

		GetCurrentLevel()->GetScene()->GetRootNode()->SetAmbientColor(daidalosengine::CVector4F(1.0F, 1.0F, 1.0F, 1.0F));
		GetCurrentLevel()->GetScene()->GetRootNode()->SetUseGlobalAmbientColor(!this->m_drawLights);

		std::vector<daidalosengine::ISceneNode*> nodes = GetCurrentLevel()->GetScene()->GetNodeArray(EDITOR_NODE_NAME_BOUNDINGBOX);
		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			(*it)->SetEnabled(this->m_boundingBox);
		}

		this->m_activeCamera = viewCameraNode;
		this->m_consoleView->SetVisible(false);
		GetCurrentLevel()->GetView()->AddToMainView(this->m_consoleView);
		this->m_meshNode = nullptr;

		SetLoading(false);
	}

	/**
	* Callback when the next level is triggered
	*/
	void CLevelEditorApplication::OnTriggerNextLevel(const std::string & nextLevel) {
		// Nothing to do
	}
}
