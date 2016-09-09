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

#include <Core/CConfigurationManager.h>
#include <Core/Utility/CTokenizer.h>
#include <Core/CInputDevice.h>
#include "CEditorConfiguration.h"

namespace EDITOR {
	/**
	* Constructor
	*/
	CEditorConfiguration::CEditorConfiguration() {
		LoadConfiguration();
	}

	/**
	* Load the configuration
	*/
	void CEditorConfiguration::LoadConfiguration() {
		this->m_configuration = DE::CConfigurationManager::GetInstance()->GetConfiguration("Resources/Engine/editor.ini");

		// Read the configuration
		DE::CTokenizer tokenizer;

		// Read the mesh
		tokenizer.AddLine(this->m_configuration.GetValueString("mesh", "ground", ""));
		tokenizer.Tokenize(":");
		this->m_groundMeshFilename.insert(this->m_groundMeshFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("mesh", "wall", ""));
		tokenizer.Tokenize(":");
		this->m_wallMeshFilename.insert(this->m_wallMeshFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("mesh", "ceiling", ""));
		tokenizer.Tokenize(":");
		this->m_ceilingMeshFilename.insert(this->m_ceilingMeshFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		// Read the diffuse textures
		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_diffuse", "ground", ""));
		tokenizer.Tokenize(":");
		this->m_groundTextureDiffuseFilename.insert(this->m_groundTextureDiffuseFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_diffuse", "wall", ""));
		tokenizer.Tokenize(":");
		this->m_wallTextureDiffuseFilename.insert(this->m_wallTextureDiffuseFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_diffuse", "ceiling", ""));
		tokenizer.Tokenize(":");
		this->m_ceilingTextureDiffuseFilename.insert(this->m_ceilingTextureDiffuseFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		// Read the normal textures
		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_normal", "ground", ""));
		tokenizer.Tokenize(":");
		this->m_groundTextureNormalFilename.insert(this->m_groundTextureNormalFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_normal", "wall", ""));
		tokenizer.Tokenize(":");
		this->m_wallTextureNormalFilename.insert(this->m_wallTextureNormalFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());

		tokenizer.Clear();
		tokenizer.AddLine(this->m_configuration.GetValueString("texture_normal", "ceiling", ""));
		tokenizer.Tokenize(":");
		this->m_ceilingTextureNormalFilename.insert(this->m_ceilingTextureNormalFilename.end(), tokenizer.GetTokens().begin(), tokenizer.GetTokens().end());


		this->m_inputDevice = DE::CInputDevice::GetInstance();

		DE::SInputActionDeclaration decl[] = {
			{ EDITOR_ACTION_EXIT, -1 },
			{ EDITOR_ACTION_CAMERA_UP, EDITOR_ACTION_CAMERA_DOWN },
			{ EDITOR_ACTION_CAMERA_DOWN, EDITOR_ACTION_CAMERA_UP },
			{ EDITOR_ACTION_CAMERA_LEFT, EDITOR_ACTION_CAMERA_RIGHT },
			{ EDITOR_ACTION_CAMERA_RIGHT, EDITOR_ACTION_CAMERA_LEFT },
			{ EDITOR_ACTION_CAMERA_ZOOM_IN, EDITOR_ACTION_CAMERA_ZOOM_OUT },
			{ EDITOR_ACTION_CAMERA_ZOOM_OUT, EDITOR_ACTION_CAMERA_ZOOM_IN },
			{ EDITOR_ACTION_CHANGE_MODE, -1 },
			{ EDITOR_ACTION_LIGHTS, -1 },
			{ EDITOR_ACTION_CULLING, -1 },
			{ EDITOR_ACTION_REFRESH, -1 },
			{ EDITOR_ACTION_MESH_ROTATE, -1 },
			{ EDITOR_ACTION_MESH_NEXT, -1 },
			{ EDITOR_ACTION_TEXTURE_NEXT, -1 },
			{ EDITOR_ACTION_SELECT_GROUND, -1 },
			{ EDITOR_ACTION_SELECT_WALL, -1 },
			{ EDITOR_ACTION_SELECT_CEILING, -1 },
			{ EDITOR_ACTION_PUT_MESH, -1 },
			{ EDITOR_ACTION_DELETE_MESH, -1 },
			{ EDITOR_ACTION_CAMERA_LAYER_UP, EDITOR_ACTION_CAMERA_LAYER_DOWN },
			{ EDITOR_ACTION_CAMERA_LAYER_DOWN, EDITOR_ACTION_CAMERA_LAYER_UP },
			{ EDITOR_ACTION_CONSOLE, -1 }
		};

		this->m_inputDevice->AddActions(decl);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_EXIT, DE::INPUT_KEY_KEYBOARD_ESCAPE);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_UP, DE::INPUT_KEY_KEYBOARD_Z);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_DOWN, DE::INPUT_KEY_KEYBOARD_S);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_LEFT, DE::INPUT_KEY_KEYBOARD_Q);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_RIGHT, DE::INPUT_KEY_KEYBOARD_D);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_ZOOM_IN, DE::INPUT_KEY_KEYBOARD_KP_PLUS);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_ZOOM_OUT, DE::INPUT_KEY_KEYBOARD_KP_MINUS);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CHANGE_MODE, DE::INPUT_KEY_KEYBOARD_TAB);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_LIGHTS, DE::INPUT_KEY_KEYBOARD_KP0);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CULLING, DE::INPUT_KEY_KEYBOARD_KP1);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_REFRESH, DE::INPUT_KEY_KEYBOARD_F5);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_MESH_ROTATE, DE::INPUT_KEY_KEYBOARD_R);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_MESH_NEXT, DE::INPUT_KEY_KEYBOARD_E);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_TEXTURE_NEXT, DE::INPUT_KEY_KEYBOARD_T);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_SELECT_GROUND, DE::INPUT_KEY_KEYBOARD_1);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_SELECT_WALL, DE::INPUT_KEY_KEYBOARD_2);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_SELECT_CEILING, DE::INPUT_KEY_KEYBOARD_3);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_PUT_MESH, DE::INPUT_KEY_KEYBOARD_SPACE);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_DELETE_MESH, DE::INPUT_KEY_KEYBOARD_DELETE);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_LAYER_UP, DE::INPUT_KEY_KEYBOARD_PAGEUP);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CAMERA_LAYER_DOWN, DE::INPUT_KEY_KEYBOARD_PAGEDOWN);
		this->m_inputDevice->SetActionKey(EDITOR_ACTION_CONSOLE, DE::INPUT_KEY_KEYBOARD_KP_MULTIPLY);
	}

	/**
	* Get the list of configured grounds
	* @return A list of configured grounds
	*/
	const std::vector<std::string> & CEditorConfiguration::GetGroundMeshFilenames() const {
		return this->m_groundMeshFilename;
	}

	/**
	* Get the list of configured walls
	* @return A list of configured walls
	*/
	const std::vector<std::string> & CEditorConfiguration::GetWallMeshFilenames() const {
		return this->m_wallMeshFilename;
	}

	/**
	* Get the list of configured ceilings
	* @return A list of configured ceilings
	*/
	const std::vector<std::string> & CEditorConfiguration::GetCeilingMeshFilenames() const {
		return this->m_ceilingMeshFilename;
	}

	/**
	* Get the list of configured grounds textures
	* @return A list of configured grounds textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetGroundTextureDiffuseFilenames() const {
		return this->m_groundTextureDiffuseFilename;
	}

	/**
	* Get the list of configured walls textures
	* @return A list of configured walls textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetWallTextureDiffuseFilenames() const {
		return this->m_wallTextureDiffuseFilename;
	}

	/**
	* Get the list of configured ceilings textures
	* @return A list of configured ceilings textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetCeilingTextureDiffuseFilenames() const {
		return this->m_ceilingTextureDiffuseFilename;
	}

	/**
	* Get the list of configured grounds textures
	* @return A list of configured grounds textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetGroundTextureNormalFilenames() const {
		return this->m_groundTextureNormalFilename;
	}

	/**
	* Get the list of configured walls textures
	* @return A list of configured walls textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetWallTextureNormalFilenames() const {
		return this->m_wallTextureNormalFilename;
	}

	/**
	* Get the list of configured ceilings textures
	* @return A list of configured ceilings textures
	*/
	const std::vector<std::string> & CEditorConfiguration::GetCeilingTextureNormalFilenames() const {
		return this->m_ceilingTextureNormalFilename;
	}
}
