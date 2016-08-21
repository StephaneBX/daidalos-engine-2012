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
