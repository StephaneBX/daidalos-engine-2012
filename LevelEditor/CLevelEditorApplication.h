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

#ifndef __CLEVELEDITORAPPLICATION_H
#define __CLEVELEDITORAPPLICATION_H

#include <IForwardSampleApplication.h>
#include <Scene/3D/CProjectionCameraSceneNode.h>
#include "CEditorConfiguration.h"
#include "CViewCamera.h"
#include "CEditCamera.h"

namespace EDITOR {
	/**
	* Level editor application implementation
	*/
	class CLevelEditorApplication : public SAMPLES::IForwardSampleApplication {
	private:
		bool m_viewMode;
		bool m_drawSkybox;
		bool m_drawLights;
		bool m_culling;

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

		DE::CProjectionCameraSceneNode * m_viewCameraNode;
		DE::CProjectionCameraSceneNode * m_editCameraNode;

		DE::CVector3F m_cameraPosition;
		DE::CVector3F m_cameraRotation;

		DE::CStaticMeshSceneNode * m_meshNode;

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
		* Update the application
		* @param time Elapsed time
		*/
		void Update(const float time);

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
		void UpdateMeshNode(const DE::CVector3F & rotation = DE::CVector3F());

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
		void Render3DScene(DE::C3DScene * scene3D);

		/**
		* Do post level loading initialization
		*/
		void PostLoadLevel();

		/**
		* Get the node atching the position, the laer and the name
		* @param position The position
		* @param layer The layer
		* @return A vector of nodes
		*/
		std::vector<DE::ISceneNode *> GetNodesOnBlock(DE::CVector2I position, int layer);

		/**
		* Render the diffuse part
		* @param scene3D The scene
		*/
		void DiffuseRendering(DE::C3DScene * scene3D);
	};
}

#endif
