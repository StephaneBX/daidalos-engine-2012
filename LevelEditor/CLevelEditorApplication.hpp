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

#ifndef __CLEVELEDITORAPPLICATION_HPP
#define __CLEVELEDITORAPPLICATION_HPP

#include <IForwardApplication.hpp>
#include <Scene/3D/CProjectionCameraSceneNode.hpp>
#include <Scene/3D/CStaticMeshSceneNode.hpp>
#include "CEditorConfiguration.hpp"
#include "3D/CViewCamera.hpp"
#include "3D/CEditCamera.hpp"
#include "2D/CConsoleView.hpp"
#include "2D/IConsoleListener.hpp"

namespace daidaloseditor {
	/**
	* Level editor application implementation
	*/
	class CLevelEditorApplication : public daidalosgameengine::IForwardApplication, public IConsoleListener {
	private:
		bool m_viewMode;
		bool m_drawSkybox;
		bool m_drawLights;
		bool m_boundingBox;

		int m_meshCategorySelectionMode;
		int m_groundMeshEditIndex;
		int m_groundTextureEditIndex;
		int m_wallMeshEditIndex;
		int m_wallTextureEditIndex;
		int m_ceilingMeshEditIndex;
		int m_ceilingTextureEditIndex;

		CEditorConfiguration m_configuration;

		std::unique_ptr<CViewCamera> m_viewCamera;
		std::unique_ptr<CEditCamera> m_editCamera;

		daidalosengine::CProjectionCameraSceneNode * m_viewCameraNode;
		daidalosengine::CProjectionCameraSceneNode * m_editCameraNode;

		daidalosengine::CVector3F m_cameraPosition;
		daidalosengine::CVector3F m_cameraRotation;

		daidalosengine::CStaticMeshSceneNode * m_meshNode;

		CConsoleView * m_consoleView;

		daidalosengine::ICameraSceneNode * m_activeCamera;

	public:
		/**
		* Constructor
		* @param hInstance Instance
		* @param nCmdShow Commands
		*/
		CLevelEditorApplication(const HINSTANCE hInstance, const int nCmdShow);

		/**
		* Destructor
		*/
		~CLevelEditorApplication();

		/**
		* Load a level and show a loading screen
		* Should be called from a new thread
		* @param filename The level filename
		*/
		void LoadLevel(const std::string & filename);

		/**
		* Update the application
		* @param time Elapsed time
		*/
		void Update(const float time);

		/**
		* Called when a load command is inputed in the console
		* @param filename The level filename to load
		*/
		void OnConsoleCommandLoad(const std::string filename);

		/**
		* Called when a save command is inputed in the console
		* @param filename The level filename to save the current scene into
		*/
		void OnConsoleCommandSave(const std::string filename);

	private:
		/**
		* Update the view mode
		* @param time Elapsed time
		*/
		void UpdateViewMode(const float time);

		/**
		* Update the edit mode
		* @param time Elapsed time
		*/
		void UpdateEditMode(const float time);

		/**
		* Update the selection mesh node
		* @param rotation The rotation
		*/
		void UpdateMeshNode(const daidalosengine::CVector3F & rotation = daidalosengine::CVector3F());

		/**
		* Update the next texture to apply depending on the mesh category selection
		*/
		void UpdateTextureNext();

		/**
		* Update the next mesh to apply depending on the mesh category selection
		*/
		void UpdateMeshNext();

		/**
		* Update the visibility of the nodes
		*/
		void UpdateNodesVisibility();

		/**
		* Render a 3D scene
		* @param scene3D The scene
		*/
		void Render3DScene(daidalosengine::C3DScene * scene3D);

		/**
		* Get the node matching the position and the layer
		* @param position The position
		* @param layer The layer
		* @return A vector of nodes
		*/
		std::vector<daidalosengine::ISceneNode *> GetNodesOnBlock(daidalosengine::CVector2I position, int layer);

		/**
		* Callback when the level is fully loaded
		*/
		void OnLevelLoaded();

		/**
		* Callback when the next level is triggered
		*/
		void OnTriggerNextLevel(const std::string & nextLevel);
	};
}

#endif
