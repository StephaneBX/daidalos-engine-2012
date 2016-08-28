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

#ifndef __CEDITORCONFIGURATION_H
#define __CEDITORCONFIGURATION_H

#include <SiniP/SiniP.h>
#include <vector>

namespace EDITOR {
	enum EEditorActionConfiguration {
		EDITOR_ACTION_EXIT = 1 << 1,
		EDITOR_ACTION_CAMERA_UP = 1 << 2,
		EDITOR_ACTION_CAMERA_DOWN = 1 << 3,
		EDITOR_ACTION_CAMERA_LEFT = 1 << 4,
		EDITOR_ACTION_CAMERA_RIGHT = 1 << 5,
		EDITOR_ACTION_CAMERA_ZOOM_IN = 1 << 6,
		EDITOR_ACTION_CAMERA_ZOOM_OUT = 1 << 7,
		EDITOR_ACTION_CHANGE_MODE = 1 << 8,
		EDITOR_ACTION_LIGHTS = 1 << 9,
		EDITOR_ACTION_CULLING = 1 << 10,
		EDITOR_ACTION_REFRESH = 1 << 11,
		EDITOR_ACTION_MESH_ROTATE = 1 << 12,
		EDITOR_ACTION_MESH_NEXT = 1 << 13,
		EDITOR_ACTION_TEXTURE_NEXT = 1 << 14,
		EDITOR_ACTION_SELECT_GROUND = 1 << 15,
		EDITOR_ACTION_SELECT_WALL = 1 << 16,
		EDITOR_ACTION_SELECT_CEILING = 1 << 17,
		EDITOR_ACTION_PUT_MESH = 1 << 18,
		EDITOR_ACTION_DELETE_MESH = 1 << 19,
		EDITOR_ACTION_CAMERA_LAYER_UP = 1 << 20,
		EDITOR_ACTION_CAMERA_LAYER_DOWN = 1 << 21
	};

	/**
	* Level editor configuration
	*/
	class CEditorConfiguration {
	private:
		SiniP m_configuration;
		DE::CInputDevice * m_inputDevice;

		std::vector<std::string> m_groundMeshFilename;
		std::vector<std::string> m_wallMeshFilename;
		std::vector<std::string> m_ceilingMeshFilename;

		std::vector<std::string> m_groundTextureDiffuseFilename;
		std::vector<std::string> m_wallTextureDiffuseFilename;
		std::vector<std::string> m_ceilingTextureDiffuseFilename;

		std::vector<std::string> m_groundTextureNormalFilename;
		std::vector<std::string> m_wallTextureNormalFilename;
		std::vector<std::string> m_ceilingTextureNormalFilename;

	public:
		/**
		* Constructor
		*/
		CEditorConfiguration();

		/**
		* Load the configuration
		*/
		void LoadConfiguration();

		/**
		* Get the list of configured grounds
		* @return A list of configured grounds
		*/
		const std::vector<std::string> & GetGroundMeshFilenames() const;

		/**
		* Get the list of configured walls
		* @return A list of configured walls
		*/
		const std::vector<std::string> & GetWallMeshFilenames() const;

		/**
		* Get the list of configured ceilings
		* @return A list of configured ceilings
		*/
		const std::vector<std::string> & GetCeilingMeshFilenames() const;

		/**
		* Get the list of configured grounds textures
		* @return A list of configured grounds textures
		*/
		const std::vector<std::string> & GetGroundTextureDiffuseFilenames() const;

		/**
		* Get the list of configured walls textures
		* @return A list of configured walls textures
		*/
		const std::vector<std::string> & GetWallTextureDiffuseFilenames() const;

		/**
		* Get the list of configured ceilings textures
		* @return A list of configured ceilings textures
		*/
		const std::vector<std::string> & GetCeilingTextureDiffuseFilenames() const;

		/**
		* Get the list of configured grounds textures
		* @return A list of configured grounds textures
		*/
		const std::vector<std::string> & GetGroundTextureNormalFilenames() const;

		/**
		* Get the list of configured walls textures
		* @return A list of configured walls textures
		*/
		const std::vector<std::string> & GetWallTextureNormalFilenames() const;

		/**
		* Get the list of configured ceilings textures
		* @return A list of configured ceilings textures
		*/
		const std::vector<std::string> & GetCeilingTextureNormalFilenames() const;
	};
}

#endif
